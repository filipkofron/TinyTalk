#ifndef PARSER_H
#define PARSER_H

class Parser;

#include "TTObject.h"
#include "Tokenizer.h"

#define PARSER_MAX_METHOD_ARGS 1024

class Parser
{
private:
    std::shared_ptr<Tokenizer> tokenizer;

    bool isSimpleMessage(const Token &token);
    bool isBlockArgument(const Token &token);
    std::string getBlockArgumentName(const Token &token);
    std::string getMultipleMessageName(const Token &token);

    TTLiteral *createLiteralArrayOfStrings(std::vector<std::string> &strs);

    TTObject *parseRightOfValue(TTObject *destExpr, Token &prevToken);
    TTObject *parseSingleValue();

    TTObject *parseSymbol(const bool &parseOnlyOne);
    TTObject *parseLiteral(const bool &parseOnlyOne);
    TTObject *parseSimpleMessageRest(TTObject *destExpr);
    TTObject *parseMultipleMessageRest(TTObject *destExpr);
    TTObject *parseBlock(const bool &parseOnlyOne);
    TTObject *parseAssignmentRest(const Token &token);
    TTObject *parseParenthesis(const bool &parseOnlyOne);
    TTObject *parseChain(TTObject *currExpr, const bool &parseOnlyOne);
    TTObject *parseCreateVariable(const bool &parseOnlyOne);

public:
    Parser(std::shared_ptr<Tokenizer> tokenizer);
    TTObject *parse(const bool &parseOnlyOne);
};

#endif
