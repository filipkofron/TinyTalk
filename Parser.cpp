#include "TTLiteral.h"
#include "Parser.h"
#include "Expression.h"
#include "Common.h"
#include "Runtime.h"

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

bool Parser::isBlockArgument(const Token &token)
{
    const std::string &str = token.getValue();
    if(str.size() < 1)
    {
        return false;
    }
    return str.c_str()[0] == ':';
}

std::string Parser::getBlockArgumentName(const Token &token)
{
    const std::string &str = token.getValue();
    if(str.size() < 1)
    {
        return "";
    }
    return str.substr(1, str.size() - 1);
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

RefPtr<TTLiteral> Parser::createLiteralArrayOfStrings(std::vector<std::string> &strs)
{
    std::vector<RefPtr<TTObject> > literals;
    for(auto str : strs)
    {
        RefPtr<TTObject> obj = TTLiteral::createStringLiteral(TO_TT_STR(str.c_str()));

        literals.push_back(obj);
    }

    return TTLiteral::createObjectArray(literals)->getLiteral();
}

RefPtr<TTObject> Parser::parseRightOfValue(RefPtr<TTObject> destExpr, Token &prevToken, const bool &untilChain)
{
#ifdef DEBUG
    std::cout << "(parseRightOfValue)" << std::endl;
#endif
    // ----------------------------------------------------------------------- //

    switch (tokenizer->peekToken().getType())
    {
        case Token::Type::SYMBOL:
            // may be either message
            if (isSimpleMessage(tokenizer->peekToken()))
            {
                return parseSimpleMessageRest(destExpr, untilChain);
            }
            else
            {
                return parseMultipleMessageRest(destExpr, untilChain);
            }
        case Token::Type::ASSIGNMENT:
            if(prevToken.getType() != Token::Type::SYMBOL)
            {
                std::cerr << "[Parser]: Line: " << tokenizer->peekToken().getLine()
                        << ": Error: Cannot assign to non-symbol!" << std::endl;
                return NULL;
            }
            return parseAssignmentRest(prevToken);
        case Token::Type::INTEGER:
        case Token::Type::PARENTHESIS_OPEN:
            std::cerr << "[Parser]: cannot send the resutl of parenthesis to object (not yet)" << std::endl;
            return NULL;
        case Token::Type::ARRAY_OPEN:
            std::cerr << "[Parser]: cannot send array to object yet" << std::endl;
                return NULL;
        case Token::Type::STRING:
            std::cerr << "[Parser]: cannot send a literal to object (not yet)" << std::endl;
            return NULL;
        case Token::Type::EXPRESSION_END:
            if(untilChain)
                return destExpr;
            else
                return parseChain(destExpr, false); // TODO: Is this correct to put it forward ?
        default:
            return destExpr;
    }
}

RefPtr<TTObject> Parser::parseSingleValue()
{
    Token token = tokenizer->peekToken();

    RefPtr<TTObject> expr = NULL;
    switch(token.getType())
    {
        case Token::Type::SYMBOL:
            expr = parseSymbol(true, false);
            break;
        case Token::Type::STRING:
        case Token::Type::INTEGER:
            expr = parseLiteral(true, false);
            break;
        case Token::Type::PARENTHESIS_OPEN:
            expr = parseParenthesis(true, false);
            break;
        case Token::Type::ARRAY_OPEN:
            expr = parseArray(true, false);
            break;
        case Token::Type::BLOCK_OPEN:
            expr = parseBlock(true, false);
            break;
        default:
            std::cerr << "[Parser]: Error: Single value expected only." << std::endl;
            return NULL;
    }

    return expr;
}

/**
* We started reading symbol.
*
* Now we can get either message (simple, multiple),
* anything else will interrupt and the result will be symbol.
*/
RefPtr<TTObject> Parser::parseSymbol(const bool &parseOnlyOne, const bool &untilChain)
{
#ifdef DEBUG
    std::cout << "(parseSymbol:"
            << (parseOnlyOne ? "parseOnce" : "parseAll") << ")" << std::endl;
#endif
    // ----------------------------------------------------------------------- //

    RefPtr<TTObject> result = NULL;

    Token symbolToken = tokenizer->readToken();

    RefPtr<TTLiteral> lit = TTLiteral::createStringLiteral(TO_TT_STR(symbolToken.getValue().c_str()))->getLiteral();
    result = Expression::createSymbolValue(lit);

    if(parseOnlyOne)
    {
        return result;
    }
    else
    {
        return parseRightOfValue(result, symbolToken, untilChain);
    }
}

RefPtr<TTObject> Parser::parseLiteral(const bool &parseOnlyOne, const bool &untilChain)
{
#ifdef DEBUG
    std::cout << "(parseLiteral:"
            << (parseOnlyOne ? "parseOnce" : "parseAll") << ")" << std::endl;
#endif
    // ----------------------------------------------------------------------- //

    RefPtr<TTObject> result = NULL;

    Token literalToken = tokenizer->readToken();

    if(literalToken.getType() == Token::Type::INTEGER)
    {
        int32_t a = 0;
        sscanf(literalToken.getValue().c_str(), "%d", &a);
        RefPtr<TTLiteral> lit = TTLiteral::createIntegerLiteral(a)->getLiteral();
        result = Expression::createLiteralValue(lit);
    }
    else
    {
        RefPtr<TTLiteral> lit = TTLiteral::createStringLiteral(TO_TT_STR(literalToken.getValue().c_str()))->getLiteral();
        result = Expression::createLiteralValue(lit);
    }



    if(parseOnlyOne)
    {
        return result;
    }
    else
    {
        return parseRightOfValue(result, literalToken, untilChain);
    }
}

RefPtr<TTObject> Parser::parseSimpleMessageRest(RefPtr<TTObject> destExpr, const bool &untilChain)
{
#ifdef DEBUG
    std::cout << "(parseSimpleMessageRest)" << std::endl;
#endif
    // ----------------------------------------------------------------------- //

    Token messageNameToken = tokenizer->readToken();

    RefPtr<TTLiteral> lit = TTLiteral::createStringLiteral(TO_TT_STR(messageNameToken.getValue().c_str()))->getLiteral();
    RefPtr<TTObject> result = Expression::createSimpleMessage(destExpr, lit);

    /*

    TODO: Provide more from the type. The message name is symbol and therefore this will fail at runtime ..

     */
    return parseRightOfValue(result, messageNameToken, untilChain);
}

RefPtr<TTObject> Parser::parseMultipleMessageRest(RefPtr<TTObject> destExpr, const bool &untilChain)
{
#ifdef DEBUG
    std::cout << "(parseMultipleMessageRest)" << std::endl;
#endif
    // ----------------------------------------------------------------------- //

    std::vector<std::string> argNames;
    std::vector<RefPtr<TTObject> > argValues;
    std::string fullName;

    bool finishedWithExpressionEnd = false;

    uint32_t maxArgs = PARSER_MAX_METHOD_ARGS + 1;
    bool atEnd = false;

    do
    {
        Token token = tokenizer->peekToken();

#ifdef DEBUG
        std::cout << "token: " << token.getTypeInfo() << " - " << token.getValue() << std::endl;
#endif

        switch(token.getType())
        {
            case Token::Type::SYMBOL:
                if(isSimpleMessage(token))
                {
                    std::cerr << "[Parser]: Line: " << token.getLine() << " Error: Multiple message can only store single value or parenthesis." << std::endl;
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
            case Token::Type::BLOCK_CLOSE:
            case Token::Type::PARENTHESIS_CLOSE:
            case Token::Type::ARRAY_CLOSE:
            case Token::Type::TEOF:
                atEnd = true;
                break;
            case Token::Type::EXPRESSION_END:
                finishedWithExpressionEnd = true;
                atEnd = true;
                break;
            default:
                std::cerr << "[Parser]: Line: " << token.getLine()
                        << ": Error: Multiple message unexpected token:" << token.getTypeInfo()
                        << " of value: " << token.getValue() << std::endl;
                return NULL;
        }

        if(atEnd)
        {
            break;
        }

        RefPtr<TTObject> expr = parseSingleValue();

#ifdef DEBUG
        std::cout << "(mul msg val): got expr: " << expr << std::endl;
#endif

        argValues.push_back(expr);
    } while (--maxArgs);

    if(maxArgs == 0)
    {
        std::cerr << "[Parser]: Error: Too many arguments, max of 1024 supported as of yet." << std::endl;
        return NULL;
    }

    RefPtr<TTLiteral> fullNameLit = TTLiteral::createStringLiteral(TO_TT_STR(fullName.c_str()))->getLiteral();

    RefPtr<TTLiteral> nameArrayLit = createLiteralArrayOfStrings(argNames);

    RefPtr<TTLiteral> valueArrayLit = TTLiteral::createObjectArray(argValues)->getLiteral();

    RefPtr<TTObject> res = Expression::createMultipleMessage(destExpr, fullNameLit, nameArrayLit, valueArrayLit);

    if(finishedWithExpressionEnd)
    {
        if(untilChain)
        {
            return res;
        }
        return parseChain(res, false);
    }
    return res;
}

RefPtr<TTObject> Parser::parseBlock(const bool &parseOnlyOne, const bool &untilChain)
{
#ifdef DEBUG
    std::cout << "(parseBlock:"
            << (parseOnlyOne ? "parseOnce" : "parseAll") << ")" << std::endl;
#endif
    // ----------------------------------------------------------------------- //

    Token lineHolder = tokenizer->readToken(); // eat [ and store token for line number

    uint32_t maxArgs = PARSER_MAX_METHOD_ARGS + 1;

    std::vector<std::string> argNames;
    std::string fullName;

    bool atEnd = false;
    do
    {
        switch (tokenizer->peekToken().getType())
        {
            case Token::Type::SYMBOL:
                if(isBlockArgument(tokenizer->peekToken()))
                {
                    Token nameToken = tokenizer->readToken();
                    std::string name = getBlockArgumentName(nameToken);
                    fullName += name; // save the argument without removing : if present


                    Token tempToken(0, Token::Type::STRING, name); // create temp token with name
                    // if it has : at the end, strip it
                    if(!isSimpleMessage(tempToken))
                    {
                        name = getMultipleMessageName(tempToken);
                    }
                    if(name.size() == 0)
                    {
                        std::cerr << "[Parser]: Line:" << tokenizer->peekToken().getLine() << " Error: Too short block argument name." << std::endl;
                        return NULL;
                    }
                    argNames.push_back(name); // eat out the token
                }
                else
                {
                    if(maxArgs != PARSER_MAX_METHOD_ARGS + 1)
                    {
                        std::cerr << "[Parser]: Line:" << tokenizer->peekToken().getLine() << " Error: Invalid block argument." << std::endl;
                        return NULL;
                    }
                    atEnd = true;
                }
                break;
            case Token::Type::VERTICAL_BAR:
                if(maxArgs == PARSER_MAX_METHOD_ARGS + 1)
                {
#ifdef DEBUG
                    std::cout << "[Parser]: Line:" << tokenizer->peekToken().getLine() << " TODO: Vertical bar at the beginning, is this local var init :) ?" << std::endl;
#endif
                    atEnd = true;
                    break;
                }
                // end of var declaration
                tokenizer->readToken();
                atEnd = true;
                break;
            case Token::Type::BLOCK_CLOSE:
                std::cerr << "[Parser]: Line:" << tokenizer->peekToken().getLine() << " Error: Block without body." << std::endl;
                KILL;
                //return NULL;
            default:
                if(maxArgs != PARSER_MAX_METHOD_ARGS + 1)
                {
                    std::cerr << "[Parser]: Line:" << tokenizer->peekToken().getLine() << " Error: Invalid block argument." << std::endl;
                    KILL;
                    //return NULL;
                }
                atEnd = true;
                break;
        }
        if(atEnd)
        {
            break;
        }
    } while(--maxArgs);

    if(maxArgs == PARSER_MAX_METHOD_ARGS + 1)
    {
#ifdef DEBUG
        std::cout << "[Parser]: Line:" << tokenizer->peekToken().getLine() << " Empty block argument." << std::endl;
#endif
    }

#ifdef DEBUG
    std::cout << "Block loaded these arguments:";
    for(auto str : argNames)
    {
        std::cout << " '" << str << "'";
    }
    std::cout << std::endl;
#endif

    if(maxArgs == 0)
    {
        std::cerr << "[Parser]: Line:" << lineHolder.getLine() << " Error: Too many arguments, max of 1024 supported as of yet." << std::endl;
        return NULL;
    }

    RefPtr<TTObject> rightSideExpression = parse(false, false);

    if(&rightSideExpression == NULL)
    {
        std::cerr << "[Parser]: Line:" << lineHolder.getLine() << " the block expression is invalid." << std::endl;
        return NULL;
    }

    Token blockEnd = tokenizer->readToken();
    if(blockEnd.getType() != Token::Type::BLOCK_CLOSE)
    {
        std::cerr << "[Parser]: Line:" << blockEnd.getLine()
                << " the block hasn't ended correctly, a token: "
                << blockEnd.getTypeInfo() << " of value " << blockEnd.getValue() << std::endl;
        return NULL;
    }

    RefPtr<TTLiteral> nameArray = createLiteralArrayOfStrings(argNames);
    RefPtr<TTLiteral> fullNameLit = TTLiteral::createStringLiteral(TO_TT_STR(fullName.c_str()))->getLiteral();

    RefPtr<TTObject> res = Expression::createBlock(nameArray, fullNameLit, rightSideExpression, NULL);
    if(parseOnlyOne)
    {
        return res;
    }
    else
    {
        return parseRightOfValue(res, blockEnd, untilChain);
    }
}

RefPtr<TTObject> Parser::parseAssignmentRest(const Token &token)
{
#ifdef DEBUG
    std::cout << "(parseAssignmentRest)" << std::endl;
#endif
    // ----------------------------------------------------------------------- //

    Token assignToken = tokenizer->readToken(); // eat the assign token

    RefPtr<TTLiteral> lit = TTLiteral::createStringLiteral(TO_TT_STR(token.getValue().c_str()))->getLiteral();

    RefPtr<TTObject> result = Expression::createAssignment(lit, parse(false, true)); // recursively parse the right side

    Token nextToken = tokenizer->peekToken();
    if(nextToken.getType() == Token::Type::EXPRESSION_END)
    {
        return parseChain(result, false);
    }
    // TODO: Is this correct after assign?

    return parseRightOfValue(result, assignToken, true);
}

RefPtr<TTObject> Parser::parseParenthesis(const bool &parseOnlyOne, const bool &untilChain)
{
#ifdef DEBUG
    std::cout << "(parseParenthesis:"
            << (parseOnlyOne ? "parseOnce" : "parseAll") << ")" << std::endl;
#endif
    // ----------------------------------------------------------------------- //

    Token lineHolder = tokenizer->readToken(); // eat opening bracket and store line number

    RefPtr<TTObject> expr = parse(false, untilChain);

    Token closedBracket = tokenizer->readToken();
    if(closedBracket.getType() != Token::Type::PARENTHESIS_CLOSE)
    {
        std::cerr << "[Parser]: " << " Line: " << lineHolder.getLine()
                << ": Error: Parenthesis didn't end correctly" << std::endl;
        return NULL;
    }

    RefPtr<TTObject> result = Expression::createParenthesis(expr);
    if(parseOnlyOne)
    {
        return result;
    }
    else
    {
        return parseRightOfValue(result, closedBracket, untilChain);
    }
}

RefPtr<TTObject> Parser::parseArray(const bool &parseOnlyOne, const bool &untilChain)
{
    Token lineHolder = tokenizer->readToken();

    std::vector<RefPtr<TTObject> > expressions;

    while(tokenizer->peekToken().getType() != Token::Type::ARRAY_CLOSE)
    {
        RefPtr<TTObject> expr = parse(false, false);
        expressions.push_back(expr);

        switch(tokenizer->peekToken().getType())
        {
            case Token::Type::ARRAY_SEPARATEOR:
                tokenizer->readToken();
                break;
            case Token::Type::ARRAY_CLOSE:
                break;
            default:
                std::cerr << "[Parser]: " << " Line: " << lineHolder.getLine()
                        << ": Error: Array didn't end correctly" << std::endl;
                return NULL;
        }
    }

    Token close = tokenizer->readToken();

    RefPtr<TTLiteral> lit = TTLiteral::createObjectArray(expressions)->getLiteral();
    RefPtr<TTObject> result = Expression::createArray(lit);

    if(parseOnlyOne)
    {
        return result;
    }
    return parseRightOfValue(result, close, untilChain);
}

RefPtr<TTObject> Parser::parseChain(RefPtr<TTObject> currExpr, const bool &parseOnlyOne)
{
#ifdef DEBUG
    std::cout << "(parseChain)" << std::endl;
#endif
    // ----------------------------------------------------------------------- //

    tokenizer->readToken(); // eat ;

    RefPtr<TTObject> expr = Expression::createChained(currExpr, parse(parseOnlyOne, false));

    return expr;
}

RefPtr<TTObject> Parser::parseCreateVariable(const bool &parseOnlyOne, const bool &untilChain)
{
#ifdef DEBUG
    std::cout << "(parseCreateVariable:"
            << (parseOnlyOne ? "parseOnce" : "parseAll") << ")" << std::endl;
#endif
    // ----------------------------------------------------------------------- //

    tokenizer->readToken(); // eat |

    bool atEnd = false;

    std::vector<std::string> names;

    do
    {
        Token token = tokenizer->peekToken();
        switch(token.getType())
        {
            case Token::Type::SYMBOL:
                names.push_back(token.getValue());
                tokenizer->readToken(); // eat it
                break;
            case Token::Type::VERTICAL_BAR:
                atEnd = true;
                tokenizer->readToken(); // eat it
                break;
            default:
                std::cerr << "[Parser] Line: " << token.getLine() << ": Error: Token "
                        << token.getTypeInfo() << " of value <" << token.getValue()
                        << "> was unexpected in variable declaration." << std::endl;
                return NULL;
        }
    } while(!atEnd);

    RefPtr<TTLiteral> nameArray = createLiteralArrayOfStrings(names);
    RefPtr<TTObject> res = Expression::createCreateVariables(nameArray);

    if(parseOnlyOne)
    {
        return res;
    }
    else
    {
        Token nextToken = tokenizer->peekToken();
        if(nextToken.getType() == Token::Type::EXPRESSION_END)
        {
            return parseChain(res, false);
        }
        else
        {
            if(nextToken.getType() == Token::Type::BLOCK_CLOSE)
            {
                return res;
            }
            else
            {
                std::cerr << "[Parser] Line: " << nextToken.getLine() << ": Error: Token "
                        << nextToken.getTypeInfo() << " of value <" << nextToken.getValue()
                        << "> after variable declaration may only be ; or ]." << std::endl;
                return NULL;
            }
        }
    }
}

RefPtr<TTObject> Parser::parse(const bool &parseOnlyOne, const bool &untilChain)
{
    Token token = tokenizer->peekToken();

    RefPtr<TTObject> res = NULL;

    switch(token.getType())
    {
        case Token::Type::SYMBOL:
            res = parseSymbol(parseOnlyOne, untilChain);
            break;
        case Token::Type::INTEGER:
        case Token::Type::STRING:
            res = parseLiteral(parseOnlyOne, untilChain);
            break;
        case Token::Type::PARENTHESIS_OPEN:
            res = parseParenthesis(parseOnlyOne, untilChain);
            break;
        case Token::Type::ARRAY_OPEN:
            res = parseArray(parseOnlyOne, untilChain);
            break;
        case Token::Type::BLOCK_OPEN:
            res = parseBlock(parseOnlyOne, untilChain);
            break;
        case Token::Type::VERTICAL_BAR:
            res = parseCreateVariable(parseOnlyOne, untilChain);
            break;
        case Token::Type::TEOF:
            res = Runtime::globalEnvironment->getField(TO_TT_STR("nil"));
            break;
        default:
            std::cerr << "[Parser] Line: " << token.getLine() << ": Token " << token.getTypeInfo()
                    << " of value <" << token.getValue() << "> was unexpected here" << std::endl;
            KILL;
    }

    if(!&res && token.getType() != Token::Type::TEOF)
    {
        // an error occures, gonna eat the next token
        std::cerr << "parse error, eating next token" << std::endl;
        tokenizer->readToken();
        KILL;
    }

    return res;
}
