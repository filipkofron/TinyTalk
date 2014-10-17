#include "common.h"
#include "Expression.h"

namespace Expression
{
    TTObject *createSimpleMessage(TTObject *destExpr, TTLiteral *name)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_FLAG_SIMPLE_MESSAGE;

        TTObject *msgName = TTObject::createObject(TT_LITERAL);
        msgName->setLiteral(name);

        expr->addField(TO_TT_STR("msgName"), msgName);
        expr->addField(TO_TT_STR("msgDestExpr"), destExpr);

        return expr;
    }

    TTObject *createMultipleMessage(TTObject *destExpr, TTLiteral *fullName, TTLiteral *nameArray, TTLiteral *valueArray)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_FLAG_SIMPLE_MESSAGE;

        TTObject *msgFullName = TTObject::createObject(TT_LITERAL);
        msgFullName->setLiteral(fullName);

        TTObject *msgNameArray = TTObject::createObject(TT_LITERAL);
        msgNameArray->setLiteral(nameArray);

        TTObject *msgValueArray = TTObject::createObject(TT_LITERAL);
        msgNameArray->setLiteral(valueArray);

        expr->addField(TO_TT_STR("msgDestExpr"), destExpr);
        expr->addField(TO_TT_STR("msgFullName"), msgFullName);
        expr->addField(TO_TT_STR("msgNameArray"), msgNameArray);
        expr->addField(TO_TT_STR("msgValueArray"), msgValueArray);

        return expr;
    }

    TTObject *createSymbolValue(TTLiteral *name)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_FLAG_SYMBOL_VALUE;

        TTObject *symbolName = TTObject::createObject(TT_LITERAL);
        symbolName->setLiteral(name);

        return expr;
    }

    TTObject *createAssignment(TTLiteral *name, TTObject *rightExpr)
    {
        TTObject *expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_FLAG_ASSIGN;

        TTObject *symbolName = TTObject::createObject(TT_LITERAL);
        symbolName->setLiteral(name);

        expr->addField(TO_TT_STR("assignSymbolName"), symbolName);
        expr->addField(TO_TT_STR("assignExpression"), rightExpr);

        return expr;
    }

    const char *getTypeInfo(TTObject *expr)
    {
        switch(expr->flags)
        {
            case EXPRESSION_FLAG_SIMPLE_MESSAGE:
                return "SIMPLE MESSAGE";
            case EXPRESSION_FLAG_SYMBOL_VALUE:
                return "SYMBOL VALUE";
            case EXPRESSION_FLAG_ASSIGN:
                return "ASSIGN";
            case EXPRESSION_FLAG_CHAINED:
                return "CHAINED";
            case EXPRESSION_FLAG_MULTIPLE_MESSAGE:
                return "MULTIPLE MESSAGE";
            case EXPRESSION_FLAG_PARENTHESIS:
                return "PARENTHESIS";
            default:
                return "INVALID";
        }
    }
}
