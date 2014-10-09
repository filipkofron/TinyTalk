#include "Token.h"

Token::Token(const Type &type, const std::string &value)
    : type(type), value(value)
{

}

const Token::Type &Token::getType()
{
    return type;
}

const std::string &Token::getValue()
{
    return value;
}
