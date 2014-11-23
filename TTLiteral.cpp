#include "TTLiteral.h"
#include "common.h"
#include "Runtime.h"
#include <cstring>

TTLiteral *TTLiteral::copy(MemAllocator *allocator, TTLiteral *lit)
{
    // TODO: simple copy, since we don't need to recurse and this value is held as a single instance
    // TODO: only, so the GC will be ok with this.
    // TODO: Also, no object references can be yet referenced from these data.

    MemAllocator *alloc = MemAllocator::getCurrent();
    TTLiteral *newLit = alloc->allocateLiteral();

    newLit->type = lit->type;
    newLit->length = lit->length;
    newLit->data = alloc->allocate(lit->length);
    memcpy(newLit->data, lit->data, lit->length);

    return newLit;
}

TTLiteral *TTLiteral::clone(TTLiteral *lit)
{
    return copy(MemAllocator::getCurrent(), lit);
}

TTObject *TTLiteral::onMessage(TTObject *dest, std::string &name, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    switch(type)
    {
        case LITERAL_TYPE_INTEGER:
            return integerOnMessage(dest, name, argNames, values);
        case LITERAL_TYPE_STRING:
            return stringOnMessage(dest, name, argNames, values);
        case LITERAL_TYPE_OBJECT_ARRAY:
            std::cout << "ObjectArray";
            break;
        case LITERAL_TYPE_BYTE_ARRAY:
            std::cout << "ByteArray";
            break;
        default:
            std::cout << "Invalid literal type" << std::endl;
            throw std::exception();
    }
    std::cout << "::onMessage(): " << std::endl;
    return Runtime::globalEnvironment->getField(TO_TT_STR("nil"));
}

TTObject *TTLiteral::stringOnMessage(TTObject *dest, std::string &name, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    std::string lookupName = "string_";
    lookupName.append(name);
    std::shared_ptr<Builtin> builtin = Runtime::builtinPool.lookupBultin(lookupName);
    if(builtin)
    {
        return builtin->invoke(dest, argNames, values);
    }

    std::cerr << "String: message not understood: " << name << std::endl;
    throw std::exception();
}

TTObject *TTLiteral::integerOnMessage(TTObject *dest, std::string &name, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    std::string lookupName = "integer_";
    lookupName.append(name);
    std::shared_ptr<Builtin> builtin = Runtime::builtinPool.lookupBultin(lookupName);
    if(builtin)
    {
        return builtin->invoke(dest, argNames, values);
    }

    std::cerr << "Integer: message not understood: " << name << std::endl;
    throw std::exception();
}

const char *TTLiteral::getTypeInfo()
{
    const char *res = "INVALID";

    switch(type)
    {
        case LITERAL_TYPE_INTEGER:
            res = "INTEGER";
            break;
        case LITERAL_TYPE_STRING:
            res = "STRING";
            break;
        case LITERAL_TYPE_OBJECT_ARRAY:
            res = "OBJECT ARRAY";
            break;
        case LITERAL_TYPE_BYTE_ARRAY:
            res = "BYTE ARRAY";
            break;
    }

    return res;
}

void TTLiteral::printValue(std::ostream &os, const uint32_t &level, const bool &recursive)
{
    switch(type)
    {
        case LITERAL_TYPE_INTEGER:
            os << *((int32_t *) data);
            break;
        case LITERAL_TYPE_STRING:
            os << (const char *) data;
            break;
        case LITERAL_TYPE_OBJECT_ARRAY:
            os << "byte size: " << length << std::endl;
            for(uint32_t i = 0; i < length / sizeof(TTObject *); i++)
            {
                prlvl(os, level);
                os << "[" << i << "] -> ";
                if(((TTObject **) data)[i])
                {
                    if(recursive)
                    {
                        ((TTObject **) data)[i]->print(os, level + 1, recursive);
                    }
                    else
                    {
                        os << "<non-recursive-print>";
                    }
                }
                else
                {
                    os << "NULL";
                }
                os << std::endl;
            }
            prlvl(os, level - 1);
        case LITERAL_TYPE_BYTE_ARRAY:
            os << "size: " << length << std::endl;
            for(uint32_t i = 0; i < length / sizeof(TTObject *); i++)
            {
                prlvl(os, level);
                os << "[" << i << "] -> ";
                if(((TTObject **) data)[i])
                {
                    if(recursive)
                    {
                        os << (unsigned) data[i];
                    }
                    else
                    {
                        os << "<non-recursive-print>";
                    }
                }
                else
                {
                    os << "NULL";
                }
                os << std::endl;
            }
            prlvl(os, level - 1);
            break;
    }
}

TTLiteral *TTLiteral::createStringLiteral(uint32_t length)
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    TTLiteral *lit = alloc->allocateLiteral();

    lit->type = LITERAL_TYPE_STRING;
    lit->length = length + 1;
    lit->data = alloc->allocate(lit->length);
    lit->data[length] = '\0';

    return lit;
}

TTLiteral *TTLiteral::createIntegerLiteral()
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    TTLiteral *lit = alloc->allocateLiteral();

    lit->type = LITERAL_TYPE_INTEGER;
    lit->length = sizeof(uint32_t);
    lit->data = alloc->allocate(lit->length);

    return lit;
}

TTLiteral *TTLiteral::createObjectArray(uint32_t size)
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    TTLiteral *lit = alloc->allocateLiteral();

    lit->type = LITERAL_TYPE_OBJECT_ARRAY;
    lit->length = sizeof(TTObject *) * size;
    lit->data = alloc->allocate(lit->length);

    return lit;
}

TTLiteral *TTLiteral::createByteArray(uint32_t size)
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    TTLiteral *lit = alloc->allocateLiteral();

    lit->type = LITERAL_TYPE_BYTE_ARRAY;
    lit->length = size;
    lit->data = alloc->allocate(lit->length);

    return lit;
}

TTLiteral *TTLiteral::createStringLiteral(const uint8_t *str)
{
    TTLiteral *lit = createStringLiteral(strlen((const char *) str));
    strcpy((char *)lit->data, (const char *)str);
    return lit;
}

TTLiteral *TTLiteral::createIntegerLiteral(const int32_t &value)
{
    TTLiteral *lit = createIntegerLiteral();

    *((int32_t *) lit->data) = value;

    return lit;
}

TTLiteral *TTLiteral::createObjectArray(const std::vector<TTObject *> &objects)
{
    TTLiteral *lit = createObjectArray(objects.size());

    TTObject **dataObs = ((TTObject **) lit->data);

    for(auto object : objects)
    {
        *dataObs = object;
        dataObs++;
    }

    return lit;
}

TTLiteral *TTLiteral::createByteArray(const std::vector<uint8_t> &bytes)
{
    TTLiteral *lit = createByteArray(bytes.size());

    uint8_t *dataBytes = lit->data;

    for(auto byt : bytes)
    {
        *dataBytes = byt;
        dataBytes++;
    }

    return lit;
}
