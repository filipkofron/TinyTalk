#include "common.h"
#include "Expression.h"

namespace Expression
{
    TTObject *createSimpleMessage(TTObject *destExpr, TTLiteral *name)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_SIMPLE_MESSAGE;

        TTObject *msgName = TTObject::createObject(TT_LITERAL);
        msgName->setLiteral(name);

        expr->addField(TO_TT_STR("msgName"), msgName);
        expr->addField(TO_TT_STR("msgDestExpr"), destExpr);

        return expr;
    }

    TTObject *createMultipleMessage(TTObject *destExpr, TTLiteral *fullName, TTLiteral *nameArray, TTLiteral *valueArray)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_MULTIPLE_MESSAGE;

        TTObject *msgFullName = TTObject::createObject(TT_LITERAL);
        msgFullName->setLiteral(fullName);

        TTObject *msgNameArray = TTObject::createObject(TT_LITERAL);
        msgNameArray->setLiteral(nameArray);

        TTObject *msgValueArray = TTObject::createObject(TT_LITERAL);
        msgValueArray->setLiteral(valueArray);

        expr->addField(TO_TT_STR("msgDestExpr"), destExpr);
        expr->addField(TO_TT_STR("msgFullName"), msgFullName);
        expr->addField(TO_TT_STR("msgNameArray"), msgNameArray);
        expr->addField(TO_TT_STR("msgValueArray"), msgValueArray);

        return expr;
    }

    TTObject *createSymbolValue(TTLiteral *name)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_SYMBOL_VALUE;

        TTObject *symbolName = TTObject::createObject(TT_LITERAL);
        symbolName->setLiteral(name);

        expr->addField(TO_TT_STR("symbolName"), symbolName);

        return expr;
    }

    TTObject *createAssignment(TTLiteral *name, TTObject *rightExpr)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_ASSIGN;

        TTObject *symbolName = TTObject::createObject(TT_LITERAL);
        symbolName->setLiteral(name);

        expr->addField(TO_TT_STR("assignSymbolName"), symbolName);
        expr->addField(TO_TT_STR("assignExpression"), rightExpr);

        return expr;
    }

    TTObject *createLiteralValue(TTLiteral *value)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_LITERAL_VALUE;

        TTObject *literalValue = TTObject::createObject(TT_LITERAL);
        literalValue->setLiteral(value);

        expr->addField(TO_TT_STR("literalValue"), literalValue);

        return expr;
    }

    TTObject *createParenthesis(TTObject *innerExpr)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_PARENTHESIS;

        expr->addField(TO_TT_STR("innerExpr"), innerExpr);

        return expr;
    }

    TTObject *createBlock(TTLiteral *nameArray, TTLiteral *fullName, TTObject *expr)
    {
        TTObject *res = TTObject::createObject(TT_EXPR);

        res->flags = EXPRESSION_BLOCK;

        TTObject *blockArgNames = TTObject::createObject(TT_LITERAL);
        blockArgNames->setLiteral(nameArray);

        TTObject *blockFullName = TTObject::createObject(TT_LITERAL);
        blockFullName->setLiteral(fullName);

        res->addField(TO_TT_STR("blockArgNames"), blockArgNames);
        res->addField(TO_TT_STR("blockFullName"), blockFullName);
        res->addField(TO_TT_STR("blockExpr"), expr);

        return res;
    }

    TTObject *createChained(TTObject *currExpr, TTObject *nextExpr)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_CHAINED;

        expr->addField(TO_TT_STR("currExpr"), currExpr);
        expr->addField(TO_TT_STR("nextExpr"), nextExpr);

        return expr;
    }

    TTObject *createCreateVariables(TTLiteral *varNames)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_CREATE_VARIABLES;

        TTObject *varNamesLit = TTObject::createObject(TT_LITERAL);
        varNamesLit->setLiteral(varNames);

        expr->addField(TO_TT_STR("varNames"), varNamesLit);

        return expr;
    }

    const char *getTypeInfo(TTObject *expr)
    {
        switch(expr->flags)
        {
            case EXPRESSION_SIMPLE_MESSAGE:
                return "SIMPLE MESSAGE";
            case EXPRESSION_SYMBOL_VALUE:
                return "SYMBOL VALUE";
            case EXPRESSION_ASSIGN:
                return "ASSIGN";
            case EXPRESSION_CHAINED:
                return "CHAINED";
            case EXPRESSION_MULTIPLE_MESSAGE:
                return "MULTIPLE MESSAGE";
            case EXPRESSION_PARENTHESIS:
                return "PARENTHESIS";
            case EXPRESSION_LITERAL_VALUE:
                return "LITERAL VALUE";
            case EXPRESSION_BLOCK:
                return "BLOCK";
            case EXPRESSION_CREATE_VARIABLES:
                return "CREATE VARIABLES";
            default:
                return "INVALID";
        }
    }
}
