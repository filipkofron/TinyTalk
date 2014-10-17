#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "TTObject.h"

/**
* Assign expression flag.
*
* consists of 2 fields:
*
* 1. symbol name - "assignSymbolName"
* 2. assigned expression for value - "assignExpression"
*/
#define EXPRESSION_FLAG_ASSIGN 0x01
#define EXPRESSION_FLAG_CHAINED 0x02
#define EXPRESSION_FLAG_PARENTHESIS 0x04

/**
* Simple message expression flag.
*
* Simple message expression object consists of
* 2 string literals.
*
* 1. destination expression - "msgExprDest"
* 2. message name - "msgName"
*/
#define EXPRESSION_FLAG_SIMPLE_MESSAGE 0x08

/**
* Multiple message expression flag.
*
* Multiple message expression object consists of
* 2 string literals and array literal.
*
* 1. destination expression - "msgExprDest"
* 2. message name - "msgFullName"
* 3. message nameArray - "msgNameArray"
* 4. message valueArray - "msgValueArray"
*/
#define EXPRESSION_FLAG_MULTIPLE_MESSAGE 0x10
#define EXPRESSION_FLAG_SYMBOL_VALUE 0x20

namespace Expression
{
    TTObject *createSimpleMessage(TTObject *destExpr, TTLiteral *name);

    TTObject *createMultipleMessage(TTObject *destExpr, TTLiteral *fullName, TTLiteral *nameArray, TTLiteral *valueArray);

    TTObject *createSymbolValue(TTLiteral *name);

    TTObject *createAssignment(TTLiteral *name, TTObject *rightExpr);

    const char *getTypeInfo(TTObject *expr);
}

#endif
