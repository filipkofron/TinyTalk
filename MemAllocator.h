#include <cstdint>

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
    static MemAllocator *getDefault();
    static void initializeDefaultAllocator(size_t poolCapacity);
    static void cleanupDefaultAllocator();
};
