#include "BuiltinPool.h"
#include "BuiltinInteger.h"
#include "BuiltinObject.h"

BuiltinPool::BuiltinPool()
{
    std::cout << "[BuiltinPool]: initializing" << std::endl;

    registerBultin("object_alloc", std::shared_ptr<Builtin> (new BuiltinObjectAlloc()));
    registerBultin("object_add", std::shared_ptr<Builtin> (new BuiltinObjectAddField));
    registerBultin("object_get", std::shared_ptr<Builtin> (new BuiltinObjectGetter()));
    registerBultin("object_set", std::shared_ptr<Builtin> (new BuiltinObjectSetter()));
    registerBultin("object_debugprint", std::shared_ptr<Builtin> (new BuiltinObjectDebugPrint));
    registerBultin("object_debugprintrec", std::shared_ptr<Builtin> (new BuiltinObjectDebugPrintRec));
    registerBultin("object_clone", std::shared_ptr<Builtin> (new BuiltinObjectClone));
    registerBultin("object_new", std::shared_ptr<Builtin> (new BuiltinObjectNew));
    registerBultin("object_tostring", std::shared_ptr<Builtin> (new BuiltinObjectToString));

    registerBultin("integer_add", std::shared_ptr<Builtin> (new BuiltinIntegerAdd));
    registerBultin("integer_minus", std::shared_ptr<Builtin> (new BuiltinIntegerMinus));
    registerBultin("integer_mul", std::shared_ptr<Builtin> (new BuiltinIntegerMul));
    registerBultin("integer_div", std::shared_ptr<Builtin> (new BuiltinIntegerDiv));
    registerBultin("integer_tostring", std::shared_ptr<Builtin> (new BuiltinIntegerToString));
    registerBultin("integer_lessthan", std::shared_ptr<Builtin> (new BuiltinIntegerLessThan));
    registerBultin("integer_greaterthan", std::shared_ptr<Builtin> (new BuiltinIntegerGreaterThan));
    registerBultin("integer_equals", std::shared_ptr<Builtin> (new BuiltinIntegerEquals));
    registerBultin("integer_lessthanorequal", std::shared_ptr<Builtin> (new BuiltinIntegerLessThanOrEqual));
    registerBultin("integer_greaterthanorequal", std::shared_ptr<Builtin> (new BuiltinIntegerGreaterThanOrEqual));
    registerBultin("integer_fromstring", std::shared_ptr<Builtin> (new BuiltinIntegerFromString));
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
