#ifndef EVALUATOR_H
#define EVALUATOR_H

class Evaluator;



class Evaluator
{
private:

public:
    TTObject *evaluate(TTObject *expression, TTObject *env);
};

#endif
