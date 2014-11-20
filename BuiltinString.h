#ifndef BUITLINSTRING_H
#define BUITLINSTRING_H

#include "Builtin.h"

class BuiltinStringCharAt : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinStringSetCharAt : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinStringToLower : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinStringToUpper : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinStringTrim : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinStringAppend : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinStringLength : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

class BuiltinStringToString : public Builtin
{
private:
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values);
};

#endif
