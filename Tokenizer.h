#ifndef TOKENIZER_H
#define TOKENIZER_H

class Tokenizer;

#include "Reader.h"
#include "Token.h"
#include <memory>

#define DEBUG_TOKENIZER_MSG "[TOKENIZER] >> "

class Tokenizer
{
private:
    bool isWhitespace(const int &c);
    bool isLetter(const int &c);
    bool isSymbol(const int &c);
    bool isSymbolStart(const int &c);
    bool isDigit(const int &c);
    bool isString(const int &c);
    bool isCharacter(const int &c);
    bool isEndOfFile(const int &c);
    std::string readInteger();
    std::string readSymbol();
    std::string readString();
    std::string readCharacter();
    void eatWhitespace();
    std::shared_ptr<Reader> reader;
public:
    Tokenizer(std::shared_ptr<Reader> reader);
    Token readNextToken();
};

#endif
