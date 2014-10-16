#ifndef PARSER_H
#define PARSER_H

class Parser;

#include "TTObject.h"
#include "Tokenizer.h"

class Parser
{
private:
    std::shared_ptr<Tokenizer> tokenizer;
public:
    Parser(std::shared_ptr<Tokenizer> tokenizer);
    TTObject *parse();
};

#endif
