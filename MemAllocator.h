#include <cstdint>
#include <cstdlib>

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
    static MemAllocator *getDefault();
    static void initializeDefaultAllocator(size_t poolCapacity);
    static void cleanupDefaultAllocator();
};
