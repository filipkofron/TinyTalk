#include <iostream>
#include "Stack.h"

Stack::Stack()
{
    len = STACK_DEFAULT_SIZE;
    sp = len;
    data = new uint8_t[STACK_DEFAULT_SIZE];
}

Stack::~Stack()
{
    delete [] data;
}

intptr_t Stack::topPtr()
{
    intptr_t val = *((intptr_t *)(intptr_t) &data[sp]);
    return val;
}

intptr_t Stack::popPtr()
{
    intptr_t val = *((intptr_t *)(intptr_t) &data[sp]);
    sp += sizeof(intptr_t);
    std::cout << " pop: " << val << std::endl;
    return val;
}

void Stack::pushPtr(intptr_t ptr)
{
    sp -= sizeof(intptr_t);
    std::cout << " push: " << ptr << std::endl;
    *((intptr_t *)(intptr_t) &data[sp]) = ptr;
}
