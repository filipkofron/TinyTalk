#include "Evaluator.h"
#include "Expression.h"
#include "common.h"
#include "Runtime.h"
#include <cstring>

TTObject *Evaluator::executeSimpleExpression(TTObject *expression, TTObject *dest, std::string &msgName, TTObject *thiz)
{
#ifdef DEBUG
    std::cout << "(executeSimpleExpression)" << std::endl;
#endif

    TTObject *env = TTObject::createObject(TT_ENV);
    TTObject *blockExpr = expression->getField(TO_TT_STR("blockExpr"));
    TTObject *blockNativeName = expression->getField(TO_TT_STR("blockNativeName"));
    TTObject *blockEnv = expression->getField(TO_TT_STR("blockEnv"));

    if(thiz)
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

    if(blockNativeName)
    {
        std::string nativeName = (char *) blockNativeName->getLiteral()->data;

        return executeSimpleNativeMessage(nativeName, dest, msgName, thiz);
    }

    return evaluate(blockExpr, env);
}

TTObject *Evaluator::executeSimpleNativeMessage(std::string &nativeName, TTObject *dest, std::string &msgName, TTObject *thiz)
{
    std::shared_ptr<Builtin> builtin = Runtime::builtinPool.lookupBultin(nativeName);
    std::vector<std::string> singleList;
    singleList.push_back(msgName);
    if(builtin)
    {
#ifdef DEBUG
        std::cout << "(executeSimpleNativeMessage): Builtin function '" << nativeName << "' found." << std::endl;
#endif
        if(thiz)
        {
            return builtin->invoke(thiz, singleList, std::vector<TTObject *>());
        }
        return builtin->invoke(dest, singleList, std::vector<TTObject *>());
    }
    else
    {
        std::cerr << "(executeSimpleNativeMessage): Builtin function '" << nativeName << "' not found!" << std::endl;
        throw std::exception();
    }
    return NULL;
}

TTObject *Evaluator::executeSimpleExpressionAtNonExpression(TTObject *object, TTObject *dest, std::string &name)
{
#ifdef DEBUG
    std::cout << "(executeSimpleExpressionAtNonExpression)" << std::endl;
#endif

    TTObject *fieldVal = object->getField(TO_TT_STR(name.c_str()));
    if(fieldVal)
    {
        if(fieldVal->type != TT_EXPR || fieldVal->flags != EXPRESSION_BLOCK)
        {
            std::cerr << "(executeSimpleExpressionAtNonExpression): Error: NON EXPRESSION FIELD: " << fieldVal << std::endl;
            throw std::exception();
        }
        return executeSimpleExpression(fieldVal, dest, name, dest);
    }

    TTObject *parent = object->getField(TO_TT_STR("parent"));

    if(!parent)
    {
        if(dest->type == TT_LITERAL)
        {
            std::vector<std::string> singleList;
            singleList.push_back(name);
            return dest->getLiteral()->onMessage(dest, name, singleList, std::vector<TTObject *>());
        }

        std::cerr << "(executeSimpleExpressionAtNonExpression): Object doesn't understand this message '" << name << "'" << std::endl;
        throw std::exception();
    }

    // Now, either the field is in this object or in its parents, nowhere else!
    return executeSimpleExpressionAtNonExpression(parent, dest, name);
}

TTObject *Evaluator::evaluateSimpleMessage(TTObject *simpleMessage, TTObject *env)
{
#ifdef DEBUG
    std::cout << "(evaluateSimpleMessage)" << std::endl;
#endif

    std::string msgName = (char *) simpleMessage->getField(TO_TT_STR("msgName"))->getLiteral()->data;
    TTObject *destExpr = simpleMessage->getField(TO_TT_STR("msgDestExpr"));
    TTObject *destValue = evaluate(destExpr, env);

    TTObject *thiz = env->getField(TO_TT_STR("this"));

    // the accepted destValues types are: Object or Expression, the expression is considered
    // an object if it doesn't have the same name

    if(destValue->type == TT_EXPR && destValue->flags == EXPRESSION_BLOCK)
    {
        std::string blockName = (char *) destValue->getField(TO_TT_STR("blockFullName"))->getLiteral()->data;
        if(COMPARE_NAME(blockName.c_str(), msgName.c_str()) == 0)
        {
            return executeSimpleExpression(destValue, destValue, msgName, thiz); // call named lambda expression
        }
        if(blockName.empty() && msgName == "value")
        {
            return executeSimpleExpression(destValue, destValue, msgName, thiz); // call nameless lambda expression
        }

        // since now we expect that the message is sent to the expression considered object
    }

    return executeSimpleExpressionAtNonExpression(destValue, destValue, msgName);
}

