#include "MemAllocator.h"
#include <cstring>
#include <cstdlib>

MemAllocator::MemAllocator(size_t poolCapacity)
    : capacity(poolCapacity), top(0)
{
    pool = (uint8_t *) malloc(poolCapacity);
    memset(pool, 0, poolCapacity);
}

MemAllocator::~MemAllocator()
{
    free(pool);
}

uint8_t *MemAllocator::allocate(size_t bytes)
{
    uint8_t *nextAddr = (uint8_t *) top;

    if((top + bytes) < capacity)
    {
        top += bytes;
        return nextAddr;
    }
    return 0;
}

MemAllocator *MemAllocator::defaultAllocator = NULL;

MemAllocator *MemAllocator::getDefault()
{
    return defaultAllocator;
}

void MemAllocator::initializeDefaultAllocator(size_t poolCapacity)
{
    defaultAllocator = new MemAllocator(poolCapacity);
}

void MemAllocator::cleanupDefaultAllocator()
{
    delete defaultAllocator;
    defaultAllocator = NULL;
}