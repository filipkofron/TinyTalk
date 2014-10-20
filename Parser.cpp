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

std::string Parser::getMultipleMessageName(const Token &token)
{
    std::string str = token.getValue();
    if(str.size() > 0)
    {
        str.resize(str.size() - 1);
    }
    return str;
}

/**
* We started reading symbol.
*
* Now we can get either message (simple, multiple),
* anything else will interrupt and the result will be symbol.
*/
TTObject *Parser::parseSymbol(const bool &parseOnlyOne)
{
    TTObject *result = NULL;

    Token symbolToken = tokenizer->readToken();

    TTLiteral *lit = NULL;
    if(parseOnlyOne)
    {
        lit = TTLiteral::createStringLiteral(TO_TT_STR(symbolToken.getValue().c_str()));
        result = Expression::createSymbolValue(lit);
        return result;
    }
    else
    {
        switch (tokenizer->peekToken().getType())
        {
            case Token::Type::SYMBOL:
                lit = TTLiteral::createStringLiteral(TO_TT_STR(symbolToken.getValue().c_str()));
                result = Expression::createSymbolValue(lit);
                // may be either message
                if (isSimpleMessage(tokenizer->peekToken()))
                {
                    std::cout << "at symbol, found simple message" << std::endl;
                    return parseSimpleMessageRest(result);
                }
                else
                {
                    std::cout << "at symbol, found multiple message" << std::endl;
                    return parseMultipleMessageRest(result);
                }
            case Token::Type::ASSIGNMENT:
                return parseAssignmentRest(symbolToken);
            case Token::Type::INTEGER:
            case Token::Type::PARENTHESIS_OPEN:
                std::cerr << "[Parser]: cannot send the resutl of parenthesis to object (not yet)" << std::endl;
                return result;
            case Token::Type::STRING:
                std::cerr << "[Parser]: cannot send a literal to object (not yet)" << std::endl;
                return result;
            default:
                lit = TTLiteral::createStringLiteral(TO_TT_STR(symbolToken.getValue().c_str()));
                result = Expression::createSymbolValue(lit);
                return result;
        }
    }
}

TTObject *Parser::parseLiteral(const bool &parseOnlyOne)
{
    std::cout << "(ParseLiteral onlyone:" << parseOnlyOne << ")" << std::endl;
    TTObject *result = NULL;

    Token literalToken = tokenizer->readToken();

    TTLiteral *lit = NULL;

    if(parseOnlyOne)
    {
        lit = TTLiteral::createStringLiteral(TO_TT_STR(literalToken.getValue().c_str()));
        result = Expression::createLiteralValue(lit);
        return result;
    }
    else
    {
        switch (tokenizer->peekToken().getType())
        {
            case Token::Type::SYMBOL:
                lit = TTLiteral::createStringLiteral(TO_TT_STR(literalToken.getValue().c_str()));
                result = Expression::createLiteralValue(lit);
                // may be either message
                if (isSimpleMessage(tokenizer->peekToken()))
                {
                    std::cout << "at symbol, found simple message" << std::endl;
                    return parseSimpleMessageRest(result);
                }
                else
                {
                    std::cout << "at symbol, found multiple message TODO IMPLEMENT" << std::endl;
                    // TODO: implement
                    throw std::exception();
                    return result;
                }
            case Token::Type::ASSIGNMENT:
                std::cerr << "Parser: Error: cannot assign to literal." << std::endl;
                return NULL;
            default:
                lit = TTLiteral::createStringLiteral(TO_TT_STR(literalToken.getValue().c_str()));
                result = Expression::createLiteralValue(lit);
                return result;
        }
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
        case Token::Type::INTEGER:
        case Token::Type::STRING:
            std::cerr << "[Parser]: cannot send a literal to object returned by message (not yet)" << std::endl;
        case Token::Type::ASSIGNMENT:
            std::cerr << "Parser: Error: cannot assign to message." << std::endl;
            return NULL;
        default:
            return result;
    }
}

