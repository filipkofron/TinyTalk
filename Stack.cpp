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

uintptr_t Stack::topPtr()
{
    uintptr_t val = *((uintptr_t *)(uintptr_t) &data[sp]);
    return val;
}

uintptr_t Stack::popPtr()
{
    if(sp > (len - sizeof(uintptr_t)))
    {
        std::cerr << "STACK UNDERFLOW" << std::endl;
        throw std::exception();
    }
    uintptr_t val = *((uintptr_t *)(uintptr_t) &data[sp]);
    sp += sizeof(uintptr_t);
//    std::cout << " pop: " << val << std::endl;
    return val;
}

void Stack::pushPtr(uintptr_t ptr)
{
    if(sp < sizeof(uintptr_t))
    {
        std::cerr << "STACK OVERFLOW" << std::endl;
        throw std::exception();
    }
    sp -= sizeof(uintptr_t);

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
    *((uintptr_t *)(uintptr_t) &data[sp]) = ptr;
}

void Stack::runGC(MemAllocator *oldMem, MemAllocator *newMem)
{
    std::cout << "Stack::runGC ==== BEGIN ====" << std::endl;
    for(uint32_t mySp = sp; mySp < len; mySp += sizeof(uintptr_t))
    {
#ifdef DEBUG
        std::cout << "GC: Stack: sp/len: " << mySp << "/" << len << std::endl;
#endif
        TTObject::_gc_COPY_copy((TTObject **) &data[mySp], oldMem, newMem);
    }
    std::cout << "Stack::runGC ==== END ====" << std::endl;
}
