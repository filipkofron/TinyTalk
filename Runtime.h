#ifndef RUNTIME_H
#define RUNTIME_H

struct Runtime;

#include "BuiltinPool.h"
#include "RefPtrMap.h"

struct Runtime
{
    static BuiltinPool builtinPool;
    static RefPtrMap refPtrMap;
    static TTObject *globalEnvironment;
};

#endif