TTObject *Parser::parseMultipleMessageRest(TTObject *destExpr)
{
    std::vector<std::string> argNames;
    std::vector<TTObject *> argValues;
    std::string fullName;

    uint32_t maxArgs = 1024 + 1;
    bool atEnd = false;

    do
    {
        Token token = tokenizer->peekToken();

        std::cout << "token: " << token.getTypeInfo() << " - " << token.getValue() << std::endl;

        switch(token.getType())
        {
            case Token::Type::SYMBOL:
                if(isSimpleMessage(token))
                {
                    std::cerr << "[Parser]: Error: Multiple message can only store single value or parenthesis." << std::endl;
                    return NULL;
                }
                else
                {
                    tokenizer->readToken(); // eat it
                    std::string name = getMultipleMessageName(token);
                    argNames.push_back(name);
                    fullName += token.getValue();
                }
                break;
            case Token::Type::PARENTHESIS_CLOSE: // TODO: Is this neccessary ?
            case Token::Type::BLOCK_CLOSE: // TODO: Is this neccessary ?
            case Token::Type::TEOF:
                atEnd = true;
                break;
            case Token::Type::EXPRESSION_END:
                tokenizer->readToken(); // eat it
                atEnd = true;
                break;
            default:
                std::cerr << "[Parser]: Error: Multiple message unexpected token." << std::endl;
                return NULL;
        }

        if(atEnd)
        {
            break;
        }

        token = tokenizer->peekToken();

        TTObject *expr = NULL;
        switch(token.getType())
        {
            case Token::Type::SYMBOL:
                expr = parseSymbol(true);
                break;
            case Token::Type::STRING:
            case Token::Type::INTEGER:
                expr = parseLiteral(true);
                break;
            case Token::Type::PARENTHESIS_OPEN:
                expr = parseParenthesis(true);
                break;
            default:
                std::cerr << "[Parser]: Error: Multiple message unexpected token." << std::endl;
                return NULL;
        }
        std::cout << "(mul msg val): got expr: " << expr << std::endl;
        argValues.push_back(expr);
    } while (--maxArgs);

    if(maxArgs == 0)
    {
        std::cerr << "[Parser]: Error: Too many arguments, max of 1024 supported as of yet." << std::endl;
        return NULL;
    }

    TTLiteral *fullNameLit = TTLiteral::createStringLiteral(TO_TT_STR(fullName.c_str()));

    std::vector<TTObject *> nameLiterals;
    for(auto name : argNames)
    {
        TTLiteral *lit = TTLiteral::createStringLiteral(TO_TT_STR(name.c_str()));
        TTObject *obj = TTObject::createObject(TT_LITERAL);
        obj->setLiteral(lit);
        nameLiterals.push_back(obj);
    }
    TTLiteral *nameArrayLit = TTLiteral::createObjectArray(nameLiterals);

    TTLiteral *valueArrayLit = TTLiteral::createObjectArray(argValues);

    return Expression::createMultipleMessage(destExpr, fullNameLit, nameArrayLit, valueArrayLit);
}

TTObject *Parser::parseAssignmentRest(const Token &token)
{
    tokenizer->readToken(); // eat the assign token

    TTLiteral *lit = TTLiteral::createStringLiteral(TO_TT_STR(token.getValue().c_str()));

    return Expression::createAssignment(lit, parse()); // recursively parse the right side
}

TTObject *Parser::parseParenthesis(const bool &parseOnlyOne)
{
    tokenizer->readToken(); // eat opening bracket

    TTObject *expr = parse();

    Token closedBracket = tokenizer->readToken();
    if(closedBracket.getType() != Token::Type::PARENTHESIS_CLOSE)
    {
        std::cerr << "[Parser]: Error: Parenthesis didn't end correctly" << std::endl;
        return NULL;
    }

    TTObject *result = NULL;
    TTLiteral *lit = NULL;
    if(parseOnlyOne)
    {
        result = Expression::createParenthesis(expr);
        return result;
    }
    else
    {
        switch (tokenizer->peekToken().getType())
        {
            case Token::Type::SYMBOL:
                result = Expression::createParenthesis(expr);
                // may be either message
                if (isSimpleMessage(tokenizer->peekToken()))
                {
                    std::cout << "after parenthesis end, found simple message" << std::endl;
                    return parseSimpleMessageRest(result);
                }
                else
                {
                    std::cout << "after parenthesis end, found multiple message" << std::endl;
                    return parseMultipleMessageRest(result);
                }
            case Token::Type::ASSIGNMENT:
                std::cerr << "Parser: Error: cannot assign to result of parenthesis." << std::endl;
                return NULL;
            case Token::Type::INTEGER:
            case Token::Type::STRING:
                std::cerr << "[Parser]: cannot send a literal to object (not yet)" << std::endl;
                return result;
            default:
                result = Expression::createParenthesis(expr);
                return result;
        }
    }
}

TTObject *Parser::parse()
{
    Token token = tokenizer->peekToken();

    std::cout << "[Parser]: token: " << token.getTypeInfo() << std::endl;

    TTObject *res = NULL;

    switch(token.getType())
    {
        case Token::Type::SYMBOL:
            res = parseSymbol(false);
            break;
        case Token::Type::INTEGER:
        case Token::Type::STRING:
            res = parseLiteral(false);
            break;
        case Token::Type::PARENTHESIS_OPEN:
            res = parseParenthesis(false);
            break;
        case Token::Type::PARENTHESIS_CLOSE:
            break;
    }

    if(!res && token.getType() != Token::Type::TEOF)
    {
        // an error occures, gonna eat the next token
        std::cerr << "parse error, eating next token" << std::endl;
        tokenizer->readToken();
        throw std::exception();
    }

    return res;
}
