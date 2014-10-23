#ifndef EVALUATOR_H
#define EVALUATOR_H

class Evaluator;

#include "TTObject.h"

class Evaluator
{
private:
    TTObject *sendSimpleMessageToNonExpression(std::string &simpleMessageName, TTObject *dest, TTObject *env);

    TTObject *executeSimpleMessage(TTObject *expression, std::string &msgName, TTObject *env);

    TTObject *evaluateSimpleMessage(TTObject *simpleMessage, TTObject *env);
    TTObject *evaluateSymbolValue(TTObject *symbolValue, TTObject *env);
    TTObject *evaluateBlock(TTObject *block, TTObject *env);

public:
    TTObject *evaluate(TTObject *expression, TTObject *env);
};

#endif
