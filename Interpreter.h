#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter;

#include "TTObject.h"
#include "Tokenizer.h"

class Interpreter
{
private:
    TTObject *globalEnvironment;
    std::shared_ptr<Tokenizer> tokenizer;

public:
    Interpreter(std::shared_ptr<Reader> &reader);
    ~Interpreter();

    void startInterpreting();
};

#endif
