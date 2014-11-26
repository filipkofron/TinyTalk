#ifndef TOKEN_H
#define TOKEN_H

class Token;

#include <string>

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
        VERTICAL_BAR = 13,
        ARRAY_OPEN = 14,
        ARRAY_CLOSE = 15,
        ARRAY_SEPARATEOR = 16,
    };

private:
    uint32_t line;
    Type type;
    std::string value;

public:
    Token(const uint32_t &line);
    Token(const uint32_t &line, const Type &type);
    Token(const uint32_t &line, const Type &type, const std::string &value);

    const uint32_t &getLine() const;
    const Type &getType() const ;
    const std::string &getValue() const;
    const char *getTypeInfo() const;
};

#endif
