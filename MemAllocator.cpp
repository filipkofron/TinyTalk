#include "Common.h"
#include "MemAllocator.h"
#include "Runtime.h"
#include <cstring>
#include <iostream>

MemAllocator *MemAllocator::defaultAllocator = NULL;
MemAllocator *MemAllocator::nextAllocator = NULL;

MemAllocator::MemAllocator(size_t poolCapacity)
        : capacity(poolCapacity), top(0)
{
    pool = (uint8_t *) malloc(poolCapacity);
    if(!pool)
    {
        std::cerr << "MemAlloc: Fatal error: Out of host memory." << std::endl;
        exit(1);
    }
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

bool MemAllocator::isInside(uintptr_t ptr)
{
#ifdef DEBUG
    std::cout << "pool[" << (unsigned long) (uintptr_t ) pool << "] <= ptr[" << (unsigned long) ptr << "] < " << (unsigned long) (uintptr_t) &pool[capacity] << std::endl;
#endif
    return (uintptr_t ) pool <= ptr && ptr < (uintptr_t) &pool[capacity];
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

    if ((top + bytes) < (capacity - 1024))
    {
        top += bytes;
        memset(nextAddr, 0, bytes);
        return nextAddr;
    }

#ifdef DEBUG
    std::cout << "Out of memory, this is " << (this == defaultAllocator ? "" : "not ") << "default allocator" << std::endl;
#endif

    Runtime::runCopyGC();

    size_t curr = getCurrent()->getFreeMemory();
    if(curr < (Runtime::allocSize / 8))
    {
        Runtime::allocSize = (size_t) (Runtime::allocSize * 1.5);
        Runtime::runCopyGC();
    }

    if(getCurrent()->getFreeMemory() < bytes)
    {
        /*std::cerr << "Runtime error: Could not free more memory." << std::endl;
        throw std::exception();*/
        Runtime::allocSize = (size_t) (Runtime::allocSize * 1.5);
        Runtime::runCopyGC();
    }

    return getCurrent()->allocate(bytes);
}

void MemAllocator::ensure(size_t bytes)
{
/*uint8_t *hack = (uint8_t *)  malloc(bytes);
    memset(hack, 0, bytes);
    return hack;*/

#ifdef DEBUG
    std::cout << "MemAllocator::allocate(): called to allocate " << bytes << " bytes, remaining: " << (capacity - (top + bytes)) << std::endl;
#endif

    if ((top + bytes) < (capacity - 1024))
    {
        return;
    }

#ifdef DEBUG
    std::cout << "Out of memory, this is " << (this == defaultAllocator ? "" : "not ") << "default allocator" << std::endl;
#endif

    Runtime::runCopyGC();

    size_t curr = getCurrent()->getFreeMemory();
    if(curr < (Runtime::allocSize / 8))
    {
        Runtime::allocSize = (size_t) (Runtime::allocSize * 1.5);
        Runtime::runCopyGC();
    }

    if(getCurrent()->getFreeMemory() < bytes)
    {
        /*std::cerr << "Runtime error: Could not free more memory." << std::endl;
        throw std::exception();*/
        Runtime::allocSize = (size_t) (Runtime::allocSize * 1.5);
        Runtime::runCopyGC();
    }
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
    nextAllocator->objects.insert((uintptr_t) obj);
    return obj;
#else
    return (TTObject *) allocate(sizeof(TTObject));
#endif
}

TTLiteral* MemAllocator::allocateLiteral()
{
#ifdef DEBUG

    TTLiteral *lit = (TTLiteral *) allocate(sizeof(TTLiteral));
    nextAllocator->literals.insert((uintptr_t) lit);
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
    nextAllocator = defaultAllocator;
}

void MemAllocator::setDefaultAllocator(MemAllocator *allocator)
{
    defaultAllocator = allocator;
}

void MemAllocator::setNextAllocator(MemAllocator *allocator)
{
    nextAllocator = allocator;
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
