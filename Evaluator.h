#ifndef EVALUATOR_H
#define EVALUATOR_H

class Evaluator;

#include "TTObject.h"

class Evaluator
{
private:
    RefPtr<TTObject> executeSimpleExpression(RefPtr<TTObject> expression, RefPtr<TTObject> dest, std::string &msgName, RefPtr<TTObject> thiz);
    RefPtr<TTObject> evaluateSimpleMessage(RefPtr<TTObject> simpleMessage, RefPtr<TTObject> env);

    RefPtr<TTObject> evaluateSymbolValue(RefPtr<TTObject> symbolValue, RefPtr<TTObject> env);

    RefPtr<TTObject> evaluateBlock(RefPtr<TTObject> block, RefPtr<TTObject> env);
    RefPtr<TTObject> evaluateParenthesis(RefPtr<TTObject> parenthesis, RefPtr<TTObject> env);
    RefPtr<TTObject> evaluateAssign(RefPtr<TTObject> expr, RefPtr<TTObject> env);
    RefPtr<TTObject> evaluateCreateVariables(RefPtr<TTObject> expr, RefPtr<TTObject> env);
    RefPtr<TTObject> evaluateChained(RefPtr<TTObject> expr, RefPtr<TTObject> env);
    RefPtr<TTObject> evaluateLiteralValue(RefPtr<TTObject> expr, RefPtr<TTObject> env);

    RefPtr<TTObject> executeMultipleExpression(RefPtr<TTObject> expression, RefPtr<TTObject> dest, std::string &msgName, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> thiz);
    RefPtr<TTObject> evaluateMultipleMessage(RefPtr<TTObject> simpleMessage, RefPtr<TTObject> env);

    RefPtr<TTObject> evaluateArray(RefPtr<TTObject> expr, RefPtr<TTObject> env);

public:
    RefPtr<TTObject> evaluate(RefPtr<TTObject> expression, RefPtr<TTObject> env);
    RefPtr<TTObject> sendSimpleMessage(RefPtr<TTObject> object, std::string &name);
};

#endif
