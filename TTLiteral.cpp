#include "TTLiteral.h"
#include <cstring>

TTLiteral *TTLiteral::copy(MemAllocator *allocator)
{
    // TODO: simple copy, since we don't need to recurse and this value is held as a single instance
    // TODO: only, so the GC will be ok with this.
    // TODO: Also, no object references can be yet referenced from these data.

    MemAllocator *alloc = MemAllocator::getCurrent();
    TTLiteral *lit = alloc->allocateLiteral();

    lit->type = type;
    lit->length = length;
    lit->data = alloc->allocate(length);
    memcpy(lit->data, data, length);

    return lit;
}

TTLiteral *TTLiteral::clone()
{
    return copy(MemAllocator::getCurrent());
}

TTObject *TTLiteral::onMessage(TTObject *object)
{
    std::cout << "TTLiteral<";

    switch(type)
    {
        case LITERAL_TYPE_INTEGER:
            std::cout << "Integer";
            break;
        case LITERAL_TYPE_STRING:
            std::cout << "String";
            break;
        case LITERAL_TYPE_OBJECT_ARRAY:
            std::cout << "ObjectArray";
            break;
        default:
            std::cout << "Invalid literal type" << std::endl;
            throw std::exception();
    }
    std::cout << ">::onMessage(): " << std::endl;
    return TTObject::createObject(TT_NIL);
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
    }

    return res;
}

void TTLiteral::printValue(std::ostream &os)
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
            os << "not implemented";
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
