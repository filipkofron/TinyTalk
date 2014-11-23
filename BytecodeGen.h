#ifndef BYTECODEGEN_H
#define BYTECODEGEN_H

class BytecodeGen;

#include "TTObject.h"
#include "Expression.h"
#include "BytecodeInterpreter.h"

class BytecodeGen
{
private:
    void putTTObj(TTObject *obj, std::vector<uint8_t> &byteCode);
    void putInstr(void (*instr)(BytecodeInterpreter &bi), std::vector<uint8_t> &byteCode);
    void genSymbolVal(TTObject *expr, std::vector<uint8_t> &byteCode);
    void genSimpleMessage(TTObject *expr, std::vector<uint8_t> &byteCode);
    void genAssign(TTObject *expr, std::vector<uint8_t> &byteCode);
    void genMultipleMessage(TTObject *expr, std::vector<uint8_t> &byteCode);
    void genCreateVar(TTObject *expr, std::vector<uint8_t> &byteCode);
    void genBlock(TTObject *expr, std::vector<uint8_t> &byteCode);
    void genChained(TTObject *expr, std::vector<uint8_t> &byteCode);
    void genLiteralValue(TTObject *expr, std::vector<uint8_t> &byteCode);
    void genParenthesis(TTObject *expr, std::vector<uint8_t> &byteCode);
    void gen(TTObject *expr, std::vector<uint8_t> &byteCode);
public:
    TTObject *generate(TTObject *expression);
};

#endif
