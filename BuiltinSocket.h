#ifndef BUILTINSOCKET_H
#define BUILTINSOCKET_H

#include "Builtin.h"

class BuiltinSocketCreateTCPServerSocketAddressPortFile : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinSocketCloseTCPServerSocketFile : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinSocketTCPServerSocketAcceptClientFile : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

class BuiltinSocketCloseTCPClientSocketFile : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

#endif
