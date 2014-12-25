#ifndef BUILTINSOCKET_H
#define BUILTINSOCKET_H

#include "Builtin.h"

class BuiltinSocketCreateTCPServerSocketAddressPortFile : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values);
};

class BuiltinSocketCloseTCPServerSocketFile : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values);
};

class BuiltinSocketTCPServerSocketAcceptClientFile : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values);
};

class BuiltinSocketCloseTCPClientSocketFile : public Builtin
{
private:
public:
    virtual RefPtr<TTObject> invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values);
};

#endif
