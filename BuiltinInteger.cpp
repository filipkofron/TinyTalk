#include "BuiltinInteger.h"
#include "common.h"
#include "Runtime.h"
#include <iostream>

TTObject *BuiltinIntegerAdd::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if(dest->type != TT_LITERAL || dest->getLiteral()->type != LITERAL_TYPE_INTEGER)
    {
        std::cerr << "Buitlin integer add expects integer destination." << std::endl;
        return Runtime::globalEnvironment->getField(TO_TT_STR("nil"));
    }

    std::cerr << "Function " << __FILE__ << ":" << __FUNCTION__ << " not implemented!" << std::endl;
    throw std::exception();
}

TTObject *BuiltinIntegerMinus::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    std::cerr << "Function " << __FILE__ << ":" << __FUNCTION__ << " not implemented!" << std::endl;
    throw std::exception();
}

TTObject *BuiltinIntegerMul::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    std::cerr << "Function " << __FILE__ << ":" << __FUNCTION__ << " not implemented!" << std::endl;
    throw std::exception();
}

TTObject *BuiltinIntegerDiv::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    std::cerr << "Function " << __FILE__ << ":" << __FUNCTION__ << " not implemented!" << std::endl;
    throw std::exception();
}

TTObject *BuiltinIntegerToString::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    std::cerr << "Function " << __FILE__ << ":" << __FUNCTION__ << " not implemented!" << std::endl;
    throw std::exception();
}
