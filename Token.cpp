#include "Token.h"

Token::Token(const uint32_t &line)
        : line(line), type(Type::INVALID), value("")
{

}

Token::Token(const uint32_t &line, const Type &type)
        : line(line), type(type), value("")
{

}

Token::Token(const uint32_t &line, const Type &type, const std::string &value)
        : line(line), type(type), value(value)
{

}

const uint32_t &Token::getLine() const
{
    return line;
}

const Token::Type &Token::getType() const
{
    return type;
}

const std::string &Token::getValue() const
{
    return value;
}

const static char *TypeStrings[17] =
        {
                "SYMBOL",
                "INTEGER",
                "STRING",
                "ASSIGNMENT",
                "CHARACTER",
                "PARENTHESIS_OPEN",
                "PARENTHESIS_CLOSE",
                "BLOCK_OPEN",
                "BLOCK_CLOSE",
                "INVALID",
                "EOF",
                "EXPRESSION_END",
                "RETURN",
                "VERTICAL_BAR",
                "ARRAY_OPEN",
                "ARRAY_CLOSE",
                "ARRAY_SEPARATOR",
        };

const char *Token::getTypeInfo() const
{
    switch (getType())
    {
        case Type::SYMBOL:
        case Type::INTEGER:
        case Type::STRING:
        case Type::ASSIGNMENT:
        case Type::CHARACTER:
        case Type::PARENTHESIS_OPEN:
        case Type::PARENTHESIS_CLOSE:
        case Type::BLOCK_OPEN:
        case Type::BLOCK_CLOSE:
        case Type::INVALID:
        case Type::TEOF:
        case Type::EXPRESSION_END:
        case Type::RETURN:
        case Type::VERTICAL_BAR:
        case Type::ARRAY_OPEN:
        case Type::ARRAY_CLOSE:
        case Type::ARRAY_SEPARATEOR:
            return TypeStrings[getType()];
        default:
            return "Unknown";
    }
}
