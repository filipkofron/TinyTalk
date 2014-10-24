#ifndef BUILTININTEGER_H
#define BUILTININTEGER_H

class BuiltinInteger;

#include "Builtin.h"

class BuiltinIntegerAdd : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinIntegerMinus : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinIntegerMul : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinIntegerDiv : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinIntegerToString : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

#endif
