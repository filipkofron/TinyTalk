#include "common.h"
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

    const char *allowedChars = "_+-/*^&~!";

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

bool Tokenizer::isBlockOpen(const int &c)
{
    return c == '[';
}

bool Tokenizer::isBlockClose(const int &c)
{
    return c == ']';
}

std::string Tokenizer::readInteger()
{
    std::string val;
    while(isDigit(reader->peek()))
    {
        val += (char) reader->read();
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
    for(;;)
    {
        int c = reader->peek();

        if(c == '\"')
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

void Tokenizer::readBlockOpen()
{
    reader->read();
}

void Tokenizer::readBlockClose()
{
    reader->read();
}

void Tokenizer::eatWhitespace()
{
    while(isWhitespace(reader->peek()))
    {
        reader->read();
    }
}

Tokenizer::Tokenizer(std::shared_ptr<Reader> reader)
    : reader(reader)
{
}

Token Tokenizer::readNextToken()
{
    //throw TokenizerException(std::string("readNextToken NOT IMPLEMENTED:") + std::string(__FILE__) + std::string(":") + std::to_string(__LINE__));

    int c = 0;

    Token token(Token::Type::INVALID, "");

    do
    {
        c = reader->peek();

        if(isWhitespace(c))
        {
            eatWhitespace();
            continue;
        }

        if(isDigit(c))
        {
            token = Token(Token::Type::INTEGER, readInteger());
            break;
        }

        if(isSymbolStart(c))
        {
            token = Token(Token::Type::SYMBOL, readSymbol());
            break;
        }

        if(isString(c))
        {
            token = Token(Token::Type::STRING, readString());
            break;
        }

        if(isCharacter(c))
        {
            token = Token(Token::Type::CHARACTER, readCharacter());
            break;
        }

        if(isAssignment(c))
        {
            readAssignment();
            token = Token(Token::Type::ASSIGNMENT);
            break;
        }

        if(isParenthesisOpen(c))
        {
            readParenthesisOpen();
            token = Token(Token::Type::PARENTHESIS_OPEN);
            break;
        }

        if(isParenthesisClose(c))
        {
            readParenthesisClose();
            token = Token(Token::Type::PARENTHESIS_CLOSE);
            break;
        }

        if(isBlockOpen(c))
        {
            readBlockOpen();
            token = Token(Token::Type::BLOCK_OPEN);
            break;
        }

        if(isBlockClose(c))
        {
            readBlockClose();
            token = Token(Token::Type::BLOCK_CLOSE);
            break;
        }

        if(isEndOfFile(c))
        {
            reader->read();
            token = Token(Token::Type::TEOF, "");
            break;
        }

#ifdef DEBUG
        std::cout << DEBUG_TOKENIZER_MSG << "just got '" << c << "'" << std::endl;
#endif
        reader->read();
        return token;
    } while(true);

    return token;
}
