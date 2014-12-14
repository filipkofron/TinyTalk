#ifndef MEMALLOCATOR_H
#define MEMALLOCATOR_H

class MemAllocator;

#include <cstdint>
#include <cstdlib>
#include <set>
#include "common.h"
#include "TTObject.h"
#include "TTLiteral.h"

// The mem allocator allocated memory from a pre-allocated pool it guarantees
// that the allocated memory segment will be initialized with zeros.
class MemAllocator
{
public:
#ifdef DEBUG
    std::set<uintptr_t> objects;
    std::set<uintptr_t> literals;
#endif
private:
    uint8_t *pool;
    size_t capacity;
    size_t top;
    static MemAllocator *defaultAllocator;
    static MemAllocator *nextAllocator;

    uint8_t *allocateString(const uint8_t *str);

public:
    MemAllocator(size_t poolCapacity);
    ~MemAllocator();

    bool isInside(uintptr_t ptr);

    uint8_t *allocate(size_t bytes);
    uint8_t *cloneString(const uint8_t *str);

    TTObject *allocateObject();
    TTLiteral *allocateLiteral();
    static MemAllocator *getCurrent();
    static void initializeDefaultAllocator(size_t poolCapacity);
    static void setDefaultAllocator(MemAllocator *allocator);
    static void setNextAllocator(MemAllocator *allocator);
    static void cleanupDefaultAllocator();

    size_t getFreeMemory();
    size_t getCapacity();
};

#endif
