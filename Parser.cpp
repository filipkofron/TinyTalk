#include "Parser.h"
#include "Expression.h"
#include "common.h"

Parser::Parser(std::shared_ptr<Tokenizer> tokenizer)
    : tokenizer(tokenizer)
{

}

bool Parser::isSimpleMessage(const Token &token)
{
    const std::string &val = token.getValue();
    if(val.size() < 1)
    {
        return false;
    }
    return val.c_str()[val.size() - 1] != ':';
}

/**
* We started reading symbol.
*
* Now we can get either message (simple, multiple),
* anything else will interrupt and the result will be symbol.
*/
TTObject *Parser::parseSymbol()
{
    TTObject *result = NULL;

    Token symbolToken = tokenizer->readToken();

    TTLiteral *lit = NULL;
    switch(tokenizer->peekToken().getType())
    {
        case Token::Type::SYMBOL:
            lit = TTLiteral::createStringLiteral(TO_TT_STR(symbolToken.getValue().c_str()));
            result = Expression::createSymbolValue(lit);
            // may be either message
            if(isSimpleMessage(tokenizer->peekToken()))
            {
                std::cout << "at symbol, found simple message" << std::endl;
                return parseSimpleMessageRest(result);
            }
            else
            {
                std::cout << "at symbol, found multiple message" << std::endl;
                return result;
            }
        case Token::Type::ASSIGNMENT:
            return parseAssignmentRest(symbolToken);
        default:
            lit = TTLiteral::createStringLiteral(TO_TT_STR(symbolToken.getValue().c_str()));
            result = Expression::createSymbolValue(lit);
            return result;
    }

}

TTObject *Parser::parseSimpleMessageRest(TTObject *destExpr)
{
    Token messageNameToken = tokenizer->readToken();

    TTLiteral *lit = TTLiteral::createStringLiteral(TO_TT_STR(messageNameToken.getValue().c_str()));
    TTObject *result = Expression::createSimpleMessage(destExpr, lit);

    switch(tokenizer->peekToken().getType())
    {
        case Token::Type::SYMBOL:
            // may be either message
            if(isSimpleMessage(tokenizer->peekToken()))
            {
                std::cout << "at message, found simple message" << std::endl;
                return parseSimpleMessageRest(result);
            }
            else
            {
                std::cout << "at message, found multiple message" << std::endl;
            }
        case Token::Type::ASSIGNMENT:
            std::cerr << "Parser: Error: cannot assign to message." << std::endl;
            return NULL;
        default:
            return result;
    }
}

TTObject *Parser::parseAssignmentRest(const Token &token)
{
    tokenizer->readToken(); // eat the assign token

    TTLiteral *lit = TTLiteral::createStringLiteral(TO_TT_STR(token.getValue().c_str()));

    return Expression::createAssignment(lit, parse()); // recursively parse the right side
}

TTObject *Parser::parseParenthesis()
{
    // TODO:
    throw std::exception();
}

TTObject *Parser::parse()
{
    Token token = tokenizer->peekToken();

    std::cout << "[Parser]: token: " << token.getTypeInfo() << std::endl;

    TTObject *res = NULL;

    switch(token.getType())
    {
        case Token::Type::SYMBOL:
            res = parseSymbol();
            break;
        case Token::Type::PARENTHESIS_OPEN:
            break;
        case Token::Type::PARENTHESIS_CLOSE:
            break;
    }

    if(!res)
    {
        // an error occures, gonna eat the next token
        std::cerr << "parse error, eating next token" << std::endl;
        tokenizer->readToken();
        throw std::exception();
    }

    return res;
}
