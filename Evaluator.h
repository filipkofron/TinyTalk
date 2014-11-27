#ifndef EVALUATOR_H
#define EVALUATOR_H

class Evaluator;

#include "TTObject.h"

class Evaluator
{
private:
    TTObject *executeSimpleExpression(TTObject *expression, TTObject *dest, std::string &msgName, TTObject *thiz);
    TTObject *evaluateSimpleMessage(TTObject *simpleMessage, TTObject *env);

    TTObject *evaluateSymbolValue(TTObject *symbolValue, TTObject *env);

    TTObject *evaluateBlock(TTObject *block, TTObject *env);
    TTObject *evaluateParenthesis(TTObject *parenthesis, TTObject *env);
    TTObject *evaluateAssign(TTObject *expr, TTObject *env);
    TTObject *evaluateCreateVariables(TTObject *expr, TTObject *env);
    TTObject *evaluateChained(TTObject *expr, TTObject *env);
    TTObject *evaluateLiteralValue(TTObject *expr, TTObject *env);

    TTObject *executeMultipleExpression(TTObject *expression, TTObject *dest, std::string &msgName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *thiz);
    TTObject *evaluateMultipleMessage(TTObject *simpleMessage, TTObject *env);

    TTObject *evaluateArray(TTObject *expr, TTObject *env);

public:
    TTObject *evaluate(TTObject *expression, TTObject *env);
    TTObject *sendSimpleMessage(TTObject *object, std::string &name);
};

#endif
