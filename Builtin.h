#ifndef BUILTIN_H
#define BUILTIN_H

class Builtin;

#include <vector>
#include <string>
#include "TTObject.h"

class Builtin
{
public:
    virtual TTObject *invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values) = 0;
};

#endif
