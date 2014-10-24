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

TTLiteral *Parser::createLiteralArrayOfStrings(std::vector<std::string> &strs)
{
    std::vector<TTObject *> literals;
    for(auto str : strs)
    {
        TTLiteral *lit = TTLiteral::createStringLiteral(TO_TT_STR(str.c_str()));
        TTObject *obj = TTObject::createObject(TT_LITERAL);
        obj->setLiteral(lit);
        literals.push_back(obj);
    }

    return TTLiteral::createObjectArray(literals);
}

TTObject *Parser::parseRightOfValue(TTObject *destExpr, Token &prevToken)
{
    std::cout << "(parseRightOfValue)" << std::endl;
    // ----------------------------------------------------------------------- //

    switch (tokenizer->peekToken().getType())
    {
        case Token::Type::SYMBOL:
            // may be either message
            if (isSimpleMessage(tokenizer->peekToken()))
            {
                return parseSimpleMessageRest(destExpr);
            }
            else
            {
                return parseMultipleMessageRest(destExpr);
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
        case Token::Type::STRING:
            std::cerr << "[Parser]: cannot send a literal to object (not yet)" << std::endl;
            return NULL;
        case Token::Type::EXPRESSION_END:
            return parseChain(destExpr, false); // TODO: Is this correct to put it forward ?
        default:
            return destExpr;
    }
}

TTObject *Parser::parseSingleValue()
{
    Token token = tokenizer->peekToken();

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
        case Token::Type::BLOCK_OPEN:
            expr = parseBlock(true);
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
TTObject *Parser::parseSymbol(const bool &parseOnlyOne)
{
    std::cout << "(parseSymbol:"
            << (parseOnlyOne ? "parseOnce" : "parseAll") << ")" << std::endl;
    // ----------------------------------------------------------------------- //

    TTObject *result = NULL;

    Token symbolToken = tokenizer->readToken();

    TTLiteral *lit = TTLiteral::createStringLiteral(TO_TT_STR(symbolToken.getValue().c_str()));
    result = Expression::createSymbolValue(lit);

    if(parseOnlyOne)
    {
        return result;
    }
    else
    {
        return parseRightOfValue(result, symbolToken);
    }
}

TTObject *Parser::parseLiteral(const bool &parseOnlyOne)
{
    std::cout << "(parseLiteral:"
            << (parseOnlyOne ? "parseOnce" : "parseAll") << ")" << std::endl;
    // ----------------------------------------------------------------------- //

    TTObject *result = NULL;

    Token literalToken = tokenizer->readToken();

    TTLiteral *lit = TTLiteral::createStringLiteral(TO_TT_STR(literalToken.getValue().c_str()));
    result = Expression::createLiteralValue(lit);

    if(parseOnlyOne)
    {
        return result;
    }
    else
    {
        return parseRightOfValue(result, literalToken);
    }
}

TTObject *Parser::parseSimpleMessageRest(TTObject *destExpr)
{
    std::cout << "(parseSimpleMessageRest)" << std::endl;
    // ----------------------------------------------------------------------- //

    Token messageNameToken = tokenizer->readToken();

    TTLiteral *lit = TTLiteral::createStringLiteral(TO_TT_STR(messageNameToken.getValue().c_str()));
    TTObject *result = Expression::createSimpleMessage(destExpr, lit);

    /*

    TODO: Provide more from the type. The message name is symbol and therefore this will fail at runtime ..

     */
    return parseRightOfValue(result, messageNameToken);
}

TTObject *Parser::parseMultipleMessageRest(TTObject *destExpr)
{
    std::cout << "(parseMultipleMessageRest)" << std::endl;
    // ----------------------------------------------------------------------- //

    std::vector<std::string> argNames;
    std::vector<TTObject *> argValues;
    std::string fullName;

    uint32_t maxArgs = PARSER_MAX_METHOD_ARGS + 1;
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
            case Token::Type::BLOCK_CLOSE:
            case Token::Type::PARENTHESIS_CLOSE:
            case Token::Type::TEOF:
                atEnd = true;
                break;
            case Token::Type::EXPRESSION_END:
                tokenizer->readToken(); // eat it
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

        TTObject *expr = parseSingleValue();

        std::cout << "(mul msg val): got expr: " << expr << std::endl;

        argValues.push_back(expr);
    } while (--maxArgs);

    if(maxArgs == 0)
    {
        std::cerr << "[Parser]: Error: Too many arguments, max of 1024 supported as of yet." << std::endl;
        return NULL;
    }

    TTLiteral *fullNameLit = TTLiteral::createStringLiteral(TO_TT_STR(fullName.c_str()));

    TTLiteral *nameArrayLit = createLiteralArrayOfStrings(argNames);

    TTLiteral *valueArrayLit = TTLiteral::createObjectArray(argValues);

    return Expression::createMultipleMessage(destExpr, fullNameLit, nameArrayLit, valueArrayLit);
}

TTObject *Parser::parseBlock(const bool &parseOnlyOne)
{
    std::cout << "(parseBlock:"
            << (parseOnlyOne ? "parseOnce" : "parseAll") << ")" << std::endl;
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
                    std::cout << "[Parser]: Line:" << tokenizer->peekToken().getLine() << " Warning: Empty block argument list. (No reason to have the vertical bar)" << std::endl;
                }
                tokenizer->readToken();
                atEnd = true;
                break;
            case Token::Type::BLOCK_CLOSE:
                std::cerr << "[Parser]: Line:" << tokenizer->peekToken().getLine() << " Error: Block without body." << std::endl;
                return NULL;
            default:
                if(maxArgs != PARSER_MAX_METHOD_ARGS + 1)
                {
                    std::cerr << "[Parser]: Line:" << tokenizer->peekToken().getLine() << " Error: Invalid block argument." << std::endl;
                    return NULL;
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
        std::cerr << "[Parser]: Line:" << tokenizer->peekToken().getLine() << " Error: Empty block argument." << std::endl;
        return NULL;
    }

    std::cout << "Block loaded these arguments:";
    for(auto str : argNames)
    {
        std::cout << " '" << str << "'";
    }
    std::cout << std::endl;

    if(maxArgs == 0)
    {
        std::cerr << "[Parser]: Line:" << lineHolder.getLine() << " Error: Too many arguments, max of 1024 supported as of yet." << std::endl;
        return NULL;
    }

    TTObject *rightSideExpression = parse(false);

    if(rightSideExpression == NULL)
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

    TTLiteral *nameArray = createLiteralArrayOfStrings(argNames);
    TTLiteral *fullNameLit = TTLiteral::createStringLiteral(TO_TT_STR(fullName.c_str()));

    TTObject *res = Expression::createBlock(nameArray, fullNameLit, rightSideExpression);
    if(parseOnlyOne)
    {
        return res;
    }
    else
    {
        return parseRightOfValue(res, blockEnd);
    }
}

