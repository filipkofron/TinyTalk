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
/*
    BC:
    assign_symbol
    - assign symbol
    stack:
        [sp]..[sp - ptr_size - 1] = object, assigned object
        [sp - ptr_size]..[sp - ptr_size * 2 - 1] = string object, name
 */

/**
* Chained expression.
*
* Consists only of next expression.
*
* 1, current expression - "currExpr"
* 2. next expression - "nextExpr"
*/
#define EXPRESSION_CHAINED 0x02
/*
    BC:
        doesn't exist
 */

/**
* Parenthesis expression flag.
*
* consists of 1 field:
*
* 1. inner expression - "innerExpr"
*/
#define EXPRESSION_PARENTHESIS 0x03
/*
    BC:
        doesn't exist
 */

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
* 5. byte code - "blockByteCode"
*
* it will later also have "blockEnv" when evaluating
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

/**
* Array expression.
*
* Consists of expression array.
*
* 1. expressions - "expressions"
*/
#define EXPRESSION_ARRAY 0x0A

namespace Expression
{
    RefPtr<TTObject> createSimpleMessage(RefPtr<TTObject> destExpr, RefPtr<TTLiteral> name);
    RefPtr<TTObject> createMultipleMessage(RefPtr<TTObject> destExpr, RefPtr<TTLiteral> fullName, RefPtr<TTLiteral> nameArray, RefPtr<TTLiteral> valueArray);
    RefPtr<TTObject> createSymbolValue(RefPtr<TTLiteral> name);
    RefPtr<TTObject> createAssignment(RefPtr<TTLiteral> name, RefPtr<TTObject> rightExpr);
    RefPtr<TTObject> createLiteralValue(RefPtr<TTLiteral> value);
    RefPtr<TTObject> createParenthesis(RefPtr<TTObject> innerExpr);
    RefPtr<TTObject> createBlock(RefPtr<TTLiteral> nameArray, RefPtr<TTLiteral> fullName, RefPtr<TTObject> expr, RefPtr<TTLiteral> nativeName);
    RefPtr<TTObject> createNaiveBlock(RefPtr<TTObject> expr);
    RefPtr<TTObject> createChained(RefPtr<TTObject> currExpr, RefPtr<TTObject> nextExpr);
    RefPtr<TTObject> createCreateVariables(RefPtr<TTLiteral> varNames);
    RefPtr<TTObject> createArray(RefPtr<TTLiteral> expressions);

    const char *getTypeInfo(TTObject *expr);
}

#endif
