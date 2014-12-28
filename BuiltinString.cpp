#include <string.h>
#include <algorithm>
#include "BuiltinString.h"
#include "Common.h"
#include "Runtime.h"

RefPtr<TTObject> BuiltinStringCharAt::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_INTEGER(0);

    RefPtr<TTLiteral> thisStr = dest->getLiteral();
    RefPtr<TTLiteral> argInt = values[0]->getLiteral();

    int32_t index = *((int32_t *) argInt->data);

    if((int32_t) thisStr->length <= index + 1)
    {
        std::cerr << "[Builtin]: Invalid string index " << *((int32_t *) argInt->data) << " string length: " << (thisStr->length - 1) << std::endl;
        throw std::exception();
    }

    uint8_t cr[2] = {0, 0};

    cr[0] = (uint8_t) thisStr->data[index];

    RefPtr<TTObject> res = TTLiteral::createStringLiteral(TO_TT_STR(cr));

    return res;
}

RefPtr<TTObject> BuiltinStringCharAsIntAt::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_INTEGER(0);

    RefPtr<TTLiteral> thisStr = dest->getLiteral();
    RefPtr<TTLiteral> argInt = values[0]->getLiteral();

    int32_t index = *((int32_t *) argInt->data);

    if((int32_t) thisStr->length <= index + 1)
    {
        std::cerr << "[Builtin]: Invalid string index " << *((int32_t *) argInt->data) << " string length: " << (thisStr->length - 1) << std::endl;
        throw std::exception();
    }

    RefPtr<TTObject> res = TTLiteral::createIntegerLiteral((uint8_t) thisStr->data[index]);

    return res;
}

RefPtr<TTObject> BuiltinStringSetCharAt::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(2, 2);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);

    BUILTIN_CHECK_LITERAL(1);
    BUILTIN_CHECK_INTEGER(1);

    RefPtr<TTLiteral> thisStr = dest->getLiteral();
    RefPtr<TTLiteral> argStr = values[0]->getLiteral();
    RefPtr<TTLiteral> argInt = values[1]->getLiteral();

    int32_t index = *((int32_t *) argInt->data);

    if((int32_t) thisStr->length <= index + 1)
    {
        std::cerr << "[Builtin]: Error: Invalid string index " << *((int32_t *) argInt->data) << " string length: " << (thisStr->length - 1) << std::endl;
        throw std::exception();
    }

    if((int32_t) argStr->length != 2)
    {
        std::cerr << "[Builtin]: Error: Not a single char !!" << std::endl;
        throw std::exception();
    }

    RefPtr<TTObject> res = TTLiteral::createStringLiteral(thisStr->data);
    res->getLiteral()->data[index] = argStr->data[0];
    return res;
}

RefPtr<TTObject> BuiltinStringToLower::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    RefPtr<TTLiteral> thisStr = dest->getLiteral();

    std::string str = (const char *) thisStr->data;

    std::transform(str.begin(), str.end(),str.begin(), ::tolower);

    RefPtr<TTObject> res = TTLiteral::createStringLiteral(TO_TT_STR(str.c_str()));

    return res;
}

RefPtr<TTObject> BuiltinStringToUpper::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    RefPtr<TTLiteral> thisStr = dest->getLiteral();

    std::string str = (const char *) thisStr->data;

    std::transform(str.begin(), str.end(),str.begin(), ::toupper);

    RefPtr<TTObject> res = TTLiteral::createStringLiteral(TO_TT_STR(str.c_str()));

    return res;
}

static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

RefPtr<TTObject> BuiltinStringTrim::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    RefPtr<TTLiteral> thisStr = dest->getLiteral();

    std::string str = (const char *) thisStr->data;

    str = trim(str);

    RefPtr<TTObject> res = TTLiteral::createStringLiteral(TO_TT_STR(str.c_str()));

    return res;
}

RefPtr<TTObject> BuiltinStringAppend::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);

    RefPtr<TTLiteral> thisStr = dest->getLiteral();
    RefPtr<TTLiteral> rightStr = values[0]->getLiteral();

    RefPtr<TTObject> appendedObj = TTLiteral::createStringLiteral(thisStr->length + rightStr->length - 1);

    uint8_t *destStr = appendedObj->getLiteral()->data;
    uint8_t *left = thisStr->data;
    uint8_t *right = rightStr->data;

    while(*left)
    {
        *destStr++ = *left++;
    }

    while(*right)
    {
        *destStr++ = *right++;
    }
    *destStr = '\0';

    return appendedObj;
}

RefPtr<TTObject> BuiltinStringLength::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    RefPtr<TTLiteral> thisStr = dest->getLiteral();

    RefPtr<TTObject> res = TTLiteral::createIntegerLiteral(thisStr->length - 1);

    return res;
}

RefPtr<TTObject> BuiltinStringToString::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    return dest;
}

RefPtr<TTObject> BuiltinStringStartsWith::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);

    uint8_t *destStr = dest->getLiteral()->data;
    uint8_t *givenStr = values[0]->getLiteral()->data;

    while(*destStr && *givenStr)
    {
        if(*destStr++ != *givenStr++)
        {
            return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
        }
    }

    if(!*destStr && !*givenStr)
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    if(!*destStr)
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
}

RefPtr<TTObject> BuiltinStringEquals::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);

    uint8_t *destStr = dest->getLiteral()->data;
    uint8_t *givenStr = values[0]->getLiteral()->data;

    if(COMPARE_NAME(destStr, givenStr) == 0)
    {
        return Runtime::globalEnvironment->getField(TO_TT_STR("True"));
    }

    return Runtime::globalEnvironment->getField(TO_TT_STR("False"));
}
