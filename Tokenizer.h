#ifndef TOKENIZER_H
#define TOKENIZER_H

class Tokenizer;

#include "Reader.h"
#include "Token.h"
#include <memory>

class Tokenizer
{
private:
    bool isWhitespace(const int &c);
    bool isDigit(const int &c);
    std::shared_ptr<Reader> reader;
public:
    Tokenizer(std::shared_ptr<Reader> reader);
    Token readNextToken();
};

#endif
