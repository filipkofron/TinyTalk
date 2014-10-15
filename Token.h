#ifndef TOKEN_H
#define TOKEN_H

class Token;

#include <string>

const static char *TypeStrings[11] =
{
        "Symbol",
        "Integer",
        "String",
        "Assignment",
        "Character",
        "ParenthesisOpen",
        "ParenthesisClose",
        "BlockOpen",
        "BlockClose",
        "Invalid",
        "EOF",
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
        TEOF = 10
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
