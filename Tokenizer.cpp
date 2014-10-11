#include "Tokenizer.h"
#include "TokenizerException.h"

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

bool Tokenizer::isDigit(const int &c)
{
    return c >= '0' && c <= '9';
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
        c = reader->read();
        switch(c)
        {
            case EOF:
                return token;
        }

    } while(true);

    return token;
}
