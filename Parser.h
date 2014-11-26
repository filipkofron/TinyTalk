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

    TTObject *parseRightOfValue(TTObject *destExpr, Token &prevToken, const bool &untilChain);
    TTObject *parseSingleValue();

    TTObject *parseSymbol(const bool &parseOnlyOne, const bool &untilChain);
    TTObject *parseLiteral(const bool &parseOnlyOne, const bool &untilChain);
    TTObject *parseSimpleMessageRest(TTObject *destExpr, const bool &untilChain);
    TTObject *parseMultipleMessageRest(TTObject *destExpr, const bool &untilChain);
    TTObject *parseBlock(const bool &parseOnlyOne, const bool &untilChain);
    TTObject *parseAssignmentRest(const Token &token);
    TTObject *parseParenthesis(const bool &parseOnlyOne, const bool &untilChain);
    TTObject *parseArray(const bool &parseOnlyOne, const bool &untilChain);
    TTObject *parseChain(TTObject *currExpr, const bool &parseOnlyOne);
    TTObject *parseCreateVariable(const bool &parseOnlyOne, const bool &untilChain);

public:
    Parser(std::shared_ptr<Tokenizer> tokenizer);
    TTObject *parse(const bool &parseOnlyOne, const bool &untilChain);
};

#endif
