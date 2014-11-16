#include "BuiltinInteger.h"
#include "common.h"
#include "Runtime.h"
#include <iostream>

TTObject *BuiltinIntegerAdd::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: Add builtin function accepts one argument exactly." << std::endl;
        throw std::exception();
    }

    if (values[0]->type != TT_LITERAL)
    {
        std::cerr << "[Builtin]: Add builtin function cannot add anything else than TT_LITERAL." << std::endl;
        throw std::exception();
    }

    if (values[0]->getLiteral()->type != LITERAL_TYPE_INTEGER)
    {
        std::cerr << "[Builtin]: Add builtin function can only add integers now." << std::endl;
        throw std::exception();
    }

    TTLiteral *lit = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            + *((int32_t *) values[0]->getLiteral()->data));
    TTObject *res = TTObject::createObject(TT_LITERAL);
    res->setLiteral(lit);

    return res;
}

TTObject *BuiltinIntegerMinus::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: Minus builtin function accepts one argument exactly." << std::endl;
        throw std::exception();
    }

    if (values[0]->type != TT_LITERAL)
    {
        std::cerr << "[Builtin]: Minus builtin function cannot sub anything else than TT_LITERAL." << std::endl;
        throw std::exception();
    }

    if (values[0]->getLiteral()->type != LITERAL_TYPE_INTEGER)
    {
        std::cerr << "[Builtin]: Minus builtin function can only sub integers now." << std::endl;
        throw std::exception();
    }

    TTLiteral *lit = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            - *((int32_t *) values[0]->getLiteral()->data));
    TTObject *res = TTObject::createObject(TT_LITERAL);
    res->setLiteral(lit);

    return res;
}

TTObject *BuiltinIntegerMul::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: Mul builtin function accepts one argument exactly." << std::endl;
        throw std::exception();
    }

    if (values[0]->type != TT_LITERAL)
    {
        std::cerr << "[Builtin]: Mul builtin function cannot multiply anything else than TT_LITERAL." << std::endl;
        throw std::exception();
    }

    if (values[0]->getLiteral()->type != LITERAL_TYPE_INTEGER)
    {
        std::cerr << "[Builtin]: Mul builtin function can only multiply integers now." << std::endl;
        throw std::exception();
    }

    TTLiteral *lit = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            * *((int32_t *) values[0]->getLiteral()->data));
    TTObject *res = TTObject::createObject(TT_LITERAL);
    res->setLiteral(lit);

    return res;
}

TTObject *BuiltinIntegerDiv::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: Div builtin function accepts one argument exactly." << std::endl;
        throw std::exception();
    }

    if (values[0]->type != TT_LITERAL)
    {
        std::cerr << "[Builtin]: Div builtin function cannot divide anything else than TT_LITERAL." << std::endl;
        throw std::exception();
    }

    if (values[0]->getLiteral()->type != LITERAL_TYPE_INTEGER)
    {
        std::cerr << "[Builtin]: Div builtin function can only divide integers now." << std::endl;
        throw std::exception();
    }

    if(*((int32_t *) values[0]->getLiteral()->data) == 0)
    {
        std::cerr << "[Builtin]: Division by zero!" << std::endl;
        throw std::exception();
    }

    TTLiteral *lit = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            / *((int32_t *) values[0]->getLiteral()->data));
    TTObject *res = TTObject::createObject(TT_LITERAL);
    res->setLiteral(lit);

    return res;
}

TTObject *BuiltinIntegerMod::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: Mod builtin function accepts one argument exactly." << std::endl;
        throw std::exception();
    }

    if (values[0]->type != TT_LITERAL)
    {
        std::cerr << "[Builtin]: Mod builtin function cannot divide anything else than TT_LITERAL." << std::endl;
        throw std::exception();
    }

    if (values[0]->getLiteral()->type != LITERAL_TYPE_INTEGER)
    {
        std::cerr << "[Builtin]: Mod builtin function can only divide integers now." << std::endl;
        throw std::exception();
    }

    if(*((int32_t *) values[0]->getLiteral()->data) == 0)
    {
        std::cerr << "[Builtin]: Division by zero!" << std::endl;
        throw std::exception();
    }

    TTLiteral *lit = TTLiteral::createIntegerLiteral(*((int32_t *) dest->getLiteral()->data)
            % *((int32_t *) values[0]->getLiteral()->data));
    TTObject *res = TTObject::createObject(TT_LITERAL);
    res->setLiteral(lit);

    return res;
}

TTObject *BuiltinIntegerToString::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 0)
    {
        std::cerr << "[Builtin]: ToString builtin function accepts no arguments." << std::endl;
        throw std::exception();
    }

    std::string val = std::to_string(*((int32_t *) dest->getLiteral()->data));

    TTObject *res = TTObject::createObject(TT_LITERAL);
    TTLiteral *string = TTLiteral::createStringLiteral(TO_TT_STR(val.c_str()));

    res->setLiteral(string);

    return res;
}

