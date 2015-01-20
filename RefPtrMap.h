#ifndef REFPTRMAP_H
#define REFPTRMAP_H

class RefPtrMap;

#include <cstdint>
#include <vector>
#include "Ptr.h"
#include "RefPtr.h"
#include "SpinLock.h"
#include <cstdio>

#define HASHMAP_FAST

#ifndef HASHMAP_FAST
#include <set>
#endif

class RefPtrMap
{
private:
#ifndef HASHMAP_FAST
    std::set<RefPtrBase *> refs;
#endif

#ifdef HASHMAP_FAST
    RefPtrBase **vals;
#endif

    SpinLock lock;
    void reg_threadunsafe(RefPtrBase *refPtr);
public:
    RefPtrMap();
    ~RefPtrMap();

    void reg(RefPtrBase *refPtr);
    void unreg(RefPtrBase *refPtr);

    std::vector<RefPtrBase *> collectRoots();
    void updateRoots();
};

#endif
