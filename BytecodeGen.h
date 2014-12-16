#ifndef BYTECODEGEN_H
#define BYTECODEGEN_H

class BytecodeGen;

#include "TTObject.h"
#include "Expression.h"
#include "BytecodeInterpreter.h"

class BytecodeGen
{
private:
    std::vector<uint8_t> byteCode;

    void putTTObj(RefPtr<TTObject> obj, std::vector<uint8_t> &byteCode);
    void putInstr(void (*instr)(BytecodeInterpreter &bi), std::vector<uint8_t> &byteCode);
    void genSymbolVal(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode);
    void genSimpleMessage(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode);
    void genAssign(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode);
    void genMultipleMessage(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode);
    void genCreateVar(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode);
    void genBlock(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode);
    void genChained(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode);
    void genLiteralValue(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode);
    void genParenthesis(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode);
    void genArray(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode);
    void gen(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode);
public:
    RefPtr<TTObject> generate(RefPtr<TTObject> expression);
    void runGC(MemAllocator *oldMem, MemAllocator *newMem);
};

#endif
