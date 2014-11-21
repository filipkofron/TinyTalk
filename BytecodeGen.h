#ifndef BYTECODEGEN_H
#define BYTECODEGEN_H

class BytecodeGen;

#include "TTObject.h"
#include "Expression.h"

class BytecodeGen
{
public:
    TTObject *generate(TTObject *expression);
};

#endif
