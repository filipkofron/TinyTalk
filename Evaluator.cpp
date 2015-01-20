#include "TTLiteral.h"
#include "Evaluator.h"
#include "Expression.h"
#include "Common.h"
#include "Runtime.h"
#include <cstring>

RefPtr<TTObject> Evaluator::executeSimpleExpression(RefPtr<TTObject> expression, RefPtr<TTObject> dest, std::string &msgName, RefPtr<TTObject> thiz)
{
#ifdef DEBUG
    std::cout << "(executeSimpleExpression)" << std::endl;
#endif

    RefPtr<TTObject> env = TTObject::createObject(TT_ENV);
    RefPtr<TTObject> blockExpr = expression->getField(TO_TT_STR("blockExpr"));
    RefPtr<TTObject> blockNativeName = expression->getField(TO_TT_STR("blockNativeName"));
    RefPtr<TTObject> blockEnv = expression->getField(TO_TT_STR("blockEnv"));

    if(&thiz)
    {
#ifdef DEBUG
        std::cout << "(executeSimpleExpression) has this: " << thiz << std::endl;
#endif
        env->addField(TO_TT_STR("this"), thiz);
    }
#ifdef DEBUG
    std::cout << "(executeSimpleExpression) parent env: " << blockEnv << std::endl;
#endif
    env->addField(TO_TT_STR("parentEnv"), blockEnv);

    if(&blockNativeName)
    {
        std::string nativeName = (char *) blockNativeName->getLiteral()->data;

        return Runtime::executeSimpleNativeMessage(nativeName, dest, msgName, blockEnv, thiz);
    }

    return evaluate(blockExpr, env);
}

RefPtr<TTObject> Evaluator::evaluateSimpleMessage(RefPtr<TTObject> simpleMessage, RefPtr<TTObject> env)
{
#ifdef DEBUG
    std::cout << "(evaluateSimpleMessage)" << std::endl;
#endif

    std::string msgName = (char *) simpleMessage->getField(TO_TT_STR("msgName"))->getLiteral()->data;
    RefPtr<TTObject> destExpr = simpleMessage->getField(TO_TT_STR("msgDestExpr"));
    RefPtr<TTObject> destValue = evaluate(destExpr, env);

#ifdef DEBUG
    std::cout << "(evaluateSimpleMessage) destValue:" << std::endl;
    destValue->print(std::cout, 0, false);
    std::cout << std::endl;
#endif

    if(!&destValue)
    {
        std::cerr << "(evaluateSimpleMessage): destValue is NULL!" << std::endl;
        throw std::exception();
    }

    TTObject *thiz = NULL;

    RefPtr<TTObject> expr = Runtime::findBlock(TO_TT_STR(msgName.c_str()), &destValue, &env, &thiz);

    return executeSimpleExpression(expr, destValue, msgName, thiz);
}

/**
* Evaluates expression of symbol value.
*
* This will look into the environment for the symbol value
* by the given name and if this fails, it tries to find it
* in the this, which corresponds to an object the message
* is currently being answered by.
*/
RefPtr<TTObject> Evaluator::evaluateSymbolValue(RefPtr<TTObject> symbolValue, RefPtr<TTObject> env)
{
    std::string name = (char *) symbolValue->getField(TO_TT_STR("symbolName"))->getLiteral()->data;
#ifdef DEBUG
    std::cout << "(evaluateSymbolValue): of name '" << name << "'" << std::endl;
#endif

    RefPtr<TTObject> nextEnv = env;
    RefPtr<TTObject> result = NULL;

    do
    {
        result = Runtime::findSymbol(TO_TT_STR(name.c_str()), nextEnv, nextEnv);
    } while(!&result && &nextEnv);

    if(&result)
    {
        return result;
    }

    //

    // we should get to the parent environment

    /*if (env != Runtime::globalEnvironment)
    {
        std::cout << "(evaluateSymbolValue): trying gloval env" << std::endl;
        return evaluateSymbolValue(symbolValue, Runtime::globalEnvironment);
    }*/
    std::cerr << "(evaluateSymbolValue): field " << name << " not found in anywhere, sorry" << std::endl;
    throw std::exception();
}

