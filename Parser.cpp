#include "Parser.h"

Parser::Parser(std::shared_ptr<Tokenizer> tokenizer)
    : tokenizer(tokenizer)
{

}

TTObject *Parser::parse()
{
    TTObject *root = TTObject::createObject(TT_EXPR);

    Token token = tokenizer->readNextToken();

    switch(token.getType())
    {
        case Token::Type::PARENTHESIS_OPEN:
            break;
        case Token::Type::PARENTHESIS_CLOSE:
            break;
    }

    return root;
}
