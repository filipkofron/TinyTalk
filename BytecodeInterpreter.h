#ifndef BYTECODEINTERPRETER_H
#define BYTECODEINTERPRETER_H

class BytecodeInterpreter;

#include "Stack.h"
#include "TTObject.h"

void createVar(BytecodeInterpreter &bi);
void loadInteger(BytecodeInterpreter &bi);
void loadString(BytecodeInterpreter &bi);
void loadSymbol(BytecodeInterpreter &bi);
void loadBlock(BytecodeInterpreter &bi);
void sendSimple(BytecodeInterpreter &bi);
void sendMultiple(BytecodeInterpreter &bi);

class BytecodeInterpreter
{
    friend void createVar(BytecodeInterpreter &bi);
    friend void loadInteger(BytecodeInterpreter &bi);
    friend void loadString(BytecodeInterpreter &bi);
    friend void loadSymbol(BytecodeInterpreter &bi);
    friend void loadBlock(BytecodeInterpreter &bi);
    friend void sendSimple(BytecodeInterpreter &bi);
    friend void sendMultiple(BytecodeInterpreter &bi);
private:
    Stack stack;

    void setupStackFrame(TTObject *block, TTObject *parentEnv, TTObject *thiz);
private:
    TTObject *env;
    uint8_t *byteCode;
    TTObject *stackFrame;
    TTObject *nil;
    uint32_t pc;    // TODO: must be updated in the stackFrame
public:
    BytecodeInterpreter(TTObject *block);
    ~BytecodeInterpreter();
};

#endif
