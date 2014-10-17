#ifndef PARSER_H
#define PARSER_H

class Parser;

#include "TTObject.h"
#include "Tokenizer.h"

class Parser
{
private:
    std::shared_ptr<Tokenizer> tokenizer;

    bool isSimpleMessage(const Token &token);

    TTObject *parseSymbol();
    TTObject *parseSimpleMessageRest(TTObject *destExpr);

    TTObject *parseAssignmentRest(const Token &token);

    TTObject *parseParenthesis();

public:
    Parser(std::shared_ptr<Tokenizer> tokenizer);
    TTObject *parse();
};

#endif
