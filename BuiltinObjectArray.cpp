#include "BuiltinObjectArray.h"
#include "Runtime.h"
#include "Common.h"
#include "Evaluator.h"

#include <sstream>

RefPtr<TTObject> BuiltinObjectArraySize::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    RefPtr<TTObject> sizeObj = TTLiteral::createIntegerLiteral((int32_t) (dest->getLiteral()->length / sizeof(TTObject *)));

    return sizeObj;
}

RefPtr<TTObject> BuiltinObjectArrayNewWithSize::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    int32_t size = *((int32_t *) values[0]->getLiteral()->data);

    if(size < 0)
    {
        std::cerr << "[Builtin]: Error: Invalid array size, must be 0 to 2^31!" << std::endl;
        KILL;
    }

    RefPtr<TTObject> nil = Runtime::globalEnvironment->getField(TO_TT_STR("nil"));

    RefPtr<TTObject> obj = TTLiteral::createObjectArray(size);
    for(int32_t i = 0; i < size; i++)
    {
        ((TTObject **) obj->getLiteral()->data)[i] = &nil;
    }

    return obj;
}

RefPtr<TTObject> BuiltinObjectArrayAt::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    int32_t elemIndex = *((int32_t *) values[0]->getLiteral()->data);
    int32_t size = (int32_t) (dest->getLiteral()->length / sizeof(TTObject *));

    if(elemIndex < 0 || elemIndex >= size)
    {
        std::cerr << "[Builtin]: Error: Index ouf of bounds!" << std::endl;
        KILL;
    }

    RefPtr<TTObject> obj = ((TTObject **) dest->getLiteral()->data)[elemIndex];

    if(!&obj)
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("nil"));
    }
    return obj;
}

RefPtr<TTObject> BuiltinObjectArrayAtSet::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(2, 2);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    int32_t elemIndex = *((int32_t *) values[0]->getLiteral()->data);
    int32_t size = (int32_t) (dest->getLiteral()->length / sizeof(TTObject *));

    if(elemIndex < 0 || elemIndex >= size)
    {
        std::cerr << "[Builtin]: Error: Index ouf of bounds!" << std::endl;
        KILL;
    }

    ((TTObject **) dest->getLiteral()->data)[elemIndex] = &values[1];

    return dest;
}

RefPtr<TTObject>  BuiltinObjectArrayEmpty::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    int32_t size = (int32_t) (dest->getLiteral()->length / sizeof(TTObject *));

    const char *toFind = size == 0 ? "True" : "False";

    return Runtime::globalEnvironment->getField(TO_TT_STR(toFind));
}

RefPtr<TTObject>  BuiltinObjectArrayToString::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    int32_t size = (int32_t) (dest->getLiteral()->length / sizeof(TTObject *));

    std::stringstream ss;

    Evaluator evaluator;
    std::string toString = "toString";

    for(int32_t i = 0; i < size; i++)
    {
        RefPtr<TTObject> obj = ((TTObject **) dest->getLiteral()->data)[i];
        RefPtr<TTObject> toStringRes = evaluator.sendSimpleMessage(obj, toString);
        ss << "[" << i << "]: ";
        if(&toStringRes)
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

    RefPtr<TTObject> resStr = TTLiteral::createStringLiteral(TO_TT_STR(res.c_str()));

    return resStr;
}
