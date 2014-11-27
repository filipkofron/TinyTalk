#include "BuiltinObjectArray.h"

TTObject *BuiltinObjectArraySize::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    TTObject *sizeObj = TTLiteral::createIntegerLiteral((int32_t) (dest->getLiteral()->length / sizeof(TTObject *)));

    return sizeObj;
}
