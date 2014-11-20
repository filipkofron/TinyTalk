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
/*
    BC:
    send_simple
    - send simple message
    stack:
        [sp]..[sp - ptr_size - 1] = dest object
        [sp - ptr_size]..[sp - ptr_size * 2 - 1] = message name object
 */

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
/*
    BC:
    send_multiple
    - send multiple message
    begin stack:
        [sp]..[sp - ptr_size - 1] = dest object
        [sp - ptr_size]..[sp - ptr_size * 2 - 1] = message name object
        [sp - ptr_size * 2]..[sp - ptr_size * 3 - 1] = arg names object
        [sp - ptr_size * 4]..[sp - ptr_size * 4 - 1] = arg values object

    result stack:
        [sp]..[sp - ptr_size - 1] = return object
 */

/**
* Symbol value expression flag.
*
* Holds the name of the symbol in one field.
*
* 1. symbol name - "symbolName"
*/
#define EXPRESSION_SYMBOL_VALUE 0x06
/*
    BC:
    load_symbol
    - load symbol value by name
    stack:
        [sp]..[sp - ptr_size - 1] = symbol name

 */

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
