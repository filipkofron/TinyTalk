#include "MemAllocator.h"
#include "common.h"
#include <cstring>
#include <iostream>

MemAllocator::MemAllocator(size_t poolCapacity)
        : capacity(poolCapacity), top(0)
{
    pool = (uint8_t *) malloc(poolCapacity);
    memset(pool, 0, poolCapacity);
#ifdef DEBUG
    std::cout << "MemAllocator(): Created with capacity of " << poolCapacity
            << " bytes (" << (poolCapacity / (1024 * 1024)) << " MB)." << std::endl;
#endif
}

MemAllocator::~MemAllocator()
{
#ifdef DEBUG
    std::cout << "~MemAllocator(): " << top << " bytes allocated" << std::endl;
#endif
    free(pool);
}

uint8_t *MemAllocator::allocate(size_t bytes)
{
    uint8_t *nextAddr = (uint8_t *) top;

    if ((top + bytes) < capacity)
    {
        top += bytes;
        return nextAddr;
    }
    return 0;
}

uint8_t *MemAllocator::allocateString(const uint8_t *str)
{
    size_t size = strlen((const char *) str);
    uint8_t *newStr = MemAllocator::getDefault()->allocate(sizeof(char) * (size + 1));
    newStr[size] = '\0';
    return newStr;
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
