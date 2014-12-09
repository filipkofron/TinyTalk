#include "TTLiteral.h"
#include "common.h"
#include "Expression.h"

namespace Expression
{
    RefPtr<TTObject> createSimpleMessage(RefPtr<TTObject> destExpr, RefPtr<TTLiteral> name)
    {
        RefPtr<TTObject> expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_SIMPLE_MESSAGE;

        RefPtr<TTObject> msgName = TTObject::createObject(TT_LITERAL);
        msgName->setLiteral(name);

        expr->addField(TO_TT_STR("msgName"), msgName);
        expr->addField(TO_TT_STR("msgDestExpr"), destExpr);

        return expr;
    }

    RefPtr<TTObject> createMultipleMessage(RefPtr<TTObject> destExpr, RefPtr<TTLiteral> fullName, RefPtr<TTLiteral> nameArray, RefPtr<TTLiteral> valueArray)
    {
        RefPtr<TTObject> expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_MULTIPLE_MESSAGE;

        RefPtr<TTObject> msgFullName = TTObject::createObject(TT_LITERAL);
        msgFullName->setLiteral(fullName);

        RefPtr<TTObject> msgNameArray = TTObject::createObject(TT_LITERAL);
        msgNameArray->setLiteral(nameArray);

        RefPtr<TTObject> msgValueArray = TTObject::createObject(TT_LITERAL);
        msgValueArray->setLiteral(valueArray);

        expr->addField(TO_TT_STR("msgDestExpr"), destExpr);
        expr->addField(TO_TT_STR("msgFullName"), msgFullName);
        expr->addField(TO_TT_STR("msgNameArray"), msgNameArray);
        expr->addField(TO_TT_STR("msgValueArray"), msgValueArray);

        return expr;
    }

    RefPtr<TTObject> createSymbolValue(RefPtr<TTLiteral> name)
    {
        RefPtr<TTObject> expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_SYMBOL_VALUE;

        RefPtr<TTObject> symbolName = TTObject::createObject(TT_LITERAL);
        symbolName->setLiteral(name);

        expr->addField(TO_TT_STR("symbolName"), symbolName);

        return expr;
    }

    RefPtr<TTObject> createAssignment(RefPtr<TTLiteral> name, RefPtr<TTObject> rightExpr)
    {
        RefPtr<TTObject> expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_ASSIGN;

        RefPtr<TTObject> symbolName = TTObject::createObject(TT_LITERAL);
        symbolName->setLiteral(name);

        expr->addField(TO_TT_STR("assignSymbolName"), symbolName);
        expr->addField(TO_TT_STR("assignExpression"), rightExpr);

        return expr;
    }

    RefPtr<TTObject> createLiteralValue(RefPtr<TTLiteral> value)
    {
        RefPtr<TTObject> expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_LITERAL_VALUE;

        RefPtr<TTObject> literalValue = TTObject::createObject(TT_LITERAL);
        literalValue->setLiteral(value);

        expr->addField(TO_TT_STR("literalValue"), literalValue);

        return expr;
    }

    RefPtr<TTObject> createParenthesis(RefPtr<TTObject> innerExpr)
    {
        RefPtr<TTObject> expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_PARENTHESIS;

        expr->addField(TO_TT_STR("innerExpr"), innerExpr);

        return expr;
    }

    RefPtr<TTObject> createBlock(RefPtr<TTLiteral> nameArray, RefPtr<TTLiteral> fullName, RefPtr<TTObject> expr, RefPtr<TTLiteral> nativeName)
    {
        RefPtr<TTObject> res = TTObject::createObject(TT_EXPR);

        res->flags = EXPRESSION_BLOCK;

        RefPtr<TTObject> blockArgNames = TTObject::createObject(TT_LITERAL);
        blockArgNames->setLiteral(nameArray);

        RefPtr<TTObject> blockFullName = TTObject::createObject(TT_LITERAL);
        blockFullName->setLiteral(fullName);

        res->addField(TO_TT_STR("blockArgNames"), blockArgNames);
        res->addField(TO_TT_STR("blockFullName"), blockFullName);
        res->addField(TO_TT_STR("blockExpr"), expr);

        if(&nativeName)
        {
            RefPtr<TTObject> blockNativeName = TTObject::createObject(TT_LITERAL);
            blockNativeName->setLiteral(nativeName);

            res->addField(TO_TT_STR("blockNativeName"), blockNativeName);
        }

        return res;
    }

    RefPtr<TTObject> createNaiveBlock(RefPtr<TTObject> expr)
    {
        RefPtr<TTObject> res = TTObject::createObject(TT_EXPR);

        res->flags = EXPRESSION_BLOCK;

        RefPtr<TTObject> blockArgNames = TTLiteral::createObjectArray(0);
        RefPtr<TTObject> blockFullName = TTLiteral::createStringLiteral(TO_TT_STR(""));

        res->addField(TO_TT_STR("blockArgNames"), blockArgNames);
        res->addField(TO_TT_STR("blockFullName"), blockFullName);
        res->addField(TO_TT_STR("blockExpr"), expr);

        return res;
    }

    RefPtr<TTObject> createChained(RefPtr<TTObject> currExpr, RefPtr<TTObject> nextExpr)
    {
        RefPtr<TTObject> expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_CHAINED;

        expr->addField(TO_TT_STR("currExpr"), currExpr);
        expr->addField(TO_TT_STR("nextExpr"), nextExpr);

        return expr;
    }

    RefPtr<TTObject> createCreateVariables(RefPtr<TTLiteral> varNames)
    {
        RefPtr<TTObject> expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_CREATE_VARIABLES;

        RefPtr<TTObject> varNamesLit = TTObject::createObject(TT_LITERAL);
        varNamesLit->setLiteral(varNames);

        expr->addField(TO_TT_STR("varNames"), varNamesLit);

        return expr;
    }

    RefPtr<TTObject> createArray(RefPtr<TTLiteral> expressions)
    {
        RefPtr<TTObject> expr = TTObject::createObject(TT_EXPR);

        expr->flags = EXPRESSION_ARRAY;

        RefPtr<TTObject> varNamesLit = TTObject::createObject(TT_LITERAL);
        varNamesLit->setLiteral(expressions);

        expr->addField(TO_TT_STR("expressions"), varNamesLit);

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
            case EXPRESSION_ARRAY:
                return "ARRAY";
            default:
                return "INVALID";
        }
    }
}
