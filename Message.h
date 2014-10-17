#include "TTObject.h"

#ifndef MESSAGE_H
#define MESSAGE_H

namespace Message
{
    TTObject *createSimpleMessage(TTLiteral *name);
    TTObject *createMultipleMessage(TTLiteral *fullName, TTLiteral *nameArray, TTLiteral *valueArray);
};

#endif
