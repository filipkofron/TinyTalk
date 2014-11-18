#include <string.h>
#include "BuiltinString.h"
#include "common.h"

TTObject *BuiltinStringCharAt::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_INTEGER(0);

    TTLiteral *thisStr = dest->getLiteral();
    TTLiteral *argInt = values[0]->getLiteral();

    int32_t index = *((int32_t *) argInt->data);

    if((int32_t) thisStr->length < index)
    {
        std::cerr << "[Builtin]: Invalid string index " << *((int32_t *) argInt->data) << " string length: " << thisStr->length << std::endl;
        throw std::exception();
    }

    TTLiteral *lit = TTLiteral::createIntegerLiteral(thisStr->data[index]);
    TTObject *res = TTObject::createObject(TT_LITERAL);
    res->setLiteral(lit);

    return res;

}

TTObject *BuiltinStringSetCharAt::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    NOT_IMPLEMENTED
}

TTObject *BuiltinStringToLower::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    NOT_IMPLEMENTED
}

TTObject *BuiltinStringToUpper::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    NOT_IMPLEMENTED
}

TTObject *BuiltinStringTrim::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    NOT_IMPLEMENTED
}

TTObject *BuiltinStringAppend::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    NOT_IMPLEMENTED
}

TTObject *BuiltinStringToString::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    return dest;
}