TTObject *Evaluator::evaluateSymbolValueInThis(TTObject *thiz, std::string &name)
{
    TTObject *resultFromThis = thiz->getField(TO_TT_STR(name.c_str()));
    if (!resultFromThis)
    {
        TTObject *thisParent = thiz->getField(TO_TT_STR("parent"));
        if(thisParent)
        {
#ifdef DEBUG
            std::cout << "(evaluateSymbolValue): field not found in this, trying parent =>" << std::endl;
#endif
            return evaluateSymbolValueInThis(thisParent, name);
        }
        else
        {
#ifdef DEBUG
            std::cout << "(evaluateSymbolValue): field not found in this, no parent as well." << std::endl;
#endif
            return NULL; // not an error, we can search for that in gloval environment after
        }
    }
    return resultFromThis;
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
#ifdef DEBUG
    std::cout << "(evaluateSymbolValue)" << std::endl;
#endif

    std::string name = (char *) symbolValue->getField(TO_TT_STR("symbolName"))->getLiteral()->data;
#ifdef DEBUG
    std::cout << "(evaluateSymbolValue): of name '" << name << "'" << std::endl;
#endif

    TTObject *resultFromEnv = env->getField(TO_TT_STR(name.c_str()));
    if (!resultFromEnv)
    {
#ifdef DEBUG
        std::cout << "(evaluateSymbolValue): field name '"
                << name << "' not found in environment" << std::endl;
#endif
    }
    else
    {
#ifdef DEBUG
        std::cout << "(evaluateSymbolValue): field name '"
                << name << "' found in environment" << std::endl;

        std::cout << "res: " << resultFromEnv << std::endl;
#endif
        return resultFromEnv;
    }

    TTObject *thiz = env->getField(TO_TT_STR("this"));
    if (!thiz)
    {
#ifdef DEBUG
        std::cout << "(evaluateSymbolValue): this is NULL" << std::endl;
#endif
    }
    else
    {
        TTObject *resultFromThis = evaluateSymbolValueInThis(thiz, name);
        if (!resultFromThis)
        {
#ifdef DEBUG
            std::cout << "(evaluateSymbolValue): field not found in this" << std::endl;
#endif
        }
        else
        {
#ifdef DEBUG
            std::cout << "(evaluateSymbolValue): field was found in this" << std::endl;
#endif
            return resultFromThis;
        }
    }

    TTObject *parent = env->getField(TO_TT_STR("parentEnv"));
    if (parent && parent->type == TT_ENV)
    {
#ifdef DEBUG
        std::cout << "(evaluateSymbolValue): trying parent env" << std::endl;
#endif
        return evaluateSymbolValue(symbolValue, parent);
    }

    // we should get to the parent environment

    /*if (env != Runtime::globalEnvironment)
    {
        std::cout << "(evaluateSymbolValue): trying gloval env" << std::endl;
        return evaluateSymbolValue(symbolValue, Runtime::globalEnvironment);
    }*/
    std::cerr << "(evaluateSymbolValue): field " << name << " not found in anywhere, sorry" << std::endl;
    throw std::exception();
}

TTObject *Evaluator::evaluateBlock(TTObject *block, TTObject *env)
{
#ifdef DEBUG
    std::cout << "(evaluateBlock)" << std::endl;
#endif

    block->addField(TO_TT_STR("blockEnv"), env);
    return block;
}

TTObject *Evaluator::evaluateParenthesis(TTObject *parenthesis, TTObject *env)
{
#ifdef DEBUG
    std::cout << "(evaluateParenthesis)" << std::endl;
#endif

    TTObject *innerExpr = parenthesis->getField(TO_TT_STR("innerExpr"));
    TTObject *innerResult = evaluate(innerExpr, env);

    return innerResult;
}

