#ifndef STACK_H
#define STACK_H

struct Stack;

#include <cstdint>

#define STACK_DEFAULT_SIZE 64 * 1024

struct Stack
{
    uint8_t *data;
    uint32_t sp;
    uint32_t len;

    Stack();
    ~Stack();

    intptr_t topPtr();
    intptr_t popPtr();
    void pushPtr(intptr_t ptr);
};

#endif
