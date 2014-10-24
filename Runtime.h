#ifndef RUNTIME_H
#define RUNTIME_H

struct Runtime;

#include "BuiltinPool.h"

struct Runtime
{
    static BuiltinPool builtinPool;
};

#endif
