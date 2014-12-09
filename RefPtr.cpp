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
    if (ptr)
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
/*
void RefPtrBase::_debug_check()
{
    if (ptr)
    {
        if (object)
        {
            TTObject *obj = (TTObject *) ptr;
            if (MemAllocator::getCurrent()->objects.find((uintptr_t) obj) == MemAllocator::getCurrent()->objects.end())
            {
                std::cout << "Invalid object ptr." << (unsigned long) ptr << std::endl;
                (*(int *) NULL) = 1;
            }
            std::cout << "OBJECT: " << (unsigned long) ptr << std::endl;
            //obj->print(std::cout, 2, false);
        }
        else
        {
            TTLiteral *obj = (TTLiteral *) ptr;
            if (MemAllocator::getCurrent()->literals.find((uintptr_t) obj) == MemAllocator::getCurrent()->literals.end())
            {
                std::cout << "Invalid literal ptr: " << (unsigned long long) ptr << std::endl;
                (*(int *) NULL) = 1;
            }
            std::cout << "LITERAL: " << (unsigned long) ptr << std::endl;
            //obj->printValue(std::cout, 2, false);
        }
    }
}*/

void RefPtrBase::setBasePtr(uintptr_t ptr, bool object)
{
    if(this->ptr)
    {
        Runtime::refPtrMap.unreg(this);
    }

    this->ptr = ptr;
    this->object = object;

    if(this->ptr)
    {
        Runtime::refPtrMap.reg(this, object);
    }
}
