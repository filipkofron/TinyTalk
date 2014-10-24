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
#define EXPRESSION_ASSIGN 0x01

/**
* Chained expression.
*
* Consists only of next expression.
*
* 1, current expression - "currExpr"
* 2. next expression - "nextExpr"
*/
#define EXPRESSION_CHAINED 0x02

/**
* Parenthesis expression flag.
*
* consists of 1 field:
*
* 1. inner expression - "innerExpr"
*/
#define EXPRESSION_PARENTHESIS 0x03

/**
* Simple message expression flag.
*
* Simple message expression object consists of
* 2 string literals.
*
* 1. destination expression - "msgDestExpr"
* 2. message name - "msgName"
*/
#define EXPRESSION_SIMPLE_MESSAGE 0x04

/**
* Multiple message expression flag.
*
* Multiple message expression object consists of
* 2 string literals and array literal.
*
* 1. destination expression - "msgDestExpr"
* 2. message name - "msgFullName"
* 3. message nameArray - "msgNameArray"
* 4. message valueArray - "msgValueArray"
*/
#define EXPRESSION_MULTIPLE_MESSAGE 0x05

/**
* Symbol value expression flag.
*
* Holds the name of the symbol in one field.
*
* 1. symbol name - "symbolName"
*/
#define EXPRESSION_SYMBOL_VALUE 0x06

/**
* Literal value expression flag.
*
* It stores a literal value in one field.
*
* 1. literal value - "literalValue"
*/
#define EXPRESSION_LITERAL_VALUE 0x07

/**
* Block expression flag.
*
* It consists of block arguments, fullname and its expression.
*
* 1. arguments  - "blockArgNames"
* 2. full name - "blockFullName"
* 3. expression - "blockExpr"
* 4. native name - "blockNativeName"
*/
#define EXPRESSION_BLOCK 0x08

/**
* Create variable expression.
*
* Consists of variable name array.
*
* 1. variable names - "varNames"
*/
#define EXPRESSION_CREATE_VARIABLES 0x09

namespace Expression
{
    TTObject *createSimpleMessage(TTObject *destExpr, TTLiteral *name);
    TTObject *createMultipleMessage(TTObject *destExpr, TTLiteral *fullName, TTLiteral *nameArray, TTLiteral *valueArray);
    TTObject *createSymbolValue(TTLiteral *name);
    TTObject *createAssignment(TTLiteral *name, TTObject *rightExpr);
    TTObject *createLiteralValue(TTLiteral *value);
    TTObject *createParenthesis(TTObject *innerExpr);
    TTObject *createBlock(TTLiteral *nameArray, TTLiteral *fullName, TTObject *expr, TTLiteral *nativeName);
    TTObject *createChained(TTObject *currExpr, TTObject *nextExpr);
    TTObject *createCreateVariables(TTLiteral *varNames);

    const char *getTypeInfo(TTObject *expr);
}

#endif
