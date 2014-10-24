#ifndef EVALUATOR_H
#define EVALUATOR_H

class Evaluator;

#include "TTObject.h"

class Evaluator
{
private:
    TTObject *sendSimpleMessageToNonExpression(std::string &simpleMessageName, TTObject *dest, TTObject *env);

    TTObject *executeSimpleMessage(TTObject *expression, std::string &msgName, TTObject *env, TTObject *thiz);

    TTObject *evaluateSimpleMessage(TTObject *simpleMessage, TTObject *env);
    TTObject *evaluateSymbolValue(TTObject *symbolValue, TTObject *env);
    TTObject *evaluateBlock(TTObject *block, TTObject *env);
    TTObject *evaluateParenthesis(TTObject *parenthesis, TTObject *env);
    TTObject *evaluateAssign(TTObject *expr, TTObject *env);
    TTObject *evaluateCreateVariables(TTObject *expr, TTObject *env);
    TTObject *evaluateChained(TTObject *expr, TTObject *env);
    TTObject *evaluateLiteralValue(TTObject *expr, TTObject *env);

    TTObject *sendMultipleMessageToNonExpression(std::string &msgFullName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *dest, TTObject *env);

    TTObject *executeMultipleMessage(TTObject *blockExpression, std::string &msgFullName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *env, TTObject *thiz);

    TTObject *evaluateMultipleMessage(TTObject *expr, TTObject *env);

public:
    TTObject *evaluate(TTObject *expression, TTObject *env);
};

#endif
