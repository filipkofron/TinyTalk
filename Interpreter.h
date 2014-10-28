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
    BuiltinPool pool;


    void initialize();
    void setupObject();
    void loadTTLib();
    void addSimpleMethod(TTObject *dest, const std::string &msgName, const std::string &buitlinName);
    void addMultipleMethod(TTObject *dest, const std::string &msgName, const std::vector<std::string> &msgArgs, const std::string &buitlinName);
public:
    Interpreter();
    ~Interpreter();

    void interpretFile(std::istream &is);
    void interpretCommandLine(std::istream &is);
};

#endif
