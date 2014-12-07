#include "RefPtr.h"
#include "Runtime.h"

RefPtrBase::RefPtrBase(uintptr_t ptr, bool object)
    : ptr(ptr), object(object)
{
    if(ptr)
    {
        Runtime::refPtrMap.reg(this, object);
    }
}

RefPtrBase::RefPtrBase(const RefPtrBase &orig)
    : ptr(orig.ptr), object(orig.object)
{
    if(ptr)
    {
        Runtime::refPtrMap.reg(this, object);
    }
}

RefPtrBase::~RefPtrBase()
{
    if(ptr)
    {
        Runtime::refPtrMap.unreg(this);
    }
}

RefPtrBase &RefPtrBase::operator =(const RefPtrBase& orig)
{
    if(&orig != this)
    {
        if (ptr)
        {
            Runtime::refPtrMap.unreg(this);
        }
        ptr = orig.ptr;
        object = orig.object;
        if (ptr)
        {
            Runtime::refPtrMap.reg(this, object);
        }
    }

    return *this;
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
