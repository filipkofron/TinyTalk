#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter;

#include "TTObject.h"
#include "Tokenizer.h"
#include "BuiltinPool.h"

class Interpreter
{
private:
    TTObject *globalEnvironment;
    std::shared_ptr<Tokenizer> tokenizer;
    BuiltinPool pool;

public:
    Interpreter(std::shared_ptr<Reader> &reader);
    void initialize();
    void setupObject();
    ~Interpreter();

    void startInterpreting();
};

#endif
