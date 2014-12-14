#ifndef BYTECODEINTERPRETER_H
#define BYTECODEINTERPRETER_H

class BytecodeInterpreter;

#include "Stack.h"
#include "TTObject.h"

void createVar(BytecodeInterpreter &bi);
void loadInteger(BytecodeInterpreter &bi);
void loadString(BytecodeInterpreter &bi);
void loadSymbol(BytecodeInterpreter &bi);
void saveSymbol(BytecodeInterpreter &bi);
void loadBlock(BytecodeInterpreter &bi);
void sendSimple(BytecodeInterpreter &bi);
void sendMultiple(BytecodeInterpreter &bi);
void loadArray(BytecodeInterpreter &bi);
void pop(BytecodeInterpreter &bi);
void push(BytecodeInterpreter &bi);

class BytecodeInterpreter
{
    friend void createVar(BytecodeInterpreter &bi);
    friend void loadInteger(BytecodeInterpreter &bi);
    friend void loadString(BytecodeInterpreter &bi);
    friend void loadSymbol(BytecodeInterpreter &bi);
    friend void saveSymbol(BytecodeInterpreter &bi);
    friend void loadBlock(BytecodeInterpreter &bi);
    friend void sendSimple(BytecodeInterpreter &bi);
    friend void sendMultiple(BytecodeInterpreter &bi);
    friend void loadArray(BytecodeInterpreter &bi);
    friend void pop(BytecodeInterpreter &bi);
    friend void push(BytecodeInterpreter &bi);
private:
    Stack stack;

    void setupStackFrame(RefPtr<TTObject> block, RefPtr<TTObject> parentEnv, RefPtr<TTObject> thiz);
    void bindStackFrame();
private:
    RefPtr<TTObject> env;
    uint8_t *byteCode;
    RefPtr<TTObject> stackFrame;
    RefPtr<TTObject> nil;
    uint32_t pc;
    uint32_t pcMax;
public:
    BytecodeInterpreter();
    ~BytecodeInterpreter();
    void refreshAfterGC();
    void test(MemAllocator *oldMem, MemAllocator *newMem);
    void runGC(MemAllocator *oldMem, MemAllocator *newMem);
    RefPtr<TTObject> interpret(RefPtr<TTObject> block, RefPtr<TTObject> env, RefPtr<TTObject> thiz);
};

#endif
