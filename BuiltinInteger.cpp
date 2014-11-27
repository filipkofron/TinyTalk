#include "BuiltinInteger.h"
#include "common.h"
#include "Runtime.h"
#include <iostream>

TTObject *BuiltinIntegerAdd::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    TTObject *res = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            + *((int32_t *) values[0]->getLiteral()->data));

    return res;
}

TTObject *BuiltinIntegerMinus::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    TTObject *res = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            - *((int32_t *) values[0]->getLiteral()->data));
    return res;
}

TTObject *BuiltinIntegerMul::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    TTObject *res = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            * *((int32_t *) values[0]->getLiteral()->data));

    return res;
}

TTObject *BuiltinIntegerDiv::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    if(*((int32_t *) values[0]->getLiteral()->data) == 0)
    {
        std::cerr << "[Builtin]: Division by zero!" << std::endl;
        throw std::exception();
    }

    TTObject *res = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            / *((int32_t *) values[0]->getLiteral()->data));

    return res;
}

TTObject *BuiltinIntegerMod::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_INTEGER(0);

    if(*((int32_t *) values[0]->getLiteral()->data) == 0)
    {
        std::cerr << "[Builtin]: Division by zero!" << std::endl;
        throw std::exception();
    }

    TTObject *res = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            % *((int32_t *) values[0]->getLiteral()->data));

    return res;
}

TTObject *BuiltinIntegerToString::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    std::string val = std::to_string(*((int32_t *) dest->getLiteral()->data));

    TTObject *res = TTLiteral::createStringLiteral(TO_TT_STR(val.c_str()));

    return res;
}

TTObject *BuiltinIntegerLessThan::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
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

TTObject *BuiltinIntegerGreaterThan::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
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

TTObject *BuiltinIntegerEquals::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
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

TTObject *BuiltinIntegerLessThanOrEqual::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
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

TTObject *BuiltinIntegerGreaterThanOrEqual::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
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

TTObject *BuiltinIntegerFromString::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);

    BUILTIN_CHECK_STRING(0);

    int32_t a = 0;
    if(sscanf((const char *) values[0]->getLiteral()->data, "%d", &a) != 1)
    {
        std::cerr << "[Builtin]: FromString builtin function parse error." << std::endl;
        throw std::exception();
    }

    TTObject *res = TTLiteral::createIntegerLiteral(a);

    return res;
}
