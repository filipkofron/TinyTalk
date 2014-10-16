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
    return TTObject::createObject(TT_NIL);
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
