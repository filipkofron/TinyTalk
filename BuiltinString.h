#ifndef BUITLINSTRING_H
#define BUITLINSTRING_H

#include "Builtin.h"

class BuiltinStringCharAt : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringCharAsIntAt : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringSetCharAt : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringToLower : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringToUpper : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringTrim : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringAppend : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringLength : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringToString : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringStartsWith : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringContains : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringSplitBy : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinStringEquals : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

#endif
