#ifndef TOKEN_H
#define TOKEN_H

class Token;

#include <string>

class Token
{
public:
    enum Type
    {
        IDENTIFIER,
        SYMBOL,
        INTEGER,
        STRING,
        ASSIGNMENT,
        CHARACTER,
        PARENTHESIS_OPEN,
        PARENTHESIS_CLOSE,
        BLOCK_OPEN,
        BLOCK_CLOSE,
        INVALID
    };

private:
    std::string value;
    Type type;

public:
    const std::string &getValue();
    const Type &getType();
};

#endif
