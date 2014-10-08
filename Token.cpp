#include "Token.h"

const std::string &Token::getValue()
{
    return value;
}

const Token::Type &Token::getType()
{
    return type;
}