#include "Evaluator.h"
#include "Expression.h"
#include "common.h"
#include "Runtime.h"
#include <cstring>
#include "Runtime.h"

TTObject *Evaluator::sendSimpleMessageToNonExpression(std::string &simpleMessageName, TTObject *dest, TTObject *env)
{
    std::cout << "(sendSimpleMessageToNonExpression)" << std::endl;

    TTObject *field = dest->getField(TO_TT_STR(simpleMessageName.c_str()));

    if(field)
    {
        if(field->type != TT_EXPR)
        {
            std::cerr << "Eval: Sorry, value is not a method: " << simpleMessageName << " !!!" << std::endl;
        }

        return executeSimpleMessage(field, simpleMessageName, env, dest);
    }
    else
    {
        std::cerr << "Eval: Sorry, object doesn't understand message: " << simpleMessageName << " !!!" << std::endl;
        return TTObject::createObject(TT_NIL);
    }
}

TTObject *Evaluator::executeSimpleMessage(TTObject *expression, std::string &msgName, TTObject *env, TTObject *thiz)
{
    std::cout << "(executeSimpleMessage) of name=" << msgName << std::endl;

    std::string name = (char *) expression->getField(TO_TT_STR("blockFullName"))->getLiteral()->data;

    std::cout << "(executeSimpleMessage) with block name=" << name << " empty: " << name.empty() << std::endl;

    bool anon = COMPARE_NAME(msgName.c_str(), "value") == 0 && name.empty();

    if(COMPARE_NAME(msgName.c_str(), name.c_str()) == 0 || anon)
    {
        TTObject *blockEnv = expression->getField(TO_TT_STR("blockEnv"));
        TTObject *blockExpr = expression->getField(TO_TT_STR("blockExpr"));
        TTObject *newEnv = TTObject::createObject(TT_ENV);
        TTObject *nativeName = expression->getField(TO_TT_STR("blockNativeName"));
        newEnv->addField(TO_TT_STR("parent"), blockEnv);

        if(thiz)
        {
            newEnv->addField(TO_TT_STR("this"), thiz);
        }

        /**
        * Todo: execute bytecode or native code .. LOL
        */

        if(nativeName)
        {
            TTLiteral *lit = nativeName->getLiteral();
            std::string strName = (char *) lit->data;

            std::shared_ptr<Builtin> builtin = Runtime::builtinPool.lookupBultin(strName);
            std::vector<std::string> singleList;
            singleList.push_back(msgName);
            if(builtin)
            {
                std::cout << "(executeSimpleMessage): Builtin function '" << strName << "' found." << std::endl;
                if(thiz)
                {
                    return builtin->invoke(thiz, singleList, std::vector<TTObject *>());
                }
                return builtin->invoke(env, singleList, std::vector<TTObject *>());
            }
            else
            {
                std::cerr << "(executeSimpleMessage): Builtin function '" << strName << "' not found!" << std::endl;
                throw std::exception();
            }
        }

        return evaluate(blockExpr, newEnv);
    }
    else
    {
        if(anon)
        {
            std::cerr << "(executeSimpleMessage): Cannot evaluate anonymous block." << std::endl;
            return NULL;
        }
        return sendSimpleMessageToNonExpression(msgName, expression, env);
    }
}

