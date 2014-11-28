#include "Runtime.h"
#include "common.h"
#include "Expression.h"

BuiltinPool Runtime::builtinPool;
RefPtrMap Runtime::refPtrMap;
BytecodeGen Runtime::bytecodeGen;
TTObject *Runtime::globalEnvironment = NULL;


TTObject *Runtime::findSymbolValueInThis(TTObject *thiz, const uint8_t *name, TTObject **nextThis)
{
    *nextThis = NULL;
    TTObject *resultFromThis = thiz->getField(name);
    if (!resultFromThis)
    {
        TTObject *thisParent = thiz->getField(TO_TT_STR("parent"));
        if(thisParent)
        {
#ifdef DEBUG
            std::cout << "(evaluateSymbolValue): field not found in this, trying parent =>" << std::endl;
#endif
            *nextThis = thisParent;
            return NULL;
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

TTObject *Runtime::findSymbol(const uint8_t *name, TTObject *env, TTObject **nextEnv)
{
    *nextEnv = NULL;
#ifdef DEBUG
        std::cout << "(findSymbol)" << std::endl;
    #endif

#ifdef DEBUG
        std::cout << "(findSymbol) env:" << std::endl;
        env->print(std::cout, 0, false);
        env->getField(TO_TT_STR("parentEnv"))->print(std::cout, 0, false);
#endif

#ifdef DEBUG
        std::cout << "(findSymbol): of name '" << name << "'" << std::endl;
    #endif

    TTObject *resultFromEnv = env->getField(name);
    if (!resultFromEnv)
    {
#ifdef DEBUG
            std::cout << "(findSymbol): field name '"
                    << name << "' not found in environment" << std::endl;
    #endif
    }
    else
    {
#ifdef DEBUG
            std::cout << "(findSymbol): field name '"
                    << name << "' found in environment" << std::endl;

            std::cout << "res: " << resultFromEnv << std::endl;
    #endif
        return resultFromEnv;
    }

    TTObject *thiz = env->getField(TO_TT_STR("this"));
    if (!thiz)
    {
#ifdef DEBUG
            std::cout << "(findSymbol): this is NULL" << std::endl;
    #endif
    }
    else
    {
        TTObject *resultFromThis = NULL;
        TTObject *nextThis = thiz;
        do
        {
            resultFromThis = findSymbolValueInThis(nextThis, name, &nextThis);
        } while(!resultFromThis && nextThis);
        if (!resultFromThis)
        {
#ifdef DEBUG
                std::cout << "(findSymbol): field not found in this" << std::endl;
    #endif
        }
        else
        {
#ifdef DEBUG
                std::cout << "(findSymbol): field was found in this" << std::endl;
    #endif
            return resultFromThis;
        }
    }

    TTObject *parent = env->getField(TO_TT_STR("parentEnv"));
    if (parent && parent->type == TT_ENV)
    {
#ifdef DEBUG
            std::cout << "(findSymbol): trying parent env" << std::endl;
    #endif

        *nextEnv = parent;
        return NULL;
    }

    return NULL;
}

TTObject *Runtime::assignSymbol(const uint8_t *safeName, TTObject *assignedVal, TTObject *env, TTObject **nextEnv)
{
    *nextEnv = NULL;
#ifdef DEBUG
    std::cout << "(assignSymbol)" << std::endl;
#endif

    if (env->hasField(safeName))
    {
        env->setField(safeName, assignedVal);
        return assignedVal;
    }
    else
    {
#ifdef DEBUG
        std::cout << "(assignSymbol): field not found in environment" << std::endl;
#endif
    }

    TTObject *thiz = env->getField(TO_TT_STR("this"));
    if (!thiz)
    {
        std::cout << "(assignSymbol): this is NULL" << std::endl;
    }
    else
    {
        if (thiz->hasField(safeName))
        {
            thiz->setField(safeName, assignedVal);
            return assignedVal;
        }
        else
        {
#ifdef DEBUG
            std::cout << "(assignSymbol): field was found in this" << std::endl;
#endif
        }
    }

    TTObject *parent = env->getField(TO_TT_STR("parentEnv"));
    if (parent && parent->type == TT_ENV)
    {
#ifdef DEBUG
        std::cout << "(assignSymbol): trying parent env" << std::endl;
#endif
        *nextEnv = parent;
        return NULL;
    }

    std::cerr << "(assignSymbol): field not found in anywhere (not going to parent), sorry" << std::endl;
    throw std::exception();
}

TTObject *Runtime::findBlockAtNonExpression(TTObject *object, const uint8_t *safeName, TTObject **next)
{
#ifdef DEBUG
    std::cout << "(findBlockAtNonExpression)" << std::endl;
#endif
    TTObject *fieldVal = object->getField(safeName);
    if(fieldVal)
    {
        if(fieldVal->type != TT_EXPR || fieldVal->flags != EXPRESSION_BLOCK)
        {
            std::cerr << "(findBlockAtNonExpression): Error: NON EXPRESSION FIELD: " << fieldVal << std::endl;
            throw std::exception();
        }
        return fieldVal;
    }

    TTObject *parent = object->getField(TO_TT_STR("parent"));

    if(!parent)
    {
        std::cerr << "(findBlockAtNonExpression): Object doesn't understand this message '" << (const char *) safeName << "'" << std::endl;
        throw std::exception();
    }

    // Now, either the field is in this object or in its parents, nowhere else!
    *next = parent;
    return NULL;
}

TTObject *Runtime::findBlock(const uint8_t *safeName, TTObject *obj, TTObject *env, TTObject **thiz)
{
#ifdef DEBUG
    std::cout << "(findBlock)" << std::endl;
#endif
    *thiz = NULL;

    TTObject *envThiz = env->getField(TO_TT_STR("this"));

    // the accepted obj types are: Object or Expression, the expression is considered
    // an object if it doesn't have the same name

    if(obj->type == TT_EXPR && obj->flags == EXPRESSION_BLOCK)
    {

        std::string blockName = (char *) obj->getField(TO_TT_STR("blockFullName"))->getLiteral()->data;
        if(COMPARE_NAME(blockName.c_str(), safeName) == 0)
        {
            *thiz = envThiz;
            return obj;
        }
        if(blockName.empty() && COMPARE_NAME(safeName, TO_TT_STR("value")) == 0)
        {
            *thiz = envThiz;
            return obj;
        }
        size_t argNamesCount = obj->getField(TO_TT_STR("blockArgNames"))->getLiteral()->length / sizeof(TTObject *);
        if(argNamesCount == 1 && COMPARE_NAME(safeName, TO_TT_STR("value:")) == 0) // we ignore the name and put the value is to be put in the single argument
        {
            *thiz = envThiz;
            return obj;
        }

        // since now we expect that the message is sent to the expression considered object
    }

    TTObject *next = obj;
    TTObject *resExpr = NULL;
    do
    {
        resExpr = findBlockAtNonExpression(next, safeName, &next);
    } while(!resExpr && next);

    *thiz = obj;

    return resExpr;
}

TTObject *Runtime::executeSimpleNativeMessage(std::string &nativeName, TTObject *dest, std::string &msgName, TTObject *thiz)
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

TTObject *Runtime::executeMultipleNativeMessage(std::string &nativeName, TTObject *dest, std::string &msgName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *thiz)
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