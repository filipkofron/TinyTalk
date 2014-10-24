#include "BuiltinObject.h"

TTObject *BuiltinObjectAlloc::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    return TTObject::createObject(TT_OBJECT);
}
