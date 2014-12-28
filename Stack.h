#ifndef STACK_H
#define STACK_H

struct Stack;

#include <cstdint>
#include "MemAllocator.h"

#define STACK_DEFAULT_SIZE 1024 * 64 // Default 64kiB

struct Stack
{
    uint8_t *data;
    uint32_t sp;
    uint32_t len;

    Stack();
    ~Stack();

    uintptr_t topPtr();
    uintptr_t popPtr();
    void pushPtr(uintptr_t ptr);

    void runGC(MemAllocator *oldMem, MemAllocator *newMem);
};

#endif
