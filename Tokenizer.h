#ifndef TOKENIZER_H
#define TOKENIZER_H

class Tokenizer;

#include "STReader.h"
#include "Token.h"
#include <memory>

class Tokenizer
{
private:
    bool isWhitespace(const int &c);
    bool isDigit(const int &c);
    std::shared_ptr<STReader> reader;
public:
    Tokenizer(std::shared_ptr<STReader> reader);
    Token readNextToken();
};

#endif
