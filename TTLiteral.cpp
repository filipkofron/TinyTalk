#include "TTLiteral.h"
#include "TTObject.h"
#include "common.h"
#include "Runtime.h"
#include <cstring>

RefPtr<TTLiteral> TTLiteral::copy(MemAllocator *allocator, RefPtr<TTLiteral> lit)
{
    // TODO: simple copy, since we don't need to recurse and this value is held as a single instance
    // TODO: only, so the GC will be ok with this.
    // TODO: Also, no object references can be yet referenced from these data.

    MemAllocator *alloc = MemAllocator::getCurrent();
    RefPtr<TTLiteral> newLit = alloc->allocateLiteral();

    newLit->type = lit->type;
    newLit->length = lit->length;
    newLit->data = alloc->allocate(lit->length);
    memcpy(newLit->data, lit->data, lit->length);

    return newLit;
}

RefPtr<TTLiteral> TTLiteral::clone(RefPtr<TTLiteral> lit)
{
    return copy(MemAllocator::getCurrent(), lit);
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
                if(&((TTObject **) data)[i])
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
            for(uint32_t i = 0; i < length; i++)
            {
                prlvl(os, level);
                os << "[" << i << "] -> ";
                    if(recursive)
                    {
                        os << (unsigned) data[i];
                    }
                    else
                    {
                        os << "<non-recursive-print>";
                    }
                os << std::endl;
            }
            prlvl(os, level - 1);
            break;
    }
}

RefPtr<TTObject> TTLiteral::createStringLiteral(uint32_t length)
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    RefPtr<TTLiteral> lit = alloc->allocateLiteral();

    RefPtr<TTObject> obj = TTObject::createObject(TT_LITERAL);
    RefPtr<TTObject> parent = Runtime::globalEnvironment->getField(TO_TT_STR("String"));
    obj->setField(TO_TT_STR("parent"), parent);

    lit->type = LITERAL_TYPE_STRING;
    lit->length = length + 1;
    lit->data = alloc->allocate(lit->length);
    lit->data[length] = '\0';

    obj->setLiteral(lit);

    return obj;
}

RefPtr<TTObject> TTLiteral::createIntegerLiteral()
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    RefPtr<TTLiteral> lit = alloc->allocateLiteral();

    RefPtr<TTObject> obj = TTObject::createObject(TT_LITERAL);
    RefPtr<TTObject> parent = Runtime::globalEnvironment->getField(TO_TT_STR("Integer"));
    obj->setField(TO_TT_STR("parent"), parent);

    lit->type = LITERAL_TYPE_INTEGER;
    lit->length = sizeof(uint32_t);
    lit->data = alloc->allocate(lit->length);

    obj->setLiteral(lit);

    return obj;
}

RefPtr<TTObject> TTLiteral::createObjectArray(uint32_t size)
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    RefPtr<TTLiteral> lit = alloc->allocateLiteral();

    RefPtr<TTObject> obj = TTObject::createObject(TT_LITERAL);
    RefPtr<TTObject> parent = Runtime::globalEnvironment->getField(TO_TT_STR("Array"));
    obj->setField(TO_TT_STR("parent"), parent);

    lit->type = LITERAL_TYPE_OBJECT_ARRAY;
    lit->length = sizeof(TTObject *) * size;
    lit->data = alloc->allocate(lit->length);

    obj->setLiteral(lit);

    return obj;
}

RefPtr<TTObject> TTLiteral::createByteArray(uint32_t size)
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    RefPtr<TTLiteral> lit = alloc->allocateLiteral();

    RefPtr<TTObject> obj = TTObject::createObject(TT_LITERAL);

    lit->type = LITERAL_TYPE_BYTE_ARRAY;
    lit->length = size;
    lit->data = alloc->allocate(lit->length);

    obj->setLiteral(&lit);

    return obj;
}

RefPtr<TTObject> TTLiteral::createStringLiteral(const uint8_t *str)
{
    RefPtr<TTObject> obj = createStringLiteral(strlen((const char *) str));
    strcpy((char *)obj->getLiteral()->data, (const char *)str);
    return obj;
}

RefPtr<TTObject> TTLiteral::createIntegerLiteral(const int32_t &value)
{
    RefPtr<TTObject> obj = createIntegerLiteral();

    *((int32_t *) obj->getLiteral()->data) = value;

    return obj;
}

RefPtr<TTObject> TTLiteral::createObjectArray(const std::vector<RefPtr<TTObject> > &objects)
{
    RefPtr<TTObject> obj = createObjectArray(objects.size());

    TTObject **dataObs = ((TTObject **) obj->getLiteral()->data);

    for(auto object : objects)
    {
        *dataObs = &object;
        dataObs++;
    }

    return obj;
}

RefPtr<TTObject> TTLiteral::createByteArray(const std::vector<uint8_t> &bytes)
{
    RefPtr<TTObject> obj = createByteArray(bytes.size());

    uint8_t *dataBytes = obj->getLiteral()->data;

    for(auto &byt : bytes)
    {
        *dataBytes = byt;
        dataBytes++;
    }

    return obj;
}

void TTLiteral::setLiteralParent(RefPtr<TTObject> obj, RefPtr<TTLiteral> lit)
{
    switch(lit->type)
    {
        case LITERAL_TYPE_STRING:
            obj->setField(TO_TT_STR("parent"), Runtime::globalEnvironment->getField(TO_TT_STR("String")));
            break;
        case LITERAL_TYPE_INTEGER:
            obj->setField(TO_TT_STR("parent"), Runtime::globalEnvironment->getField(TO_TT_STR("Integer")));
            break;
        case LITERAL_TYPE_OBJECT_ARRAY:
            obj->setField(TO_TT_STR("parent"), Runtime::globalEnvironment->getField(TO_TT_STR("Array")));
            break;
        case LITERAL_TYPE_BYTE_ARRAY:
            // TODO: add parent
            break;
        default:
            std::cerr << "Unsupported literal type!" << std::endl;
            throw std::exception();
    }
}
