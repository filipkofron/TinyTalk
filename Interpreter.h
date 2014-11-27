#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter;

#include "TTObject.h"
#include "Tokenizer.h"
#include "BuiltinPool.h"

class Interpreter
{
private:
    BuiltinPool pool;


    void initialize();
    void setupObject();
    void setupLiterals();
    void loadTTLib();
public:
    Interpreter();
    ~Interpreter();

    void interpretFile(std::istream &is, bool silent);
    void interpretCommandLine(std::istream &is);
};

#endif
