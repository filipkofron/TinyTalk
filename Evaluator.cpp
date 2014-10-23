#include "Evaluator.h"
#include "Expression.h"
#include "common.h"
#include <cstring>

TTObject *Evaluator::sendSimpleMessageToNonExpression(std::string &simpleMessageName, TTObject *dest, TTObject *env)
{
    std::cout << "(sendSimpleMessageToNonExpression)" << std::endl;

    TTObject *field = dest->getField(TO_TT_STR(simpleMessageName.c_str()));

    if(field)
    {
        return executeSimpleMessage(field, simpleMessageName, env);
    }
    else
    {
        std::cerr << "Eval: Sorry, object doesn't understand!!!" << std::endl;
        return TTObject::createObject(TT_NIL);
    }
}

TTObject *Evaluator::executeSimpleMessage(TTObject *expression, std::string &msgName, TTObject *env)
{
    std::cout << "(executeSimpleMessage)" << std::endl;

    std::string name = (char *) expression->getField(TO_TT_STR("blockFullName"))->getLiteral()->data;
    if(COMPARE_NAME(msgName.c_str(), name.c_str()) == 0)
    {
        TTObject *blockEnv = expression->getField(TO_TT_STR("blockEnv"));
        TTObject *blockExpr = expression->getField(TO_TT_STR("blockExpr"));
        TTObject *newEnv = TTObject::createObject(TT_ENV);
        newEnv->addField(TO_TT_STR("parent"), blockEnv);

        /**
        * Todo: execute bytecode or native code .. or builtin function. LOL
        */

        return evaluate(blockExpr, blockEnv);
    }
    else
    {
        return sendSimpleMessageToNonExpression(msgName, expression, env);
    }
}

TTObject *Evaluator::evaluateSimpleMessage(TTObject *simpleMessage, TTObject *env)
{
    std::cout << "(evaluateSimpleMessage)" << std::endl;
    TTObject *destExpr = simpleMessage->getField(TO_TT_STR("destExpr"));
    TTObject *destValue = evaluate(destExpr, env);

    std::string msgName = (char *) simpleMessage->getField(TO_TT_STR("msgName"))->getLiteral()->data;

    if(!destValue)
    {
        std::cerr << "(evaluateSimpleMessage): destValue is NULL!" << std::endl;
        return TTObject::createObject(TT_NIL);
    }

    if(destValue->type == TT_EXPR)
    {
        if(destValue->flags == EXPRESSION_BLOCK)
        {
            return executeSimpleMessage(destValue, msgName, env);
        }
        else
        {
            return sendSimpleMessageToNonExpression(msgName, destValue, env);
        }
    }
    else
    {
        return sendSimpleMessageToNonExpression(msgName, destValue, env);
    }
}

/**
* Evaluates expression of symbol value.
*
* This will look into the environment for the symbol value
* by the given name and if this fails, it tries to find it
* in the this, which corresponds to an object the message
* is currently being answered by.
*/
TTObject *Evaluator::evaluateSymbolValue(TTObject *symbolValue, TTObject *env)
{
    std::cout << "(evaluateSymbolValue)" << std::endl;

    uint8_t *name = symbolValue->getField(TO_TT_STR("symbolName"))->getLiteral()->data;

    TTObject *resultFromEnv = env->getField(name);
    if(!resultFromEnv)
    {
        std::cout << "(evaluateSymbolValue): field not found in environment" << std::endl;
    }
    else
    {
        return resultFromEnv;
    }

    TTObject *thiz = env->getField(TO_TT_STR("this"));
    if(!thiz)
    {
        std::cout << "(evaluateSymbolValue): this is NULL" << std::endl;
        return TTObject::createObject(TT_NIL);
    }
    else
    {
        TTObject *resultFromThis = thiz->getField(name);
        if(!resultFromThis)
        {
            std::cout << "(evaluateSymbolValue): field not found in this" << std::endl;
            return TTObject::createObject(TT_NIL);
        }
        else
        {
            std::cout << "(evaluateSymbolValue): field was found in this" << std::endl;
            return resultFromThis;
        }
    }
}

TTObject *Evaluator::evaluateBlock(TTObject *block, TTObject *env)
{
    block->addField(TO_TT_STR("blockEnv"), env);
    return block;
}

TTObject *Evaluator::evaluate(TTObject *expression, TTObject *env)
{
    std::cout << "(evaluate)" << std::endl;
    TTObject *res = NULL;

    if(!expression)
    {
        return res;
    }

    if(expression->type != TT_EXPR)
    {
        return res;
    }

    switch (expression->flags)
    {
        case EXPRESSION_SYMBOL_VALUE:
            res = evaluateSymbolValue(expression, env);
            break;
        case EXPRESSION_SIMPLE_MESSAGE:
            res = evaluateSimpleMessage(expression, env);
            break;
        case EXPRESSION_ASSIGN:
            break;
        case EXPRESSION_CREATE_VARIABLES:
            break;
        case EXPRESSION_BLOCK:
            break;
        case EXPRESSION_CHAINED:
            break;
        case EXPRESSION_LITERAL_VALUE:
            break;
        case EXPRESSION_MULTIPLE_MESSAGE:
            break;
        case EXPRESSION_PARENTHESIS:
            break;
    }

    return res;
}
