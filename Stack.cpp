#include "Stack.h"

Stack::Stack()
{
    data = new uint8_t[STACK_DEFAULT_SIZE];
}

Stack::~Stack()
{
    delete [] data;
}

int32_t Stack::topInt()
{
    int32_t val = *((int32_t *)(intptr_t) data[sp]);
    return val;
}

int32_t Stack::popInt()
{
    int32_t val = *((int32_t *)(intptr_t) data[sp]);
    sp += sizeof(int32_t);
    return val;
}

intptr_t Stack::topPtr()
{
    intptr_t val = *((intptr_t *)(intptr_t) data[sp]);
    return val;
}

intptr_t Stack::popPtr()
{
    intptr_t val = *((intptr_t *)(intptr_t) data[sp]);
    sp += sizeof(intptr_t);
    return val;
}

void Stack::pushInt(int32_t val)
{
    sp -= sizeof(int32_t);
    *((int32_t *)(intptr_t) data[sp]) = val;
}

void Stack::pushPtr(intptr_t ptr)
{
    sp -= sizeof(intptr_t);
    *((intptr_t *)(intptr_t) data[sp]) = ptr;
}
