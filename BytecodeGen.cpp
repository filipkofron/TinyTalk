#include "TTLiteral.h"
#include "BytecodeGen.h"
#include "Common.h"
#include "BytecodeInterpreter.h"
#include "Runtime.h"

void BytecodeGen::putTTObj(RefPtr<TTObject> obj, std::vector<uint8_t> &byteCode)
{
    TTObject *bobj = &obj;
    for(uint32_t i = 0; i < sizeof(uintptr_t); i++)
    {
        uint8_t val = ((uint8_t *) &bobj)[i];
        byteCode.push_back(val);
    }
}

void BytecodeGen::putInstr(void (*instr)(BytecodeInterpreter &bi), std::vector<uint8_t> &byteCode)
{
    for(uint32_t i = 0; i < sizeof(uintptr_t); i++)
    {
        byteCode.push_back(((uint8_t *)&instr)[i]);
    }
}

void BytecodeGen::genSymbolVal(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEBUG
    std::cout << "{genSymbolVal}" << std::endl;
#endif
    putInstr(loadString, byteCode);
#ifdef DEBUG
    std::cout << "{genSymbolVal} symbolName: ";
    (expr->getField(TO_TT_STR("symbolName")))->print(std::cout, 0, false);
    std::cout << std::endl;
#endif
    putTTObj(expr->getField(TO_TT_STR("symbolName")), byteCode);
    putInstr(loadSymbol, byteCode);
}

void BytecodeGen::genSimpleMessage(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEBUG
    std::cout << "{genSimpleMessage}" << std::endl;
#endif
    putInstr(loadString, byteCode);
    putTTObj(expr->getField(TO_TT_STR("msgName")), byteCode);
    RefPtr<TTObject> destExpr = expr->getField(TO_TT_STR("msgDestExpr"));
    gen(destExpr, byteCode);
    putInstr(sendSimple, byteCode);
}

void BytecodeGen::genAssign(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEBUG
    std::cout << "{genAssign}" << std::endl;
#endif
    RefPtr<TTObject> assignedExpr = expr->getField(TO_TT_STR("assignExpression"));
    gen(assignedExpr, byteCode);
    putInstr(loadString, byteCode);
    putTTObj(expr->getField(TO_TT_STR("assignSymbolName")), byteCode);
    putInstr(saveSymbol, byteCode);
}

void BytecodeGen::genMultipleMessage(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEBUG
    std::cout << "{genMultipleMessage}" << std::endl;
#endif
    RefPtr<TTObject> msgDestExpr = expr->getField(TO_TT_STR("msgDestExpr"));
    RefPtr<TTObject> msgFullName = expr->getField(TO_TT_STR("msgFullName"));
    RefPtr<TTObject> msgNameArray = expr->getField(TO_TT_STR("msgNameArray"));
    RefPtr<TTObject> msgValueArray = expr->getField(TO_TT_STR("msgValueArray"));

    int32_t len = (int32_t) (msgNameArray->getLiteral()->length / sizeof(TTObject *));

    for(int32_t i = len - 1; i >= 0; i--)
    {
        RefPtr<TTObject> argName = ((TTObject **) msgNameArray->getLiteral()->data)[i];
        RefPtr<TTObject> argExpr = ((TTObject **) msgValueArray->getLiteral()->data)[i];

        putInstr(loadString, byteCode);
#ifdef DEBUG
        std::cout << "{genMultipleMessage}: argname: '" << (const char *) argName->getLiteral()->data << "'" << std::endl;
#endif
        putTTObj(argName, byteCode);

        gen(argExpr, byteCode);
    }

    gen(msgDestExpr, byteCode);

    putInstr(loadString, byteCode);
    putTTObj(msgFullName, byteCode);

    putInstr(loadInteger, byteCode);

    RefPtr<TTObject> lenObj = TTLiteral::createIntegerLiteral(len);
    putTTObj(lenObj, byteCode);

    putInstr(sendMultiple, byteCode);
}

void BytecodeGen::genCreateVar(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEBUG
    std::cout << "{genCreateVar}" << std::endl;
#endif
    RefPtr<TTObject> varNames = expr->getField(TO_TT_STR("varNames"));
    RefPtr<TTLiteral> varNamesLit = varNames->getLiteral();

    size_t len = varNamesLit->length / sizeof(TTObject *);

    for(size_t i = 0; i < len; i++)
    {
        RefPtr<TTObject> name = ((TTObject **) varNamesLit->data)[i];
#ifdef DEBUG
        std::cout << "{genCreateVar} for name ='" << (const char *) name->getLiteral()->data << "'" << std::endl;
#endif
        putInstr(loadString, byteCode);
        putTTObj(name, byteCode);
        putInstr(createVar, byteCode);
        if(i < len - 1)
        {
            putInstr(pop, byteCode);
        }
    }
}

void BytecodeGen::genBlock(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEBUG
    std::cout << "{genBlock}" << std::endl;
#endif
    putInstr(loadBlock, byteCode);
    putTTObj(expr, byteCode);
}