RefPtr<TTObject> Evaluator::evaluateBlock(RefPtr<TTObject> block, RefPtr<TTObject> env)
{
#ifdef DEBUG
    std::cout << "(evaluateBlock)" << std::endl;
    std::cout << "(evaluateBlock) env: " << std::endl;
    env->print(std::cout, 0, false);
    std::cout << std::endl;
#endif

    RefPtr<TTObject> newBlock = TTObject::clone(block);

    bool success = newBlock->addField(TO_TT_STR("blockEnv"), env);
    if(!success)
    {
        newBlock->setField(TO_TT_STR("blockEnv"), env);
    }

#ifdef DEBUG
    std::cout << "(evaluateBlock) env after: " << std::endl;
    env->print(std::cout, 0, false);
    std::cout << std::endl;
#endif
    return newBlock;
    /*
    bool success = block->addField(TO_TT_STR("blockEnv"), env);
    if(!success)
    {
        block->setField(TO_TT_STR("blockEnv"), env);
    }
    return block;*/
}

RefPtr<TTObject> Evaluator::evaluateParenthesis(RefPtr<TTObject> parenthesis, RefPtr<TTObject> env)
{
#ifdef DEBUG
    std::cout << "(evaluateParenthesis)" << std::endl;
#endif

    RefPtr<TTObject> innerExpr = parenthesis->getField(TO_TT_STR("innerExpr"));
    RefPtr<TTObject> innerResult = evaluate(innerExpr, env);

    return innerResult;
}

RefPtr<TTObject> Evaluator::evaluateAssign(RefPtr<TTObject> expr, RefPtr<TTObject> env)
{
#ifdef DEBUG
    std::cout << "(evaluateAssign)" << std::endl;
#endif

    std::string name = (char *) expr->getField(TO_TT_STR("assignSymbolName"))->getLiteral()->data;

    if (env->hasField(TO_TT_STR(name.c_str())))
    {
        RefPtr<TTObject> assignedExpr = evaluate(expr->getField(TO_TT_STR("assignExpression")), env);
        env->setField(TO_TT_STR(name.c_str()), assignedExpr);
        return assignedExpr;
    }
    else
    {
#ifdef DEBUG
        std::cout << "(evaluateAssign): field not found in environment" << std::endl;
#endif
    }

    RefPtr<TTObject> thiz = env->getField(TO_TT_STR("this"));
    if (!&thiz)
    {
        std::cout << "(evaluateAssign): this is NULL" << std::endl;
    }
    else
    {
        if (thiz->hasField(TO_TT_STR(name.c_str())))
        {
            RefPtr<TTObject> assignedExpr = evaluate(expr->getField(TO_TT_STR("assignExpression")), env);
            thiz->setField(TO_TT_STR(name.c_str()), assignedExpr);
            return assignedExpr;
        }
        else
        {
#ifdef DEBUG
            std::cout << "(evaluateAssign): field was found in this" << std::endl;
#endif
        }
    }

    RefPtr<TTObject> parent = env->getField(TO_TT_STR("parentEnv"));
    if (&parent && parent->type == TT_ENV)
    {
#ifdef DEBUG
        std::cout << "(evaluateAssign): trying parent env" << std::endl;
#endif
        return evaluateAssign(expr, parent);
    }

    std::cerr << "(evaluateAssign): field not found in anywhere (not going to parent), sorry" << std::endl;
    throw std::exception();
}

RefPtr<TTObject> Evaluator::evaluateCreateVariables(RefPtr<TTObject> expr, RefPtr<TTObject> env)
{
#ifdef DEBUG
    std::cout << "(evaluateCreateVariables)" << std::endl;
#endif

    RefPtr<TTObject> varNames = expr->getField(TO_TT_STR("varNames"));
    RefPtr<TTLiteral> varNamesLit = varNames->getLiteral();

    if(&varNamesLit)
    {
        uint32_t len = varNamesLit->length / sizeof(TTObject *);

        for(uint32_t i = 0; i < len; i++)
        {
            RefPtr<TTObject> lit = ((TTObject **) varNamesLit->data)[i];
            std::string name = (char *) lit->getLiteral()->data;
            env->addField(TO_TT_STR(name.c_str()), Runtime::globalEnvironment->getField(TO_TT_STR("nil")));
        }
    }
    else
    {
        std::cerr << "(evaluateCreateVariables): variable array is NULL!!" << std::endl;
        throw std::exception();
    }
    return Runtime::globalEnvironment->getField(TO_TT_STR("nil"));
}

