#ifndef BUILTINFILE_HPP
#define BUILTINFILE_HPP

class BuiltinFile;

#include "Builtin.h"

class BuiltinFileOpenPath : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values);
};

class BuiltinFileClose : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values);
};

class BuiltinFileIsEOF : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values);
};

class BuiltinFileReadChar : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values);
};

class BuiltinFileWriteChar : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values);
};

#endif
