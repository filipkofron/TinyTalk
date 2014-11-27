#include <string.h>
#include <algorithm>
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

    if((int32_t) thisStr->length <= index + 1)
    {
        std::cerr << "[Builtin]: Invalid string index " << *((int32_t *) argInt->data) << " string length: " << (thisStr->length - 1) << std::endl;
        throw std::exception();
    }

    uint8_t cr[2] = {0, 0};

    cr[0] = (uint8_t) thisStr->data[index];

    TTObject *res = TTLiteral::createStringLiteral(TO_TT_STR(cr));

    return res;
}

TTObject *BuiltinStringSetCharAt::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(2, 2);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);

    BUILTIN_CHECK_LITERAL(1);
    BUILTIN_CHECK_INTEGER(1);

    TTLiteral *thisStr = dest->getLiteral();
    TTLiteral *argStr = values[0]->getLiteral();
    TTLiteral *argInt = values[1]->getLiteral();

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

    TTObject *res = TTLiteral::createStringLiteral(thisStr->data);
    res->getLiteral()->data[index] = argStr->data[0];
    return res;
}

TTObject *BuiltinStringToLower::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    TTLiteral *thisStr = dest->getLiteral();

    std::string str = (const char *) thisStr->data;

    std::transform(str.begin(), str.end(),str.begin(), ::tolower);

    TTObject *res = TTLiteral::createStringLiteral(TO_TT_STR(str.c_str()));

    return res;
}

TTObject *BuiltinStringToUpper::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    TTLiteral *thisStr = dest->getLiteral();

    std::string str = (const char *) thisStr->data;

    std::transform(str.begin(), str.end(),str.begin(), ::toupper);

    TTObject *res = TTLiteral::createStringLiteral(TO_TT_STR(str.c_str()));

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

TTObject *BuiltinStringTrim::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    TTLiteral *thisStr = dest->getLiteral();

    std::string str = (const char *) thisStr->data;

    str = trim(str);

    TTObject *res = TTLiteral::createStringLiteral(TO_TT_STR(str.c_str()));

    return res;
}

TTObject *BuiltinStringAppend::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);

    TTLiteral *thisStr = dest->getLiteral();
    TTLiteral *rightStr = values[0]->getLiteral();

    TTObject *appendedObj = TTLiteral::createStringLiteral(thisStr->length + rightStr->length - 1);

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

TTObject *BuiltinStringLength::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 0);

    TTLiteral *thisStr = dest->getLiteral();

    TTObject *res = TTLiteral::createIntegerLiteral(thisStr->length - 1);

    return res;
}

TTObject *BuiltinStringToString::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    return dest;
}
