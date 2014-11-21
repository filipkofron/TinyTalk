#ifndef STACK_H
#define STACK_H

struct Stack;

#include <cstdint>

#define STACK_DEFAULT_SIZE 64 * 1024

struct Stack
{
    uint8_t *data;
    uint32_t sp;

    Stack();
    ~Stack();

    int32_t topInt();
    int32_t popInt();
    intptr_t topPtr();
    intptr_t popPtr();
    void pushInt(int32_t val);
    void pushPtr(intptr_t ptr);
};

#endif
