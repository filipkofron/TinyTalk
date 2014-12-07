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

    RefPtr<TTLiteral> createLiteralArrayOfStrings(std::vector<std::string> &strs);

    RefPtr<TTObject> parseRightOfValue(RefPtr<TTObject> destExpr, Token &prevToken, const bool &untilChain);
    RefPtr<TTObject> parseSingleValue();

    RefPtr<TTObject> parseSymbol(const bool &parseOnlyOne, const bool &untilChain);
    RefPtr<TTObject> parseLiteral(const bool &parseOnlyOne, const bool &untilChain);
    RefPtr<TTObject> parseSimpleMessageRest(RefPtr<TTObject> destExpr, const bool &untilChain);
    RefPtr<TTObject> parseMultipleMessageRest(RefPtr<TTObject> destExpr, const bool &untilChain);
    RefPtr<TTObject> parseBlock(const bool &parseOnlyOne, const bool &untilChain);
    RefPtr<TTObject> parseAssignmentRest(const Token &token);
    RefPtr<TTObject> parseParenthesis(const bool &parseOnlyOne, const bool &untilChain);
    RefPtr<TTObject> parseArray(const bool &parseOnlyOne, const bool &untilChain);
    RefPtr<TTObject> parseChain(RefPtr<TTObject> currExpr, const bool &parseOnlyOne);
    RefPtr<TTObject> parseCreateVariable(const bool &parseOnlyOne, const bool &untilChain);

public:
    Parser(std::shared_ptr<Tokenizer> tokenizer);
    RefPtr<TTObject> parse(const bool &parseOnlyOne, const bool &untilChain);
};

#endif
