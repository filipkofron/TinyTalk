#include "Common.h"
#include "Tokenizer.h"
#include "TokenizerException.h"
#include <cstdio>
#include <iostream>

bool Tokenizer::isWhitespace(const int &c)
{
    switch(c)
    {
        case '\t':
        case ' ':
        case '\n':
            return true;
    }

    return false;
}

bool Tokenizer::isLetter(const int &c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Tokenizer::isSymbol(const int &c)
{
    return isSymbolStart(c) || isDigit(c);
}

bool Tokenizer::isSymbolStart(const int &c)
{
    if(isLetter(c))
    {
        return true;
    }

    const char *allowedChars = "_+-/*^&~!:";

    while(*allowedChars)
    {
        if(*allowedChars++ == (char) c)
        {
            return true;
        }
    }

    return false;
}

bool Tokenizer::isDigit(const int &c)
{
    return c >= '0' && c <= '9';
}


bool Tokenizer::isDigitBegin(const int &c)
{
    return isDigit(c) || c == '-';
}

bool Tokenizer::isString(const int &c)
{
    return c == '"';
}

bool Tokenizer::isCharacter(const int &c)
{
    return c == '\'';
}

bool Tokenizer::isEndOfFile(const int &c)
{
    return c == EOF;
}

bool Tokenizer::isAssignment(const int &c)
{
    return c == '=';
}

bool Tokenizer::isParenthesisOpen(const int &c)
{
    return c == '(';
}

bool Tokenizer::isParenthesisClose(const int &c)
{
    return c == ')';
}

bool Tokenizer::isArraySeparator(const int &c)
{
    return c == ',';
}

bool Tokenizer::isArrayOpen(const int &c)
{
    return c == '{';
}

bool Tokenizer::isArrayClose(const int &c)
{
    return c == '}';
}

bool Tokenizer::isBlockOpen(const int &c)
{
    return c == '[';
}

bool Tokenizer::isBlockClose(const int &c)
{
    return c == ']';
}

bool Tokenizer::isExpressionEnd(const int &c)
{
    return c == ';';
}

bool Tokenizer::isReturn(const int &c)
{
    return c == '^';
}

bool Tokenizer::isVerticalBar(const int &c)
{
    return c == '|';
}

bool Tokenizer::isComment(const int &c)
{
    if(c == '/')
    {
        int toBeReturned = reader->read();
        int next = reader->peek();
        reader->putBack(toBeReturned);
        return next == '/';
    }
    return false;
}

std::string Tokenizer::readInteger()
{
    std::string val;
    bool neg = reader->peek() == '-';
    val += reader->read(); // might be minus
    int digits = 1;
    while(isDigit(reader->peek()))
    {
        val += (char) reader->read();
        digits++;
    }

    if(neg && digits == 1)
    {
        throw TokenizerException("[Tokenizer]: Only minus not allowed.");
    }

    return val;
}

std::string Tokenizer::readSymbol()
{
    std::string val;
    while(isSymbol(reader->peek()))
    {
        val += (char) reader->read();
    }

    return val;
}

std::string Tokenizer::readString()
{
    std::string val;
    reader->read(); // eat "
    bool escape = false;
    for(;;)
    {
        int c = reader->peek();

        if(!escape && c == '\\')
        {
            escape = true;
            reader->read();
            continue;
        }

        if(!escape && c == '\"')
        {
            reader->read();
            break;
        }

        if(c == EOF)
        {
            break;
        }

        if(escape)
        {
            switch(c)
            {
                case 'a':
                    c = '\a';
                    break;
                case 'b':
                    c = '\b';
                    break;
                case 'f':
                    c = '\f';
                    break;
                case 'n':
                    c = '\n';
                    break;
                case 'r':
                    c = '\r';
                    break;
                case 't':
                    c = '\t';
                    break;
                case 'v':
                    c = '\v';
                    break;
                case '?':
                    c = '\?';
                    break;
            }

            escape = false;
        }

        val += c;
        reader->read();
    }
    return val;
}

std::string Tokenizer::readCharacter()
{
    std::string val;
    reader->read(); // eat '
    for(;;)
    {
        int c = reader->peek();

        if(c == '\'')
        {
            reader->read();
            break;
        }

        if(c == EOF)
        {
            break;
        }

        val += c;
        reader->read();
        break;
    }
    if(reader->peek() == '\'')
    {
        reader->read();
    }
    else
    {
        throw TokenizerException("Invalid character format. Missing the ' probably.");
    }
    return val;
}

void Tokenizer::readAssignment()
{
    reader->read();
}

void Tokenizer::readParenthesisOpen()
{
    reader->read();
}

void Tokenizer::readParenthesisClose()
{
    reader->read();
}

void Tokenizer::readArraySeparator()
{
    reader->read();
}

void Tokenizer::readArrayOpen()
{
    reader->read();
}

void Tokenizer::readArrayClose()
{
    reader->read();
}

void Tokenizer::readBlockOpen()
{
    reader->read();
}

void Tokenizer::readBlockClose()
{
    reader->read();
}

void Tokenizer::readExpressionEnd()
{
    reader->read();
}

void Tokenizer::readVerticalBar()
{
    reader->read();
}

void Tokenizer::readReturn()
{
    reader->read();
}

void Tokenizer::readComment()
{
    reader->read();
    reader->read();
    while(reader->peek() != '\n' && reader->peek() != '\r' && reader->peek() != EOF)
    {
        if(reader->peek() == '\n') lineCounter++;
        reader->read();
    }
}

void Tokenizer::eatWhitespace()
{
    while(isWhitespace(reader->peek()))
    {
        int c = reader->read();
        if(c == '\n') lineCounter++;
    }
}

Tokenizer::Tokenizer(std::shared_ptr<Reader> reader)
    : reader(reader), reachedEOF(false), lineCounter(1)
{
}

Token Tokenizer::reallyReadToken()
{
    int c = 0;

    Token token(lineCounter, Token::Type::INVALID, "");

    do
    {
        c = reader->peek();

        if(isWhitespace(c))
        {
            eatWhitespace();
            continue;
        }

        if(isComment(c))
        {
            readComment();
            continue;
        }

        if(isDigitBegin(c))
        {
            token = Token(lineCounter, Token::Type::INTEGER, readInteger());
            break;
        }

        if(isSymbolStart(c))
        {
            token = Token(lineCounter, Token::Type::SYMBOL, readSymbol());
            break;
        }

        if(isString(c))
        {
            token = Token(lineCounter, Token::Type::STRING, readString());
            break;
        }

        if(isCharacter(c))
        {
            token = Token(lineCounter, Token::Type::CHARACTER, readCharacter());
            break;
        }

        if(isAssignment(c))
        {
            readAssignment();
            token = Token(lineCounter, Token::Type::ASSIGNMENT);
            break;
        }

        if(isParenthesisOpen(c))
        {
            readParenthesisOpen();
            token = Token(lineCounter, Token::Type::PARENTHESIS_OPEN);
            break;
        }

        if(isParenthesisClose(c))
        {
            readParenthesisClose();
            token = Token(lineCounter, Token::Type::PARENTHESIS_CLOSE);
            break;
        }

        if(isArraySeparator(c))
        {
            readArraySeparator();
            token = Token(lineCounter, Token::Type::ARRAY_SEPARATEOR);
            break;
        }

        if(isArrayOpen(c))
        {
            readArrayOpen();
            token = Token(lineCounter, Token::Type::ARRAY_OPEN);
            break;
        }

        if(isArrayClose(c))
        {
            readArrayClose();
            token = Token(lineCounter, Token::Type::ARRAY_CLOSE);
            break;
        }

        if(isBlockOpen(c))
        {
            readBlockOpen();
            token = Token(lineCounter, Token::Type::BLOCK_OPEN);
            break;
        }

        if(isBlockClose(c))
        {
            readBlockClose();
            token = Token(lineCounter, Token::Type::BLOCK_CLOSE);
            break;
        }

        if(isExpressionEnd(c))
        {
            readExpressionEnd();
            token = Token(lineCounter, Token::Type::EXPRESSION_END);
            break;
        }

        if(isReturn(c))
        {
            readReturn();
            token = Token(lineCounter, Token::Type::RETURN);
            break;
        }

        if(isVerticalBar(c))
        {
            readVerticalBar();
            token = Token(lineCounter, Token::Type::VERTICAL_BAR);
            break;
        }

        if(isEndOfFile(c))
        {
            reader->read();
            token = Token(lineCounter, Token::Type::TEOF);
            reachedEOF = true;
            break;
        }

#ifdef DEBUG
        std::cout << "<" << token.getValue() << ">" << std::endl;
#endif
        reader->read();
        return token;
    } while(true);

    return token;
}

Token Tokenizer::readToken()
{
    if(tokenBuffer.empty())
    {
        return reallyReadToken();
    }
    Token token = tokenBuffer.top();
    tokenBuffer.pop();
    return token;
}

void Tokenizer::putBackToken(Token token)
{
    tokenBuffer.push(token);
}

Token Tokenizer::peekToken()
{
    if(tokenBuffer.empty())
    {
        tokenBuffer.push(reallyReadToken());
    }
    return tokenBuffer.top();
}

bool Tokenizer::hasReachedEOF()
{
    return reachedEOF;
}
