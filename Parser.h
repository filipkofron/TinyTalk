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
    std::string getMultipleMessageName(const Token &token);

    TTObject *parseSymbol(const bool &parseOnlyOne);
    TTObject *parseLiteral(const bool &parseOnlyOne);
    TTObject *parseSimpleMessageRest(TTObject *destExpr);
    TTObject *parseMultipleMessageRest(TTObject *destExpr);

    TTObject *parseAssignmentRest(const Token &token);

    TTObject *parseParenthesis(const bool &parseOnlyOne);

public:
    Parser(std::shared_ptr<Tokenizer> tokenizer);
    TTObject *parse();
};

#endif