TTObject *BuiltinIntegerLessThan::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: LessThan builtin function accepts one argument exactly." << std::endl;
        throw std::exception();
    }

    if (values[0]->type != TT_LITERAL)
    {
        std::cerr << "[Builtin]: LessThan builtin function cannot compare anything else than TT_LITERAL." << std::endl;
        throw std::exception();
    }

    if (values[0]->getLiteral()->type != LITERAL_TYPE_INTEGER)
    {
        std::cerr << "[Builtin]: LessThan builtin function can only compare integers now." << std::endl;
        throw std::exception();
    }

    if(*((int32_t *) dest->getLiteral()->data)
            < *((int32_t *) values[0]->getLiteral()->data))
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
}

TTObject *BuiltinIntegerGreaterThan::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: GreaterThan builtin function accepts one argument exactly." << std::endl;
        throw std::exception();
    }

    if (values[0]->type != TT_LITERAL)
    {
        std::cerr << "[Builtin]: GreaterThan builtin function cannot compare anything else than TT_LITERAL." << std::endl;
        throw std::exception();
    }

    if (values[0]->getLiteral()->type != LITERAL_TYPE_INTEGER)
    {
        std::cerr << "[Builtin]: GreaterThan builtin function can only compare integers now." << std::endl;
        throw std::exception();
    }

    if(*((int32_t *) dest->getLiteral()->data)
            > *((int32_t *) values[0]->getLiteral()->data))
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
}

TTObject *BuiltinIntegerEquals::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: Equals builtin function accepts one argument exactly." << std::endl;
        throw std::exception();
    }

    if (values[0]->type != TT_LITERAL)
    {
        std::cerr << "[Builtin]: Equals builtin function cannot compare anything else than TT_LITERAL." << std::endl;
        throw std::exception();
    }

    if (values[0]->getLiteral()->type != LITERAL_TYPE_INTEGER)
    {
        std::cerr << "[Builtin]: Equals builtin function can only compare integers now." << std::endl;
        throw std::exception();
    }

    if(*((int32_t *) dest->getLiteral()->data)
            == *((int32_t *) values[0]->getLiteral()->data))
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
}

TTObject *BuiltinIntegerLessThanOrEqual::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: LessThanOrEquals builtin function accepts one argument exactly." << std::endl;
        throw std::exception();
    }

    if (values[0]->type != TT_LITERAL)
    {
        std::cerr << "[Builtin]: LessThanOrEquals builtin function cannot compare anything else than TT_LITERAL." << std::endl;
        throw std::exception();
    }

    if (values[0]->getLiteral()->type != LITERAL_TYPE_INTEGER)
    {
        std::cerr << "[Builtin]: LessThanOrEquals builtin function can only compare integers now." << std::endl;
        throw std::exception();
    }

    if(*((int32_t *) dest->getLiteral()->data)
            <= *((int32_t *) values[0]->getLiteral()->data))
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
}

TTObject *BuiltinIntegerGreaterThanOrEqual::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: GreaterThanOrEqual builtin function accepts one argument exactly." << std::endl;
        throw std::exception();
    }

    if (values[0]->type != TT_LITERAL)
    {
        std::cerr << "[Builtin]: GreaterThanOrEqual builtin function cannot compare anything else than TT_LITERAL." << std::endl;
        throw std::exception();
    }

    if (values[0]->getLiteral()->type != LITERAL_TYPE_INTEGER)
    {
        std::cerr << "[Builtin]: GreaterThanOrEqual builtin function can only compare integers now." << std::endl;
        throw std::exception();
    }

    if(*((int32_t *) dest->getLiteral()->data)
            >= *((int32_t *) values[0]->getLiteral()->data))
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
}

TTObject *BuiltinIntegerFromString::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: FromString builtin function accepts one argument exactly." << std::endl;
        throw std::exception();
    }

    if (values[0]->type != TT_LITERAL)
    {
        std::cerr << "[Builtin]: FromString builtin function cannot load anything else than TT_LITERAL." << std::endl;
        throw std::exception();
    }

    if (values[0]->getLiteral()->type != LITERAL_TYPE_STRING)
    {
        std::cerr << "[Builtin]: FromString builtin function can only load strings now." << std::endl;
        throw std::exception();
    }

    int32_t a = 0;
    if(sscanf((const char *) values[0]->getLiteral()->data, "%d", &a) != 1)
    {
        std::cerr << "[Builtin]: FromString builtin function parse error." << std::endl;
        throw std::exception();
    }
    TTLiteral *lit = TTLiteral::createIntegerLiteral(a);
    TTObject *res = TTObject::createObject(TT_LITERAL);
    res->setLiteral(lit);

    return res;
}