TTObject *Evaluator::evaluateAssign(TTObject *expr, TTObject *env)
{
#ifdef DEBUG
    std::cout << "(evaluateAssign)" << std::endl;
#endif

    std::string name = (char *) expr->getField(TO_TT_STR("assignSymbolName"))->getLiteral()->data;

    if (env->hasField(TO_TT_STR(name.c_str())))
    {
        TTObject *assignedExpr = evaluate(expr->getField(TO_TT_STR("assignExpression")), env);
        env->setField(TO_TT_STR(name.c_str()), assignedExpr);
        return assignedExpr;
    }
    else
    {
#ifdef DEBUG
        std::cout << "(evaluateAssign): field not found in environment" << std::endl;
#endif
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
            TTObject *assignedExpr = evaluate(expr->getField(TO_TT_STR("assignExpression")), env);
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

    TTObject *parent = env->getField(TO_TT_STR("parentEnv"));
    if (parent && parent->type == TT_ENV)
    {
#ifdef DEBUG
        std::cout << "(evaluateAssign): trying parent env" << std::endl;
#endif
        return evaluateAssign(expr, parent);
    }

    std::cerr << "(evaluateAssign): field not found in anywhere (not going to parent), sorry" << std::endl;
    throw std::exception();
}

TTObject *Evaluator::evaluateCreateVariables(TTObject *expr, TTObject *env)
{
#ifdef DEBUG
    std::cout << "(evaluateCreateVariables)" << std::endl;
#endif

    TTObject *varNames = expr->getField(TO_TT_STR("varNames"));
    TTLiteral *varNamesLit = varNames->getLiteral();

    if(varNamesLit)
    {
        uint32_t len = varNamesLit->length / sizeof(TTObject *);

        for(uint32_t i = 0; i < len; i++)
        {
            TTObject *lit = ((TTObject **) varNamesLit->data)[i];
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

TTObject *Evaluator::evaluateChained(TTObject *expr, TTObject *env)
{
#ifdef DEBUG
    std::cout << "(evaluateChained)" << std::endl;
#endif

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
#ifdef DEBUG
    std::cout << "(evaluateLiteralValue)" << std::endl;
#endif

    TTObject *lit = expr->getField(TO_TT_STR("literalValue"));
    return lit;
}

TTObject *Evaluator::executeMultipleExpression(TTObject *expression, TTObject *dest, std::string &msgName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *thiz)
{
#ifdef DEBUG
    std::cout << "(executeMultipleExpression)" << std::endl;
#endif

    // TODO: Allocation of env wastes memory

    TTObject *env = TTObject::createObject(TT_ENV);
    TTObject *blockExpr = expression->getField(TO_TT_STR("blockExpr"));
    TTObject *blockNativeName = expression->getField(TO_TT_STR("blockNativeName"));
    TTObject *blockEnv = expression->getField(TO_TT_STR("blockEnv"));

    if(thiz)
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

    if(blockNativeName)
    {
        std::string nativeName = (char *) blockNativeName->getLiteral()->data;

        return executeMultipleNativeMessage(nativeName, dest, msgName, argNames, values, thiz);
    }

    for(size_t i = 0; i < argNames.size(); i++)
    {
        env->addField(TO_TT_STR(argNames[i].c_str()), values[i]);
    }

    return evaluate(blockExpr, env);
}

TTObject *Evaluator::executeMultipleNativeMessage(std::string &nativeName, TTObject *dest, std::string &msgName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *thiz)
{
#ifdef DEBUG
    std::cout << "(executeMultipleNativeMessage)" << std::endl;
#endif

    std::shared_ptr<Builtin> builtin = Runtime::builtinPool.lookupBultin(nativeName);
    if(builtin)
    {
#ifdef DEBUG
        std::cout << "(executeMultipleNativeMessage): Builtin function '" << nativeName << "' found." << std::endl;
#endif
        if(thiz)
        {
            return builtin->invoke(thiz, argNames, values);
        }
        return builtin->invoke(dest, argNames, values);
    }
    else
    {
        std::cerr << "(executeMultipleNativeMessage): Builtin function '" << nativeName << "' not found!" << std::endl;
        throw std::exception();
    }
    return NULL;
}

TTObject *Evaluator::executeMultipleExpressionAtNonExpression(TTObject *object, TTObject *dest, std::string &name, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
#ifdef DEBUG
    std::cout << "(executeMultipleExpressionAtNonExpression)" << std::endl;
#endif

    TTObject *fieldVal = object->getField(TO_TT_STR(name.c_str()));
    if(fieldVal)
    {
        if(fieldVal->type != TT_EXPR || fieldVal->flags != EXPRESSION_BLOCK)
        {
            std::cerr << "(executeMultipleExpressionAtNonExpression): Error: NON EXPRESSION FIELD: " << fieldVal << std::endl;
            throw std::exception();
        }
        return executeMultipleExpression(fieldVal, dest, name, argNames, values, dest);
    }

    TTObject *parent = object->getField(TO_TT_STR("parent"));

    if(!parent)
    {
        if(dest->type == TT_LITERAL)
        {
            return dest->getLiteral()->onMessage(dest, name, argNames, values);
        }

        std::cerr << "(executeMultipleExpressionAtNonExpression): Object doesn't understand this message: " << name << std::endl;
        throw std::exception();
    }

    // Now, either the field is in this object or in its parents, nowhere else!
    return executeMultipleExpressionAtNonExpression(parent, dest, name, argNames, values);
}

TTObject *Evaluator::evaluateMultipleMessage(TTObject *simpleMessage, TTObject *env)
{
#ifdef DEBUG
    std::cout << "(evaluateMultipleMessage)" << std::endl;
#endif

    TTObject *msgDestExpr = simpleMessage->getField(TO_TT_STR("msgDestExpr"));
    TTObject *msgFullName = simpleMessage->getField(TO_TT_STR("msgFullName"));
    TTObject *msgNameArray = simpleMessage->getField(TO_TT_STR("msgNameArray"));
    TTObject *msgValueArray = simpleMessage->getField(TO_TT_STR("msgValueArray"));

#ifdef DEBUG
    std::cout << "(evaluateMultipleMessage) fullname: " << (const char *) msgFullName->getLiteral()->data << std::endl;
#endif

    TTObject *thiz = env->getField(TO_TT_STR("this"));

    TTObject *msgDestValue = evaluate(msgDestExpr, env);

    std::string fullName = (char *) msgFullName->getLiteral()->data;
    std::vector<std::string> names;
    std::vector<TTObject *> valuesExpressions;
    std::vector<TTObject *> values;

    uint32_t len = (uint32_t) (msgNameArray->getLiteral()->length / sizeof(TTObject *));
    for(uint32_t i = 0; i < len; i++)
    {
        names.push_back((char *) ((TTObject **) msgNameArray->getLiteral()->data)[i]->getLiteral()->data);
#ifdef DEBUG
        std::cout << "(evaluateMultipleMessage): arg name " << i << " = " << names[i] << std::endl;
#endif
        valuesExpressions.push_back(((TTObject **) msgValueArray->getLiteral()->data)[i]);
    }

#ifdef DEBUG
    std::cout << "(evaluateMultipleMessage) before eval values env: " << env << std::endl;
#endif

    for(uint32_t i = 0; i < len; i++)
    {
        TTObject *value = evaluate(valuesExpressions[i], env);
        values.push_back(value);
    }

    if(!msgDestValue)
    {
        std::cerr << "(evaluateMultipleMessage): msgDestValue is NULL!" << std::endl;
        throw std::exception();
    }

#ifdef DEBUG
    std::cout << "(evaluateMultipleMessage): msgDestValue: " << msgDestValue << std::endl;
#endif

    // the accepted destValues types are: Object or Expression, the expression is considered
    // an object if it doesn't have the same name

    if(msgDestValue->type == TT_EXPR && msgDestValue->flags == EXPRESSION_BLOCK)
    {
        std::string blockName = (char *) msgDestValue->getField(TO_TT_STR("blockFullName"))->getLiteral()->data;
        if(COMPARE_NAME(blockName.c_str(), fullName.c_str()) == 0)
        {
            return executeMultipleExpression(msgDestValue, msgDestValue, fullName, names, values, thiz); // call named lambda expression
        }

        // no anonymous function can happen here

        // since now we expect that the message is sent to the expression considered object
    }

    return executeMultipleExpressionAtNonExpression(msgDestValue, msgDestValue, fullName, names, values);
}

TTObject *Evaluator::evaluate(TTObject *expression, TTObject *env)
{
#ifdef DEBUG
    std::cout << "(evaluate)" << std::endl;
#endif
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

TTObject *Evaluator::sendSimpleMessage(TTObject *object, std::string &name)
{
#ifdef DEBUG
    std::cout << "(sendSimpleMessage)" << std::endl;
    std::cout << "(sendSimpleMessage) name: " << name << std::endl;
#endif
    return executeSimpleExpressionAtNonExpression(object, object, name);
}