RefPtr<TTObject> Evaluator::evaluateChained(RefPtr<TTObject> expr, RefPtr<TTObject> env)
{
#ifdef DEBUG
    std::cout << "(evaluateChained)" << std::endl;
#endif

    RefPtr<TTObject> currExpr = expr->getField(TO_TT_STR("currExpr"));
    RefPtr<TTObject> nextExpr = expr->getField(TO_TT_STR("nextExpr"));

    RefPtr<TTObject> currResult = evaluate(currExpr, env);

    if(!&nextExpr)
    {
        return currResult;
    }
    if(nextExpr->type == TT_NIL)
    {
        return currResult;
    }

    return evaluate(nextExpr, env);
}

RefPtr<TTObject> Evaluator::evaluateLiteralValue(RefPtr<TTObject> expr, RefPtr<TTObject> env)
{
#ifdef DEBUG
    std::cout << "(evaluateLiteralValue)" << std::endl;
#endif

    RefPtr<TTObject> lit = expr->getField(TO_TT_STR("literalValue"));
    return lit;
}

RefPtr<TTObject> Evaluator::executeMultipleExpression(RefPtr<TTObject> expression, RefPtr<TTObject> dest, std::string &msgName, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> thiz)
{
#ifdef DEBUG
    std::cout << "(executeMultipleExpression)" << std::endl;
#endif

    // TODO: Allocation of env wastes memory

    RefPtr<TTObject> env = TTObject::createObject(TT_ENV);
    RefPtr<TTObject> blockExpr = expression->getField(TO_TT_STR("blockExpr"));
    RefPtr<TTObject> blockNativeName = expression->getField(TO_TT_STR("blockNativeName"));
    RefPtr<TTObject> blockEnv = expression->getField(TO_TT_STR("blockEnv"));

    if(&thiz)
    {
#ifdef DEBUG
        std::cout << "(executeMultipleExpression) has this: " << thiz << std::endl;
#endif
        env->addField(TO_TT_STR("this"), thiz);
    }
#ifdef DEBUG
    std::cout << "(executeMultipleExpression) parent env: " << blockEnv << std::endl;
#endif
    env->addField(TO_TT_STR("parentEnv"), blockEnv);

    if(&blockNativeName)
    {
        std::string nativeName = (char *) blockNativeName->getLiteral()->data;

        return Runtime::executeMultipleNativeMessage(nativeName, dest, msgName, argNames, values, blockEnv, thiz);
    }

    for(size_t i = 0; i < argNames.size(); i++)
    {
        env->addField(TO_TT_STR(argNames[i].c_str()), values[i]);
    }

    return evaluate(blockExpr, env);
}

