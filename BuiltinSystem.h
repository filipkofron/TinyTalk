#ifndef BUILTINSYSTEM_H
#define BUILTINSYSTEM_H

#include "Builtin.h"

class BuiltinSystemRunFile : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values);
};

#endif
