#include <iostream>
#include "Stack.h"

Stack::Stack()
{
    len = STACK_DEFAULT_SIZE;
    sp = 0;
    data = (uint8_t *) malloc(len);
}

Stack::~Stack()
{
    free(data);
}

uintptr_t Stack::topPtr()
{
    uintptr_t val = *((uintptr_t *)(uintptr_t) &data[sp - sizeof(uintptr_t)]);
    return val;
}

uintptr_t Stack::popPtr()
{
    if(sp <= 0)
    {
        std::cerr << "STACK UNDERFLOW" << std::endl;
        KILL;
    }
    sp -= sizeof(uintptr_t);
    uintptr_t val = *((uintptr_t *)(uintptr_t) &data[sp]);
//    std::cout << " pop: " << val << std::endl;
    return val;
}

void Stack::pushPtr(uintptr_t ptr)
{
    if(sp >= (len - sizeof(uintptr_t)))
    {
        len *= 2;
        data = (uint8_t *) realloc(data, len);
        if(!data)
        {
            std::cerr << "STACK OVERFLOW" << std::endl;
            KILL;
        }
    }
    *((uintptr_t *)(uintptr_t) &data[sp]) = ptr;
    sp += sizeof(uintptr_t);

#ifdef DEBUG
    TTObject *obj = (TTObject *) ptr;
    if (MemAllocator::getCurrent()->objects.find((uintptr_t) obj) == MemAllocator::getCurrent()->objects.end())
    {
        std::cout << "Invalid object ptr." << (unsigned long) ptr << std::endl;
        KILL;
    }
    std::cout << "OBJECT: " << (unsigned long) ptr << std::endl;
#endif

//    std::cout << " push: " << ptr << std::endl;
}

void Stack::runGC(MemAllocator *oldMem, MemAllocator *newMem)
{
#ifdef DEBUG
    std::cout << "Stack::runGC ==== BEGIN ====" << std::endl;
#endif
    for(uint32_t mySp = 0; mySp < sp; mySp += sizeof(uintptr_t))
    {
#ifdef DEBUG
        std::cout << "GC: Stack: sp/len: " << mySp << "/" << len << std::endl;
#endif
        TTObject::_gc_COPY_copy((TTObject **) &data[mySp], oldMem, newMem);
    }
#ifdef DEBUG
    std::cout << "Stack::runGC ==== END ====" << std::endl;
#endif
}
