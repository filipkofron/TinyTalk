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

/**
* Parenthesis expression flag.
*
* consists of 1 field:
*
* 1. inner expression - "innerExpr"
*/
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

/**
* Symbol value expression flag.
*
* Holds the name of the symbol in one field.
*
* 1. symbol name - "symbolName"
*/
#define EXPRESSION_FLAG_SYMBOL_VALUE 0x20

/**
* Literal value expression flag.
*
* It stores a literal value in one field.
*
* 1. literal value - "literalValue"
*/
#define EXPRESSION_FLAG_LITERAL_VALUE 0x40

namespace Expression
{
    TTObject *createSimpleMessage(TTObject *destExpr, TTLiteral *name);

    TTObject *createMultipleMessage(TTObject *destExpr, TTLiteral *fullName, TTLiteral *nameArray, TTLiteral *valueArray);

    TTObject *createSymbolValue(TTLiteral *name);

    TTObject *createAssignment(TTLiteral *name, TTObject *rightExpr);

    TTObject *createLiteralValue(TTLiteral *value);

    TTObject *createParenthesis(TTObject *innerExpr);

    const char *getTypeInfo(TTObject *expr);
}

#endif