TTObject *Parser::parseAssignmentRest(const Token &token)
{
    std::cout << "(parseAssignmentRest)" << std::endl;
    // ----------------------------------------------------------------------- //

    Token assignToken = tokenizer->readToken(); // eat the assign token

    TTLiteral *lit = TTLiteral::createStringLiteral(TO_TT_STR(token.getValue().c_str()));

    TTObject* result = Expression::createAssignment(lit, parse(true)); // recursively parse the right side

    /*Token nextToken = tokenizer->peekToken();
    if(nextToken.getType() == Token::Type::EXPRESSION_END)
    {

    }*/
    // TODO: Is this correct after assign?

    return parseRightOfValue(result, assignToken);
}

TTObject *Parser::parseParenthesis(const bool &parseOnlyOne)
{
    std::cout << "(parseParenthesis:"
            << (parseOnlyOne ? "parseOnce" : "parseAll") << ")" << std::endl;
    // ----------------------------------------------------------------------- //

    Token lineHolder = tokenizer->readToken(); // eat opening bracket and store line number

    TTObject *expr = parse(false);

    Token closedBracket = tokenizer->readToken();
    if(closedBracket.getType() != Token::Type::PARENTHESIS_CLOSE)
    {
        std::cerr << "[Parser]: " << " Line: " << lineHolder.getLine()
                << ": Error: Parenthesis didn't end correctly" << std::endl;
        return NULL;
    }

    TTObject *result = Expression::createParenthesis(expr);
    if(parseOnlyOne)
    {
        return result;
    }
    else
    {
        return parseRightOfValue(result, closedBracket);
    }
}

TTObject *Parser::parseChain(TTObject *currExpr, const bool &parseOnlyOne)
{
    std::cout << "(parseChain)" << std::endl;
    // ----------------------------------------------------------------------- //

    tokenizer->readToken(); // eat ;

    TTObject *expr = Expression::createChained(currExpr, parse(parseOnlyOne));

    return expr;
}

TTObject *Parser::parseCreateVariable(const bool &parseOnlyOne)
{
    std::cout << "(parseCreateVariable:"
            << (parseOnlyOne ? "parseOnce" : "parseAll") << ")" << std::endl;
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

    TTLiteral *nameArray = createLiteralArrayOfStrings(names);
    TTObject *res = Expression::createCreateVariables(nameArray);

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

TTObject *Parser::parse(const bool &parseOnlyOne)
{
    Token token = tokenizer->peekToken();

    TTObject *res = NULL;

    switch(token.getType())
    {
        case Token::Type::SYMBOL:
            res = parseSymbol(parseOnlyOne);
            break;
        case Token::Type::INTEGER:
        case Token::Type::STRING:
            res = parseLiteral(parseOnlyOne);
            break;
        case Token::Type::PARENTHESIS_OPEN:
            res = parseParenthesis(parseOnlyOne);
            break;
        case Token::Type::BLOCK_OPEN:
            res = parseBlock(parseOnlyOne);
            break;
        case Token::Type::VERTICAL_BAR:
            res = parseCreateVariable(parseOnlyOne);
            break;
        case Token::Type::TEOF:
            res = TTObject::createObject(TT_NIL);
            break;
        default:
            std::cerr << "[Parser] Line: " << token.getLine() << ": Token " << token.getTypeInfo()
                    << " of value <" << token.getValue() << "> was unexpected here" << std::endl;
            throw std::exception();
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