void BytecodeGen::genChained(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEBUG
    std::cout << "{genChained}" << std::endl;
#endif
    RefPtr<TTObject> currExpr = expr->getField(TO_TT_STR("currExpr"));
    RefPtr<TTObject> nextExpr = expr->getField(TO_TT_STR("nextExpr"));

    gen(currExpr, byteCode);

    if(!&nextExpr || nextExpr->type == TT_NIL)
    {
        return;
    }

    putInstr(pop, byteCode);

    gen(nextExpr, byteCode);
}

void BytecodeGen::genLiteralValue(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEBUG
    std::cout << "{genLiteralValue}" << std::endl;
#endif
    RefPtr<TTObject> obj = expr->getField(TO_TT_STR("literalValue"));
    RefPtr<TTLiteral> lit = obj->getLiteral();

    if(lit->type == LITERAL_TYPE_INTEGER)
    {
        putInstr(loadInteger, byteCode);
        putTTObj(obj, byteCode);
        return;
    }

    if(lit->type == LITERAL_TYPE_STRING)
    {
        putInstr(loadString, byteCode);
        putTTObj(obj, byteCode);
        return;
    }

    std::cerr << "[BytecodeGen]: Unknown literal type!!" << std::endl;
}

void BytecodeGen::genParenthesis(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEBUG
    std::cout << "{genParenthesis}" << std::endl;
#endif
    RefPtr<TTObject> innerExpr = expr->getField(TO_TT_STR("innerExpr"));
    gen(innerExpr, byteCode);
}

void BytecodeGen::genArray(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEBUG
    std::cout << "{genArray}" << std::endl;
#endif
    RefPtr<TTObject> expressions = expr->getField(TO_TT_STR("expressions"));
    RefPtr<TTLiteral> array = expressions->getLiteral();

    size_t len = array->length / sizeof(TTObject *);

    for(size_t i = 0; i < len; i++)
    {
        RefPtr<TTObject> innerExpr = ((TTObject **) array->data)[i];

        gen(innerExpr, byteCode);
    }

    RefPtr<TTObject> sizeObj = TTLiteral::createIntegerLiteral((uint32_t) len);

    putInstr(loadInteger, byteCode);
    putTTObj(sizeObj, byteCode);

    putInstr(loadArray, byteCode);
}

void BytecodeGen::gen(RefPtr<TTObject> expr, std::vector<uint8_t> &byteCode)
{
    if(&expr == NULL || expr->type == TT_NIL)
    {
        putInstr(push, byteCode);
        putTTObj(Runtime::globalEnvironment->getField(TO_TT_STR("nil")), byteCode);
        return;
    }
    switch (expr->flags)
    {
        case EXPRESSION_SYMBOL_VALUE:
            genSymbolVal(expr, byteCode);
            break;
        case EXPRESSION_SIMPLE_MESSAGE:
            genSimpleMessage(expr, byteCode);
            break;
        case EXPRESSION_ASSIGN:
            genAssign(expr, byteCode);
            break;
        case EXPRESSION_CREATE_VARIABLES:
            genCreateVar(expr, byteCode);
            break;
        case EXPRESSION_BLOCK:
            genBlock(expr, byteCode);
            break;
        case EXPRESSION_CHAINED:
            genChained(expr, byteCode);
            break;
        case EXPRESSION_LITERAL_VALUE:
            genLiteralValue(expr, byteCode);
            break;
        case EXPRESSION_MULTIPLE_MESSAGE:
            genMultipleMessage(expr, byteCode);
            break;
        case EXPRESSION_PARENTHESIS:
            genParenthesis(expr, byteCode);
            break;
        case EXPRESSION_ARRAY:
            genArray(expr, byteCode);
            break;
        default:
            std::cerr << "[BytecodeGen]: Invalid expression" << std::endl;
            throw std::exception();
    }
}

RefPtr<TTObject> BytecodeGen::generate(RefPtr<TTObject> expression)
{
#ifdef DEBUG
    std::cout << "(generate)" << std::endl;
#endif
    byteCode.clear();

    gen(expression, byteCode);

    RefPtr<TTObject> res = TTLiteral::createByteArray(byteCode);
#ifdef VERBOSE
    std::cout << "#generated bytecode of size: " << byteCode.size() << std::endl;
#endif

    res->getLiteral()->collectiblePtrs = 1;

    return res;
}

void BytecodeGen::runGC(MemAllocator *oldMem, MemAllocator *newMem)
{
    uint32_t lenMax = (uint32_t) byteCode.size();
    for (uint32_t i = 0; i < lenMax / sizeof(TTObject *); i++)
    {
#ifdef DEBUG
                std::cout << "GC: BC_gen: i/pcMax: " << i << "/" << pcMax << std::endl;
#endif
        //&(((TTObject **) byteCode.data())[i])
        TTObject::_gc_COPY_copy(&(((TTObject **) byteCode.data())[i]), oldMem, newMem);
    }
}
