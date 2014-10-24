#ifndef BUILTINOBJECT_H
#define BUILTINOBJECT_H

class BuiltinObject;

#include "Builtin.h"

class BuiltinObjectAlloc : public Builtin
{
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinObjectAddField : public Builtin
{
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinObjectGetter : public Builtin
{
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinObjectSetter : public Builtin
{
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};


#endif
