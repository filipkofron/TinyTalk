#ifndef MEMALLOCATOR_H
#define MEMALLOCATOR_H

class MemAllocator;

#include <cstdint>
#include <cstdlib>
#include "TTObject.h"

// The mem allocator allocated memory from a pre-allocated pool it guarantees
// that the allocated memory segment will be initialized with zeros.
class MemAllocator
{
private:
    uint8_t *pool;
    size_t capacity;
    size_t top;
    static MemAllocator *defaultAllocator;

public:
    MemAllocator(size_t poolCapacity);
    ~MemAllocator();

    uint8_t *allocate(size_t bytes);
    uint8_t *allocateString(const uint8_t *str);
    TTObject *allocateObject();
    static MemAllocator *getCurrent();
    static void initializeDefaultAllocator(size_t poolCapacity);
    static void cleanupDefaultAllocator();
};

#endif
