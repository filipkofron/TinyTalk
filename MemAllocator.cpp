#include "common.h"
#include "MemAllocator.h"
#include "Runtime.h"
#include <cstring>
#include <iostream>

MemAllocator *MemAllocator::defaultAllocator = NULL;

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
#ifdef VERBOSE
    std::cout << "~MemAllocator(): " << std::dec << top << " bytes allocated" << std::endl;
#endif
    free(pool);
}

bool MemAllocator::isInside(uintptr_t ptr)
{
    return ptr >= (uintptr_t ) pool && ptr < (uintptr_t) &pool[capacity];
}

uint8_t *MemAllocator::allocate(size_t bytes)
{
    /*uint8_t *hack = (uint8_t *)  malloc(bytes);
    memset(hack, 0, bytes);
    return hack;*/
    uint8_t *nextAddr = pool + top;

#ifdef DEBUG
    std::cout << "MemAllocator::allocate(): called to allocate " << bytes << " bytes, remaining: " << (capacity - (top + bytes)) << std::endl;
#endif

    if ((top + bytes) < capacity)
    {
        top += bytes;
        memset(nextAddr, 0, bytes);
        return nextAddr;
    }

#ifdef DEBUG
    std::cout << "Out of memory, this is " << (this == defaultAllocator ? "" : "not ") << "default allocator" << std::endl;
#endif

    Runtime::runCopyGC();

    return getCurrent()->allocate(bytes);
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

TTObject* MemAllocator::allocateObject()
{
#ifdef DEBUG
    TTObject *obj = (TTObject *) allocate(sizeof(TTObject));
    objects.insert((uintptr_t) obj);
    return obj;
#else
    return (TTObject *) allocate(sizeof(TTObject));
#endif
}

TTLiteral* MemAllocator::allocateLiteral()
{
#ifdef DEBUG
    TTLiteral *lit = (TTLiteral *) allocate(sizeof(TTLiteral));
    literals.insert((uintptr_t) lit);
    return lit;
#else
    return (TTLiteral *) allocate(sizeof(TTLiteral));
#endif
}

MemAllocator *MemAllocator::getCurrent()
{
    return defaultAllocator;
}

void MemAllocator::initializeDefaultAllocator(size_t poolCapacity)
{
    defaultAllocator = new MemAllocator(poolCapacity);
}

void MemAllocator::setDefaultAllocator(MemAllocator *allocator)
{
    defaultAllocator = allocator;
}

void MemAllocator::cleanupDefaultAllocator()
{
    delete defaultAllocator;
    defaultAllocator = NULL;
}

size_t MemAllocator::getFreeMemory()
{
    return capacity - top;
}

size_t MemAllocator::getCapacity()
{
    return capacity;
}
