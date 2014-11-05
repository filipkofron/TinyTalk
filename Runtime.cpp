#include "Runtime.h"

BuiltinPool Runtime::builtinPool;
RefPtrMap Runtime::refPtrMap;
TTObject *Runtime::globalEnvironment = NULL;
