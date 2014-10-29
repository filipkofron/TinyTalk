#include "RefPtr.h"
#include "Runtime.h"

RefPtrBase::RefPtrBase(uintptr_t ptr, bool object)
{
    if(ptr)
    {
        Runtime::refPtrMap.reg(this, ptr);
    }
}

RefPtrBase::~RefPtrBase()
{
    if(ptr)
    {
        Runtime::refPtrMap.unreg(this);
    }
}

void RefPtrBase::setBasePtr(uintptr_t ptr)
{
    if(this->ptr)
    {
        Runtime::refPtrMap.unreg(this);
    }

    this->ptr = ptr;

    if(this->ptr)
    {
        Runtime::refPtrMap.reg(this, this->ptr);
    }
}
