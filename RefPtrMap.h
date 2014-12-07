#ifndef REFPTRMAP_H
#define REFPTRMAP_H

class RefPtrMap;

#include <cstdint>
#include <map>
#include <vector>
#include "Ptr.h"
#include "RefPtr.h"

class RefPtrMap
{
private:
    std::map<RefPtrBase *, Ptr> refs;
public:
    RefPtrMap();

    void reg(RefPtrBase *refPtr, bool object);
    void unreg(RefPtrBase *refPtr);

    std::vector<Ptr> collectRoots();
    void updateRoots();
};

#endif
