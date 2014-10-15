#ifndef TOKEN_H
#define TOKEN_H

class Token;

#include <string>

const static char *TypeStrings[14] =
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
};

class Token
{
public:
    enum Type
    {
        SYMBOL = 0,
        INTEGER = 1,
        STRING = 2,
        ASSIGNMENT = 3,
        CHARACTER = 4,
        PARENTHESIS_OPEN = 5,
        PARENTHESIS_CLOSE = 6,
        BLOCK_OPEN = 7,
        BLOCK_CLOSE = 8,
        INVALID = 9,
        TEOF = 10,
        EXPRESSION_END = 11,
        RETURN = 12,
        VERTICAL_BAR = 13
    };

private:
    Type type;
    std::string value;

public:
    Token();
    Token(const Type &type);
    Token(const Type &type, const std::string &value);

    const Type &getType();
    const std::string &getValue();
    const char *getTypeInfo();
};

#endif
