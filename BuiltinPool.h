#ifndef BUILTINPOOL_H
#define BUILTINPOOL_H

class BuiltinPool;

#include <vector>
#include <string>
#include <memory>
#include "Builtin.h"

class BuiltinPool
{
private:
    std::map<std::string, std::shared_ptr<Builtin> > builtins;
public:
    BuiltinPool();

    void registerBultin(const std::string &name, std::shared_ptr<Builtin> builtin);
    std::shared_ptr<Builtin> lookupBultin(const std::string &name);
};

#endif
