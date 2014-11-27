#include "BuiltinObjectArray.h"
#include "Runtime.h"
#include "common.h"
#include "Evaluator.h"

#include <sstream>

TTObject *BuiltinObjectArraySize::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    TTObject *sizeObj = TTLiteral::createIntegerLiteral((int32_t) (dest->getLiteral()->length / sizeof(TTObject *)));

    return sizeObj;
}

TTObject *BuiltinObjectArrayAt::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    int32_t elemIndex = *((int32_t *) values[0]->getLiteral()->data);
    int32_t size = (int32_t) (dest->getLiteral()->length / sizeof(TTObject *));

    if(elemIndex < 0 || elemIndex >= size)
    {
        std::cerr << "[Builtin]: Error: Index ouf of bounds!" << std::endl;
        throw std::exception();
    }

    TTObject *obj = ((TTObject **) dest->getLiteral()->data)[elemIndex];

    if(!obj)
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("nil"));
    }
    return obj;
}

TTObject *BuiltinObjectArrayAtSet::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(2, 2);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    int32_t elemIndex = *((int32_t *) values[0]->getLiteral()->data);
    int32_t size = (int32_t) (dest->getLiteral()->length / sizeof(TTObject *));

    if(elemIndex < 0 || elemIndex >= size)
    {
        std::cerr << "[Builtin]: Error: Index ouf of bounds!" << std::endl;
        throw std::exception();
    }

    ((TTObject **) dest->getLiteral()->data)[elemIndex] = values[1];

    return dest;
}


TTObject * BuiltinObjectArrayEmpty::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    int32_t size = (int32_t) (dest->getLiteral()->length / sizeof(TTObject *));

    const char *toFind = size == 0 ? "True" : "False";

    return Runtime::globalEnvironment->getField(TO_TT_STR(toFind));
}

TTObject * BuiltinObjectArrayToString::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    int32_t size = (int32_t) (dest->getLiteral()->length / sizeof(TTObject *));

    std::stringstream ss;

    Evaluator evaluator;
    std::string toString = "toString";

    for(int32_t i = 0; i < size; i++)
    {
        TTObject *obj = ((TTObject **) dest->getLiteral()->data)[i];
        TTObject *toStringRes = evaluator.sendSimpleMessage(obj, toString);
        ss << "[" << i << "]: ";
        if(toStringRes)
        {
            ss << ((const char *) toStringRes->getLiteral()->data) << std::endl;
        }
        else
        {
            ss << "nil" << std::endl;
        }
    }

    std::string res;

    getline(ss, res, '\0');

    TTObject *resStr = TTLiteral::createStringLiteral(TO_TT_STR(res.c_str()));

    return resStr;
}
