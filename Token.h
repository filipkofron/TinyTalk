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
    Type type;
    std::string value;

public:
    Token(const Type &type, const std::string &value);

    const Type &getType();
    const std::string &getValue();
};

#endif
