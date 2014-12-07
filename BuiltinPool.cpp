#include "BuiltinPool.h"
#include "BuiltinInteger.h"
#include "BuiltinObject.h"
#include "BuiltinString.h"
#include "BuiltinObjectArray.h"

BuiltinPool::BuiltinPool()
{
    std::cout << "[BuiltinPool]: initializing" << std::endl;

    registerBultin("object_alloc", std::shared_ptr<Builtin> (new BuiltinObjectAlloc()));
    registerBultin("object_add", std::shared_ptr<Builtin> (new BuiltinObjectAddField));
    registerBultin("object_get", std::shared_ptr<Builtin> (new BuiltinObjectGetter()));
    registerBultin("object_set", std::shared_ptr<Builtin> (new BuiltinObjectSetter()));
    registerBultin("object_debugprint", std::shared_ptr<Builtin> (new BuiltinObjectDebugPrint));
    registerBultin("object_debugprintrec", std::shared_ptr<Builtin> (new BuiltinObjectDebugPrintRec));
    registerBultin("object_debugprintstring", std::shared_ptr<Builtin> (new BuiltinObjectDebugPrintString));
    registerBultin("object_clone", std::shared_ptr<Builtin> (new BuiltinObjectClone));
    registerBultin("object_new", std::shared_ptr<Builtin> (new BuiltinObjectNew));
    registerBultin("object_tostring", std::shared_ptr<Builtin> (new BuiltinObjectToString));

    registerBultin("integer_add:", std::shared_ptr<Builtin> (new BuiltinIntegerAdd));
    registerBultin("integer_minus:", std::shared_ptr<Builtin> (new BuiltinIntegerMinus));
    registerBultin("integer_mul:", std::shared_ptr<Builtin> (new BuiltinIntegerMul));
    registerBultin("integer_div:", std::shared_ptr<Builtin> (new BuiltinIntegerDiv));
    registerBultin("integer_mod:", std::shared_ptr<Builtin> (new BuiltinIntegerMod));
    registerBultin("integer_toString", std::shared_ptr<Builtin> (new BuiltinIntegerToString));
    registerBultin("integer_lessThan:", std::shared_ptr<Builtin> (new BuiltinIntegerLessThan));
    registerBultin("integer_greaterThan:", std::shared_ptr<Builtin> (new BuiltinIntegerGreaterThan));
    registerBultin("integer_equals:", std::shared_ptr<Builtin> (new BuiltinIntegerEquals));
    registerBultin("integer_lessThanOrEqual:", std::shared_ptr<Builtin> (new BuiltinIntegerLessThanOrEqual));
    registerBultin("integer_greaterThanOrEqual", std::shared_ptr<Builtin> (new BuiltinIntegerGreaterThanOrEqual));
    registerBultin("integer_fromString:", std::shared_ptr<Builtin> (new BuiltinIntegerFromString));

    registerBultin("string_charAt:", std::shared_ptr<Builtin> (new BuiltinStringCharAt));
    registerBultin("string_setChar:at:", std::shared_ptr<Builtin> (new BuiltinStringSetCharAt));
    registerBultin("string_toLower", std::shared_ptr<Builtin> (new BuiltinStringToLower));
    registerBultin("string_toUpper", std::shared_ptr<Builtin> (new BuiltinStringToUpper));
    registerBultin("string_trim", std::shared_ptr<Builtin> (new BuiltinStringTrim));
    registerBultin("string_append:", std::shared_ptr<Builtin> (new BuiltinStringAppend));
    registerBultin("string_length", std::shared_ptr<Builtin> (new BuiltinStringLength));
    registerBultin("string_toString", std::shared_ptr<Builtin> (new BuiltinStringToString));

    registerBultin("array_size", std::shared_ptr<Builtin> (new BuiltinObjectArraySize));
    registerBultin("array_at:", std::shared_ptr<Builtin> (new BuiltinObjectArrayAt));
    registerBultin("array_at:set:", std::shared_ptr<Builtin> (new BuiltinObjectArrayAtSet));
    registerBultin("array_empty", std::shared_ptr<Builtin> (new BuiltinObjectArrayEmpty));
    registerBultin("array_toString", std::shared_ptr<Builtin> (new BuiltinObjectArrayToString));

    std::cout << "[BuiltinPool]: initialized." << std::endl;
}

void BuiltinPool::registerBultin(const std::string &name, std::shared_ptr<Builtin> builtin)
{
    builtins.insert(std::pair<std::string, std::shared_ptr<Builtin> >(name, builtin));
}

std::shared_ptr<Builtin> BuiltinPool::lookupBultin(const std::string &name)
{
    auto it = builtins.find(name);
    if(it != builtins.end())
    {
        return it->second;
    }
    return std::shared_ptr<Builtin> (NULL);
}
