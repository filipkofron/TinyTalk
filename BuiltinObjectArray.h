#ifndef BUILTINOBJECTARRAY_H
#define BUILTINOBJECTARRAY_H

#include "Builtin.h"

class BuiltinObjectArraySize : public Builtin
{
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinObjectArrayNewWithSize : public Builtin
{
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinObjectArrayAt : public Builtin
{
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinObjectArrayAtSet : public Builtin
{
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinObjectArrayEmpty : public Builtin
{
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinObjectArrayToString : public Builtin
{
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

#endif
