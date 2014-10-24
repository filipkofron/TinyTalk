#include "BuiltinPool.h"
#include "BuiltinInteger.h"
#include "BuiltinObject.h"

BuiltinPool::BuiltinPool()
{
    std::cout << "[BuiltinPool]: initializing" << std::endl;

    registerBultin("obj_alloc", std::shared_ptr<BuiltinObjectAlloc> (new BuiltinObjectAlloc()));

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
