#ifndef PTR_H
#define PTR_H

#include <cstdint>

struct Ptr
{
    uintptr_t ptr;
    bool object;

    Ptr(uintptr_t ptr, bool object);
};

#endif
