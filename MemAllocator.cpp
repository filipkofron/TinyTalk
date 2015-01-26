#include "Common.h"
#include "MemAllocator.h"
#include "Runtime.h"
#include <cstring>
#include <iostream>

MemAllocator *MemAllocator::defaultAllocator = NULL;
MemAllocator *MemAllocator::nextAllocator = NULL;

// #define DEBUG_MALLOC_HAX

MemAllocator::MemAllocator(size_t poolCapacity)
        : capacity(poolCapacity), top(0)
{
    pool = (uint8_t *) malloc(poolCapacity);
    if(!pool)
    {
        std::cerr << "MemAlloc: Fatal error: Out of host memory." << std::endl;
        KILL;
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

#ifdef DEBUG_MALLOC_HAX
SpinLock HAXOR;
#endif

uint8_t *MemAllocator::allocate(size_t bytes)
{
#ifdef DEBUG_MALLOC_HAX
    HAXOR.lock();
    uint8_t *resss = (uint8_t *) calloc(1, bytes);
    HAXOR.unlock();
    return resss;
#endif

    bool runAgain = false;
    Runtime::gcBarrier.freeHelp = false;
    Runtime::gcBarrier.enteringAlloc(runAgain);
    if(runAgain)
    {
        Runtime::gcBarrier.leavingAlloc();
        Runtime::gcBarrier.freeHelp = true;
        return getCurrent()->allocate(bytes);
    }

    uint8_t *nextAddr = pool + top;

#ifdef DEBUG
    std::cout << "MemAllocator::allocate(): called to allocate " << bytes << " bytes, remaining: " << (capacity - (top + bytes)) << std::endl;
#endif

    if(this == getCurrent())
    {
        if ((top + bytes) < (capacity - 1024))
        {
            top += bytes;
            Runtime::gcBarrier.leavingAlloc();
            Runtime::gcBarrier.freeHelp = true;
            memset(nextAddr, 0, bytes);
            return nextAddr;
        }
    }
    else
    {
        if ((top + bytes) < capacity)
        {
            top += bytes;
            Runtime::gcBarrier.leavingAlloc();
            Runtime::gcBarrier.freeHelp = true;
            memset(nextAddr, 0, bytes);
            return nextAddr;
        }
    }

    if(bytes > (capacity - capacity / 2))
    {
        std::cout << "Runtime: increasing memory from: " << Runtime::allocSize / 1024 << " to ";
        Runtime::allocSize += bytes;
        std::cout << Runtime::allocSize / 1024 << " kiB" << std::endl;
    }

    // TODO: Create Barrier that will stop at all pre blocking/allocating

    if((currentTimeMilis() - Runtime::lastGCRun) < 150)
    {
        std::cout << "Runtime: increasing memory from: " << Runtime::allocSize / 1024 << " to ";
        Runtime::allocSize = (size_t) (1.5 * Runtime::allocSize);
        std::cout << Runtime::allocSize / 1024 << " kiB" << std::endl;
    }
    Runtime::lastGCRun = currentTimeMilis();
    Runtime::gcBarrier.enteringGC();


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
        Runtime::allocSize = (size_t) (Runtime::allocSize * 1.5);
        Runtime::runCopyGC();
    }

    Runtime::gcBarrier.leavingGC();
    Runtime::gcBarrier.leavingAlloc();
    Runtime::gcBarrier.freeHelp = true;

    uint8_t *res = getCurrent()->allocate(bytes);

    return res;
}

uint8_t *MemAllocator::allocateSureAndThreadUnsafe(size_t bytes)
{
#ifdef DEBUG_MALLOC_HAX
    HAXOR.lock();
    uint8_t *ress = (uint8_t *) calloc(1, bytes);
    HAXOR.unlock();
    return ress;
#endif

    /*uint8_t *hack = (uint8_t *)  malloc(bytes);
    memset(hack, 0, bytes);
    return hack;*/
    uint8_t *nextAddr = pool + top;

#ifdef DEBUG
    std::cout << "MemAllocator::allocate(): called to allocate " << bytes << " bytes, remaining: " << (capacity - (top + bytes)) << std::endl;
#endif
    top += bytes;
    memset(nextAddr, 0, bytes);

    return nextAddr;
}

void MemAllocator::ensureWithLock(size_t bytes)
{
#ifdef DEBUG_MALLOC_HAX
    return;
#endif

    bool runAgain = false;
    Runtime::gcBarrier.enteringAlloc(runAgain);
    Runtime::gcBarrier.freeHelp = false;
    if(runAgain)
    {
        Runtime::gcBarrier.leavingAlloc();
        Runtime::gcBarrier.freeHelp = true;
        getCurrent()->ensureWithLock(bytes);
        return;
    }

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

    if(bytes > (capacity - capacity / 2))
    {
        std::cout << "Runtime: increasing memory from: " << Runtime::allocSize / 1024 << " to ";
        Runtime::allocSize += bytes;
        std::cout << Runtime::allocSize / 1024 << " kiB" << std::endl;
    }

    if((currentTimeMilis() - Runtime::lastGCRun) < 150)
    {
        std::cout << "Runtime: increasing memory from: " << Runtime::allocSize / 1024 << " to ";
        Runtime::allocSize = (size_t) (1.5 * Runtime::allocSize);
        std::cout << Runtime::allocSize / 1024 << " kiB" << std::endl;
    }
    Runtime::lastGCRun = currentTimeMilis();

    Runtime::gcBarrier.enteringGC();

    Runtime::runCopyGC();

    std::cout << "after" << std::endl;

    size_t curr = getCurrent()->getFreeMemory();
    if(curr < (Runtime::allocSize / 8))
    {
        Runtime::allocSize = (size_t) (Runtime::allocSize * 1.5);
        Runtime::runCopyGC();
    }

    if(getCurrent()->getFreeMemory() < bytes)
    {
        Runtime::allocSize = (size_t) (Runtime::allocSize * 1.5);
        Runtime::runCopyGC();
    }

    Runtime::gcBarrier.leavingGC();
}

void MemAllocator::ensureWithUnlock()
{
    Runtime::gcBarrier.leavingAlloc();
    Runtime::gcBarrier.freeHelp = true;
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

TTObject *MemAllocator::allocateObject_threadunsafe()
{
#ifdef DEBUG
    TTObject *obj = (TTObject *) allocate(sizeof(TTObject));
    nextAllocator->objects.insert((uintptr_t) obj);
    return obj;
#else
    return (TTObject *) allocateSureAndThreadUnsafe(sizeof(TTObject));
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
