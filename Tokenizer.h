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
    bool isDigitBegin(const int &c);
    bool isString(const int &c);
    bool isCharacter(const int &c);
    bool isEndOfFile(const int &c);
    bool isAssignment(const int &c);
    bool isParenthesisOpen(const int &c);
    bool isParenthesisClose(const int &c);
    bool isBlockOpen(const int &c);
    bool isBlockClose(const int &c);
    bool isExpressionEnd(const int &c);
    bool isReturn(const int &c);
    bool isVerticalBar(const int &c);
    bool isComment(const int &c);
    std::string readInteger();
    std::string readSymbol();
    std::string readString();
    std::string readCharacter();
    void readAssignment();
    void readParenthesisOpen();
    void readParenthesisClose();
    void readBlockOpen();
    void readBlockClose();
    void readExpressionEnd();
    void readReturn();
    void readVerticalBar();
    void readComment();
    void eatWhitespace();

    Token reallyReadToken();

    std::shared_ptr<Reader> reader;
    bool reachedEOF;
    std::stack<Token> tokenBuffer;
    uint32_t lineCounter;
public:
    Tokenizer(std::shared_ptr<Reader> reader);
    Token readToken();
    void putBackToken(Token token);
    Token peekToken();
    bool hasReachedEOF();
};

#endif
