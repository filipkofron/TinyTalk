#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "TTObject.h"

#define EXPRESSION_FLAG_ASSIGN 0x01
#define EXPRESSION_FLAG_CHAINED 0x02
#define EXPRESSION_FLAG_PARENTHESIS 0x04

/**
* Simple message expression flag.
*
* Simple message expression object consists of
* 2 string literals.
*
* 1. destination object - "msgDestination"
* 2. message name - "msgName"
*/
#define EXPRESSION_FLAG_SIMPLE_MESSAGE 0x08

/**
* Simple message expression flag.
*
* Simple message expression object consists of
* 2 string literals and array literal.
*
* 1. destination object - "msgDestination"
* 2. message name - "msgName"
* 3. arguments object (consists of fields)
*/
#define EXPRESSION_FLAG_MULTIPLE_MESSAGE 0x10

namespace Expression
{
    TTObject *createSimpleMessage(uint8_t *destination, uint8_t *name);

    TTObject *createMultipleMessage(uint8_t *destination, uint8_t *name);
}

#endif
