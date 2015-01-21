#include "BuiltinInteger.h"
#include "Common.h"
#include "Runtime.h"
#include <iostream>

RefPtr<TTObject> BuiltinIntegerAdd::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    RefPtr<TTObject> res = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            + *((int32_t *) values[0]->getLiteral()->data));

    return res;
}

RefPtr<TTObject> BuiltinIntegerMinus::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    RefPtr<TTObject> res = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            - *((int32_t *) values[0]->getLiteral()->data));
    return res;
}

RefPtr<TTObject> BuiltinIntegerMul::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    RefPtr<TTObject> res = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            * *((int32_t *) values[0]->getLiteral()->data));

    return res;
}

RefPtr<TTObject> BuiltinIntegerDiv::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    if(*((int32_t *) values[0]->getLiteral()->data) == 0)
    {
        std::cerr << "[Builtin]: Division by zero!" << std::endl;
        KILL;
    }

    RefPtr<TTObject> res = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            / *((int32_t *) values[0]->getLiteral()->data));

    return res;
}

RefPtr<TTObject> BuiltinIntegerMod::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    if(*((int32_t *) values[0]->getLiteral()->data) == 0)
    {
        std::cerr << "[Builtin]: Division by zero!" << std::endl;
        KILL;
    }

    RefPtr<TTObject> res = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            % *((int32_t *) values[0]->getLiteral()->data));

    return res;
}

RefPtr<TTObject> BuiltinIntegerToString::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    std::string val = std::to_string(*((int32_t *) dest->getLiteral()->data));

    RefPtr<TTObject> res = TTLiteral::createStringLiteral(TO_TT_STR(val.c_str()));

    return res;
}

RefPtr<TTObject> BuiltinIntegerLessThan::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    if(*((int32_t *) dest->getLiteral()->data)
            < *((int32_t *) values[0]->getLiteral()->data))
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
}

RefPtr<TTObject> BuiltinIntegerGreaterThan::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    if(*((int32_t *) dest->getLiteral()->data)
            > *((int32_t *) values[0]->getLiteral()->data))
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
}

RefPtr<TTObject> BuiltinIntegerEquals::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    if(*((int32_t *) dest->getLiteral()->data)
            == *((int32_t *) values[0]->getLiteral()->data))
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
}

RefPtr<TTObject> BuiltinIntegerLessThanOrEqual::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    if(*((int32_t *) dest->getLiteral()->data)
            <= *((int32_t *) values[0]->getLiteral()->data))
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
}

RefPtr<TTObject> BuiltinIntegerGreaterThanOrEqual::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    if(*((int32_t *) dest->getLiteral()->data)
            >= *((int32_t *) values[0]->getLiteral()->data))
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
}

RefPtr<TTObject> BuiltinIntegerFromString::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_STRING(0);

    int32_t a = 0;
    if(sscanf((const char *) values[0]->getLiteral()->data, "%d", &a) != 1)
    {
        std::cerr << "[Builtin]: FromString builtin function parse error." << std::endl;
        KILL;
    }

    RefPtr<TTObject> res = TTLiteral::createIntegerLiteral(a);

    return res;
}

RefPtr<TTObject> BuiltinIntegerCharValue::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    uint8_t str[2];
    str[0] = (uint8_t) *((int32_t *) dest->getLiteral()->data);
    str[1] = '\0';

    RefPtr<TTObject> res = TTLiteral::createStringLiteral(str);

    return res;
}