RefPtr<TTObject> Evaluator::evaluateMultipleMessage(RefPtr<TTObject> simpleMessage, RefPtr<TTObject> env)
{
#ifdef DEBUG
    std::cout << "(evaluateMultipleMessage)" << std::endl;
#endif

    RefPtr<TTObject> msgDestExpr = simpleMessage->getField(TO_TT_STR("msgDestExpr"));
    RefPtr<TTObject> msgFullName = simpleMessage->getField(TO_TT_STR("msgFullName"));
    RefPtr<TTObject> msgNameArray = simpleMessage->getField(TO_TT_STR("msgNameArray"));
    RefPtr<TTObject> msgValueArray = simpleMessage->getField(TO_TT_STR("msgValueArray"));

#ifdef DEBUG
    std::cout << "(evaluateMultipleMessage) fullname: " << (const char *) msgFullName->getLiteral()->data << std::endl;
#endif

    std::string fullName = (char *) msgFullName->getLiteral()->data;
    std::vector<std::string> names;
    std::vector<RefPtr<TTObject> > valuesExpressions;
    std::vector<RefPtr<TTObject> > values;

    bool replace = false;
    if(fullName == "value:")
    {
        replace = true;
    }

    uint32_t len = (uint32_t) (msgNameArray->getLiteral()->length / sizeof(TTObject *));
    for(uint32_t i = 0; i < len; i++)
    {
        if(!replace)
        {
            names.push_back((char *) ((TTObject **) msgNameArray->getLiteral()->data)[i]->getLiteral()->data);
#ifdef DEBUG
            std::cout << "(evaluateMultipleMessage): arg name " << i << " = " << names[i] << std::endl;
#endif
        }
        else
        {
#ifdef DEBUG
            std::cout << "(evaluateMultipleMessage): skipping because of value:" << std::endl;
#endif
        }

        valuesExpressions.push_back(((TTObject **) msgValueArray->getLiteral()->data)[i]);
    }

#ifdef DEBUG
    std::cout << "(evaluateMultipleMessage) before eval values env: " << env << std::endl;
#endif

    for(uint32_t i = 0; i < len; i++)
    {
        RefPtr<TTObject> value = evaluate(valuesExpressions[i], env);
        values.push_back(value);
    }

    RefPtr<TTObject> msgDestValue = evaluate(msgDestExpr, env);

    if(!&msgDestValue)
    {
        std::cerr << "(evaluateMultipleMessage): msgDestValue is NULL!" << std::endl;
        throw std::exception();
    }

#ifdef DEBUG
    std::cout << "(evaluateMultipleMessage): msgDestValue: " << msgDestValue << std::endl;
#endif

    TTObject *thiz = NULL;

    RefPtr<TTObject> expr = Runtime::findBlock(TO_TT_STR(fullName.c_str()), &msgDestValue, &env, &thiz);

    if(replace)
    {
        names.push_back((const char *) ((TTObject **) expr->getField(TO_TT_STR("blockArgNames"))->getLiteral()->data)[0]->getLiteral()->data);
    }

    return executeMultipleExpression(expr, msgDestValue, fullName, names, values, thiz); // call named lambda expression
}

RefPtr<TTObject> Evaluator::evaluateArray(RefPtr<TTObject> expr, RefPtr<TTObject> env)
{
#ifdef DEBUG
    std::cout << "(evaluateArray): expr: " << expr << std::endl;
#endif
    RefPtr<TTObject> expressions = expr->getField(TO_TT_STR("expressions"));
    RefPtr<TTLiteral> expressionsLit = expressions->getLiteral();
    uint32_t size = (uint32_t) (expressionsLit->length / sizeof(TTObject *));

#ifdef DEBUG
    std::cout << "(evaluateArray): size: " << size << std::endl;
#endif

    RefPtr<TTObject> array = TTLiteral::createObjectArray(size);

    for(uint32_t i = 0; i < size; i++)
    {
        RefPtr<TTObject> exprItem = ((TTObject **) expressionsLit->data)[i];
        RefPtr<TTObject> val = NULL;
        if(&exprItem)
        {
            val = evaluate(exprItem, env);
        }
        else
        {
            val = Runtime::globalEnvironment->getField(TO_TT_STR("nil"));
        }


        ((TTObject **) array->getLiteral()->data)[i] = &val;
    }

    return array;

}

RefPtr<TTObject> Evaluator::evaluate(RefPtr<TTObject> expression, RefPtr<TTObject> env)
{
#ifdef DEBUG
    std::cout << "(evaluate)" << std::endl;
#endif
    RefPtr<TTObject> res = NULL;

    if(!&expression)
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
        case EXPRESSION_ARRAY:
            res = evaluateArray(expression, env);
            break;
        default:
            std::cerr << "[Runtime]: Error: Invalid expression flags: " << (uint32_t) expression->flags << std::endl;
            throw std::exception();
    }

    return res;
}
// TODO: remove this!!!!!!!!!!!!
RefPtr<TTObject> Evaluator::sendSimpleMessage(RefPtr<TTObject> object, std::string &name)
{
#ifdef DEBUG
    std::cout << "(sendSimpleMessage)" << std::endl;
    std::cout << "(sendSimpleMessage) name: " << name << std::endl;
#endif

    TTObject *next = &object;
    TTObject *resExpr = NULL;
    do
    {
        resExpr = Runtime::findBlockAtNonExpression(next, TO_TT_STR(name.c_str()), &next);
    } while(!resExpr && next);

    return executeSimpleExpression(resExpr, object, name, object);
}
