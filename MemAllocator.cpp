#include "MemAllocator.h"
#include "common.h"
#include <cstring>
#include <iostream>

#undef DEBUG

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
    std::cout << "~MemAllocator(): " << std::dec << top << " bytes allocated" << std::endl;
#endif
    free(pool);
}

uint8_t *MemAllocator::allocate(size_t bytes)
{
    uint8_t *nextAddr = pool + top;

#ifdef DEBUG
    std::cout << "MemAllocator::allocate(): called to allocate " << bytes << " bytes." << std::endl;
#endif

    if ((top + bytes) < capacity)
    {
        top += bytes;
        memset(nextAddr, 0, bytes);
        return nextAddr;
    }

    std::cerr << "OUT OF MEMORY" << std::endl;
    throw std::exception();
    return 0;
}

uint8_t *MemAllocator::allocateString(const uint8_t *str)
{
    size_t size = strlen((const char *) str);
    uint8_t *newStr = allocate(sizeof(char) * (size + 1));
    newStr[size] = '\0';
    return newStr;
}

uint8_t *MemAllocator::cloneString(const uint8_t *str)
{
    uint8_t *newStr = allocateString(str);
    strcpy((char *) newStr, (const char *) str);

    return newStr;
}

TTObject *MemAllocator::allocateObject()
{
    return (TTObject *) allocate(sizeof(TTObject));
}

TTLiteral *MemAllocator::allocateLiteral()
{
    return (TTLiteral *) allocate(sizeof(TTLiteral));
}

MemAllocator *MemAllocator::defaultAllocator = NULL;

MemAllocator *MemAllocator::getCurrent()
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