TTObject *Evaluator::evaluateSimpleMessage(TTObject *simpleMessage, TTObject *env)
{
    std::cout << "(evaluateSimpleMessage)" << std::endl;

    std::string msgName = (char *) simpleMessage->getField(TO_TT_STR("msgName"))->getLiteral()->data;
    TTObject *destExpr = simpleMessage->getField(TO_TT_STR("msgDestExpr"));

    if(!destExpr)
    {
        std::cerr << "(evaluateSimpleMessage): destExpr is NULL (message name: "
                << msgName << ")!" << std::endl;
        return TTObject::createObject(TT_NIL);
    }

    TTObject *destValue = evaluate(destExpr, env);

    if(!destValue)
    {
        std::cerr << "(evaluateSimpleMessage): destValue is NULL!" << std::endl;
        return TTObject::createObject(TT_NIL);
    }

    if(destValue->type == TT_EXPR)
    {
        if(destValue->flags == EXPRESSION_BLOCK)
        {
            return executeSimpleMessage(destValue, msgName, env, NULL);
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

    std::string name = (char *) symbolValue->getField(TO_TT_STR("symbolName"))->getLiteral()->data;

    std::cout << "(evaluateSymbolValue): of name '" << name << "'" << std::endl;

    TTObject *resultFromEnv = env->getField(TO_TT_STR(name.c_str()));
    if (!resultFromEnv)
    {
        std::cout << "(evaluateSymbolValue): field name '"
                << name << "' not found in environment" << std::endl;
    }
    else
    {
        std::cout << "(evaluateSymbolValue): field name '"
                << name << "' found in environment" << std::endl;

        std::cout << "res: " << resultFromEnv << std::endl;

        return resultFromEnv;
    }

    TTObject *thiz = env->getField(TO_TT_STR("this"));
    if (!thiz)
    {
        std::cout << "(evaluateSymbolValue): this is NULL" << std::endl;
    }
    else
    {
        TTObject *resultFromThis = thiz->getField(TO_TT_STR(name.c_str()));
        if (!resultFromThis)
        {
            std::cout << "(evaluateSymbolValue): field not found in this" << std::endl;
        }
        else
        {
            std::cout << "(evaluateSymbolValue): field was found in this" << std::endl;
            return resultFromThis;
        }
    }
    TTObject *parent = env->getField(TO_TT_STR("parent"));
    if (parent && parent->type == TT_ENV)
    {
        std::cout << "(evaluateSymbolValue): trying parent env" << std::endl;
        return evaluateSymbolValue(symbolValue, parent);
    }
    std::cout << "(evaluateSymbolValue): field not found in anywhere, sorry" << std::endl;
    return TTObject::createObject(TT_NIL);
}

TTObject *Evaluator::evaluateBlock(TTObject *block, TTObject *env)
{
    std::cout << "(evaluateBlock)" << std::endl;

    block->addField(TO_TT_STR("blockEnv"), env);
    return block;
}

TTObject *Evaluator::evaluateParenthesis(TTObject *parenthesis, TTObject *env)
{
    std::cout << "(evaluateParenthesis)" << std::endl;

    TTObject *innerExpr = parenthesis->getField(TO_TT_STR("innerExpr"));
    TTObject *innerResult = evaluate(innerExpr, env);

    return innerResult;
}

TTObject *Evaluator::evaluateAssign(TTObject *expr, TTObject *env)
{
    std::cout << "(evaluateAssign)" << std::endl;

    std::string name = (char *) expr->getField(TO_TT_STR("assignSymbolName"))->getLiteral()->data;

    TTObject *assignedExpr = evaluate(expr->getField(TO_TT_STR("assignExpression")), env);


    if (env->hasField(TO_TT_STR(name.c_str())))
    {
        env->setField(TO_TT_STR(name.c_str()), assignedExpr);
        return assignedExpr;
    }
    else
    {
        std::cout << "(evaluateAssign): field not found in environment" << std::endl;
    }

    TTObject *thiz = env->getField(TO_TT_STR("this"));
    if (!thiz)
    {
        std::cout << "(evaluateAssign): this is NULL" << std::endl;
    }
    else
    {
        if (thiz->hasField(TO_TT_STR(name.c_str())))
        {
            thiz->setField(TO_TT_STR(name.c_str()), assignedExpr);
            return assignedExpr;
        }
        else
        {
            std::cout << "(evaluateAssign): field was found in this" << std::endl;
        }
    }
    std::cout << "(evaluateAssign): field not found in anywhere (not going to parent), sorry" << std::endl;
    return assignedExpr;
}

TTObject *Evaluator::evaluateCreateVariables(TTObject *expr, TTObject *env)
{
    std::cout << "(evaluateCreateVariables)" << std::endl;

    TTObject *varNames = expr->getField(TO_TT_STR("varNames"));
    TTLiteral *varNamesLit = varNames->getLiteral();

    if(varNamesLit)
    {
        uint32_t len = varNamesLit->length / sizeof(TTObject *);
        TTObject **objects = (TTObject **) varNamesLit->data; // TODO: GC will fail with this here!

        for(uint32_t i = 0; i < len; i++)
        {
            TTObject *lit = objects[i];
            std::string name = (char *) lit->getLiteral()->data;
            env->addField(TO_TT_STR(name.c_str()), TTObject::createObject(TT_NIL));
        }
    }
    else
    {
        std::cerr << "(evaluateCreateVariables): variable array is NULL!!" << std::endl;
        throw std::exception();
    }
    return TTObject::createObject(TT_NIL);
}

TTObject *Evaluator::evaluateChained(TTObject *expr, TTObject *env)
{
    std::cout << "(evaluateChained)" << std::endl;

    TTObject *currExpr = expr->getField(TO_TT_STR("currExpr"));
    TTObject *nextExpr = expr->getField(TO_TT_STR("nextExpr"));

    TTObject *currResult = evaluate(currExpr, env);

    if(!nextExpr)
    {
        return currResult;
    }
    if(nextExpr->type == TT_NIL)
    {
        return currResult;
    }

    return evaluate(nextExpr, env);
}

TTObject *Evaluator::evaluateLiteralValue(TTObject *expr, TTObject *env)
{
    std::cout << "(evaluateLiteralValue)" << std::endl;

    TTObject *lit = expr->getField(TO_TT_STR("literalValue"));
    return lit;
}

TTObject *Evaluator::sendMultipleMessageToNonExpression(std::string &msgFullName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *dest, TTObject *env)
{
    std::cout << "(sendMultipleMessageToNonExpression)" << std::endl;

    TTObject *field = dest->getField(TO_TT_STR(msgFullName.c_str()));

    if(field)
    {
        if(field->type != TT_EXPR)
        {
            std::cerr << "(sendMultipleMessageToNonExpression): Sorry, value is not a method: " << field << " !!!" << std::endl;
        }

        return executeMultipleMessage(dest, msgFullName, argNames, values, env, dest);
    }
    else
    {
        std::cerr << "(sendMultipleMessageToNonExpression): Sorry, object doesn't understand message: " << msgFullName << " !!!" << std::endl;
        return TTObject::createObject(TT_NIL);
    }
}

TTObject *Evaluator::executeMultipleMessage(TTObject *blockExpression, std::string &msgFullName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *env, TTObject *thiz)
{
    std::cout << "(executeMultipleMessage)" << std::endl;

    std::string name = (char *) blockExpression->getField(TO_TT_STR("blockFullName"))->getLiteral()->data;
    if(COMPARE_NAME(msgFullName.c_str(), name.c_str()) == 0)
    {
        TTObject *blockEnv = blockExpression->getField(TO_TT_STR("blockEnv"));
        TTObject *blockExpr = blockExpression->getField(TO_TT_STR("blockExpr"));
        TTObject *nativeName = blockExpression->getField(TO_TT_STR("blockNativeName"));
        TTObject *newEnv = TTObject::createObject(TT_ENV);
        newEnv->addField(TO_TT_STR("parent"), blockEnv);

        if(thiz)
        {
            newEnv->addField(TO_TT_STR("this"), thiz);
        }

        for(size_t i = 0; i < argNames.size(); i++)
        {
            newEnv->addField(TO_TT_STR(argNames[i].c_str()), values[i]);
        }

        /**
        * Todo: execute bytecode or native code ... LOL
        */

        if(nativeName)
        {
            TTLiteral *lit = nativeName->getLiteral();
            std::string strName = (char *) lit->data;

            std::shared_ptr<Builtin> builtin = Runtime::builtinPool.lookupBultin(strName);
            if(builtin)
            {
                std::cout << "(executeMultipleMessage): Builtin function '" << strName << "' found." << std::endl;
                if(thiz)
                {
                    return builtin->invoke(thiz, argNames, values);
                }
                return builtin->invoke(env, argNames, values);
            }
            else
            {
                std::cerr << "(executeMultipleMessage): Builtin function '" << strName << "' not found!" << std::endl;
                throw std::exception();
            }
        }

        return evaluate(blockExpr, newEnv);
    }
    else
    {
        return sendMultipleMessageToNonExpression(msgFullName, argNames, values, blockExpression, env);
    }
}


TTObject *Evaluator::evaluateMultipleMessage(TTObject *expr, TTObject *env)
{
    std::cout << "(evaluateMultipleMessage)" << std::endl;

    TTObject *msgDestExpr = expr->getField(TO_TT_STR("msgDestExpr"));
    TTObject *msgFullName = expr->getField(TO_TT_STR("msgFullName"));
    TTObject *msgNameArray = expr->getField(TO_TT_STR("msgNameArray"));
    TTObject *msgValueArray = expr->getField(TO_TT_STR("msgValueArray"));

    TTObject *msgDest = evaluate(msgDestExpr, env);
    std::string fullName = (char *) msgFullName->getLiteral()->data;
    std::vector<std::string> names;
    std::vector<TTObject *> valuesExpressions;
    std::vector<TTObject *> values;

    uint32_t len = msgNameArray->getLiteral()->length / sizeof(TTObject *);
    TTObject **nameObjects = (TTObject **) msgNameArray->getLiteral()->data; // TODO: fix this, we should not allocate any memory, but if a GC is forced here, the inevitable will happen ... GC will fail
    TTObject **expressionObjects = (TTObject **) msgValueArray->getLiteral()->data; // TODO: fix this, we should not allocate any memory, but if a GC is forced here, the inevitable will happen ... GC will fail
    for(uint32_t i = 0; i < len; i++)
    {
        names.push_back((char *) nameObjects[i]->getLiteral()->data);
        std::cout << "(evaluateMultipleMessage): arg name " << i << " = " << names[i] << std::endl;
        valuesExpressions.push_back(expressionObjects[i]);
    }

    for(uint32_t i = 0; i < len; i++)
    {
        TTObject *value = evaluate(valuesExpressions[i], env);
        values.push_back(value);
    }

    if(!msgDest)
    {
        std::cerr << "(evaluateMultipleMessage): destValue is NULL!" << std::endl;
        return TTObject::createObject(TT_NIL);
    }

    if(msgDest->type == TT_EXPR)
    {
        if(msgDest->flags == EXPRESSION_BLOCK)
        {
            return executeMultipleMessage(msgDest, fullName, names, values, env, NULL);
        }
        else
        {
            return sendMultipleMessageToNonExpression(fullName, names, values, msgDest, env);
        }
    }
    else
    {
        return sendMultipleMessageToNonExpression(fullName, names, values, msgDest, env);
    }
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
            res = evaluateAssign(expression, env);
            break;
        case EXPRESSION_CREATE_VARIABLES:
            res = evaluateCreateVariables(expression, env);
            break;
        case EXPRESSION_BLOCK:
            res = evaluateBlock(expression, env);
            break;
        case EXPRESSION_CHAINED:
            res = evaluateChained(expression, env);
            break;
        case EXPRESSION_LITERAL_VALUE:
            res = evaluateLiteralValue(expression, env);
            break;
        case EXPRESSION_MULTIPLE_MESSAGE:
            res = evaluateMultipleMessage(expression, env);
            break;
        case EXPRESSION_PARENTHESIS:
            res = evaluateParenthesis(expression, env);
            break;
    }

    return res;
}
