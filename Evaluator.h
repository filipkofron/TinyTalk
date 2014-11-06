#ifndef EVALUATOR_H
#define EVALUATOR_H

class Evaluator;

#include "TTObject.h"

class Evaluator
{
private:
    TTObject *executeSimpleExpression(TTObject *expression, TTObject *dest, std::string &msgName, TTObject *thiz);
    TTObject *executeSimpleNativeMessage(std::string &nativeName, TTObject *dest, std::string &msgName, TTObject *thiz);
    TTObject *executeSimpleExpressionAtNonExpression(TTObject *object, TTObject *dest, std::string &name);
    TTObject *evaluateSimpleMessage(TTObject *simpleMessage, TTObject *env);

    TTObject *evaluateSymbolValueInThis(TTObject *thiz, std::string &name);
    TTObject *evaluateSymbolValue(TTObject *symbolValue, TTObject *env);

    TTObject *evaluateBlock(TTObject *block, TTObject *env);
    TTObject *evaluateParenthesis(TTObject *parenthesis, TTObject *env);
    TTObject *evaluateAssign(TTObject *expr, TTObject *env);
    TTObject *evaluateCreateVariables(TTObject *expr, TTObject *env);
    TTObject *evaluateChained(TTObject *expr, TTObject *env);
    TTObject *evaluateLiteralValue(TTObject *expr, TTObject *env);

    /*TTObject *sendMultipleMessageToNonExpression(std::string &msgFullName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *dest, TTObject *env);
    TTObject *executeMultipleMessage(TTObject *blockExpression, std::string &msgFullName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *env, TTObject *thiz);
    TTObject *evaluateMultipleMessage(TTObject *expr, TTObject *env);*/

    TTObject *executeMultipleExpression(TTObject *expression, TTObject *dest, std::string &msgName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *thiz);
    TTObject *executeMultipleNativeMessage(std::string &nativeName, TTObject *dest, std::string &msgName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *thiz);
    TTObject *executeMultipleExpressionAtNonExpression(TTObject *object, TTObject *dest, std::string &name, std::vector<std::string> &argNames, std::vector<TTObject *> values);
    TTObject *evaluateMultipleMessage(TTObject *simpleMessage, TTObject *env);

public:
    TTObject *evaluate(TTObject *expression, TTObject *env);
    TTObject *sendSimpleMessage(TTObject *object, std::string &name);
};

#endif
