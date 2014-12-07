#include "RefPtrMap.h"

RefPtrMap::RefPtrMap()
{
    std::cout << "RefPtrMap constructor" << std::endl;
}

void RefPtrMap::reg(RefPtrBase *refPtr, bool object)
{
    refs.insert(std::make_pair(refPtr, Ptr(refPtr->ptr, object)));
}

void RefPtrMap::unreg(RefPtrBase *refPtr)
{
    refs.erase(refPtr);
}

std::vector<Ptr> RefPtrMap::collectRoots()
{
    std::vector<Ptr> ptrs;

    for(auto pair : refs)
    {
        ptrs.push_back(pair.second);
    }

    return ptrs;
}

void RefPtrMap::updateRoots()
{
    for(auto pair : refs)
    {
        pair.first->ptr = *(uintptr_t *) pair.first->ptr;
    }
}
