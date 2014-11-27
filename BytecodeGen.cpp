#include "BytecodeGen.h"
#include "common.h"
#include "BytecodeInterpreter.h"
#include "Runtime.h"

void BytecodeGen::putTTObj(TTObject *obj, std::vector<uint8_t> &byteCode)
{
    for(uint32_t i = 0; i < sizeof(intptr_t); i++)
    {
        byteCode.push_back(((uint8_t *)&obj)[i]);
    }
}

void BytecodeGen::putInstr(void (*instr)(BytecodeInterpreter &bi), std::vector<uint8_t> &byteCode)
{
    for(uint32_t i = 0; i < sizeof(intptr_t); i++)
    {
        byteCode.push_back(((uint8_t *)&instr)[i]);
    }
}

void BytecodeGen::genSymbolVal(TTObject *expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEFINE
    std::cout << "{genSymbolVal}" << std::endl;
#endif
    putInstr(loadString, byteCode);
    putTTObj(expr->getField(TO_TT_STR("symbolName")), byteCode);
    putInstr(loadSymbol, byteCode);
}

void BytecodeGen::genSimpleMessage(TTObject *expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEFINE
    std::cout << "{genSimpleMessage}" << std::endl;
#endif
    putInstr(loadString, byteCode);
    putTTObj(expr->getField(TO_TT_STR("msgName")), byteCode);
    TTObject *destExpr = expr->getField(TO_TT_STR("msgDestExpr"));
    gen(destExpr, byteCode);
    putInstr(sendSimple, byteCode);
}

void BytecodeGen::genAssign(TTObject *expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEFINE
    std::cout << "{genAssign}" << std::endl;
#endif
    TTObject *assignedExpr = expr->getField(TO_TT_STR("assignExpression"));
    gen(assignedExpr, byteCode);
    putInstr(loadString, byteCode);
    putTTObj(expr->getField(TO_TT_STR("assignSymbolName")), byteCode);
    putInstr(saveSymbol, byteCode);
}

void BytecodeGen::genMultipleMessage(TTObject *expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEFINE
    std::cout << "{genMultipleMessage}" << std::endl;
#endif
    TTObject *msgDestExpr = expr->getField(TO_TT_STR("msgDestExpr"));
    TTObject *msgFullName = expr->getField(TO_TT_STR("msgFullName"));
    TTObject *msgNameArray = expr->getField(TO_TT_STR("msgNameArray"));
    TTObject *msgValueArray = expr->getField(TO_TT_STR("msgValueArray"));

    int32_t len = (int32_t) (msgNameArray->getLiteral()->length / sizeof(TTObject *));

    for(int32_t i = len - 1; i >= 0; i--)
    {
        TTObject *argName = ((TTObject **) msgNameArray->getLiteral()->data)[i];
        TTObject *argExpr = ((TTObject **) msgValueArray->getLiteral()->data)[i];

        putInstr(loadString, byteCode);
#ifdef DEFINE
        std::cout << "{genMultipleMessage}: argname: '" << (const char *) argName->getLiteral()->data << "'" << std::endl;
#endif
        putTTObj(argName, byteCode);

        gen(argExpr, byteCode);
    }

    gen(msgDestExpr, byteCode);

    putInstr(loadString, byteCode);
    putTTObj(msgFullName, byteCode);

    putInstr(loadInteger, byteCode);

    TTObject *lenObj = TTLiteral::createIntegerLiteral(len);
    putTTObj(lenObj, byteCode);

    putInstr(sendMultiple, byteCode);
}

void BytecodeGen::genCreateVar(TTObject *expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEFINE
    std::cout << "{genCreateVar}" << std::endl;
#endif
    TTObject *varNames = expr->getField(TO_TT_STR("varNames"));
    TTLiteral *varNamesLit = varNames->getLiteral();

    size_t len = varNamesLit->length / sizeof(TTObject *);

    for(size_t i = 0; i < len; i++)
    {
        TTObject *name = ((TTObject **) varNamesLit->data)[i];
#ifdef DEFINE
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

void BytecodeGen::genBlock(TTObject *expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEFINE
    std::cout << "{genBlock}" << std::endl;
#endif
    putInstr(loadBlock, byteCode);
    putTTObj(expr, byteCode);
}

void BytecodeGen::genChained(TTObject *expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEFINE
    std::cout << "{genChained}" << std::endl;
#endif
    TTObject *currExpr = expr->getField(TO_TT_STR("currExpr"));
    TTObject *nextExpr = expr->getField(TO_TT_STR("nextExpr"));

    gen(currExpr, byteCode);

    if(!nextExpr || nextExpr->type == TT_NIL)
    {
        return;
    }

    putInstr(pop, byteCode);

    gen(nextExpr, byteCode);
}

void BytecodeGen::genLiteralValue(TTObject *expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEFINE
    std::cout << "{genLiteralValue}" << std::endl;
#endif
    TTObject *obj = expr->getField(TO_TT_STR("literalValue"));
    TTLiteral *lit = obj->getLiteral();

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

void BytecodeGen::genParenthesis(TTObject *expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEFINE
    std::cout << "{genParenthesis}" << std::endl;
#endif
    TTObject *innerExpr = expr->getField(TO_TT_STR("innerExpr"));
    gen(innerExpr, byteCode);
}

void BytecodeGen::genArray(TTObject *expr, std::vector<uint8_t> &byteCode)
{
#ifdef DEFINE
    std::cout << "{genArray}" << std::endl;
#endif
    TTObject *expressions = expr->getField(TO_TT_STR("expressions"));
    TTLiteral *array = expressions->getLiteral();

    size_t len = array->length / sizeof(TTObject *);

    for(size_t i = 0; i < len; i++)
    {
        TTObject *innerExpr = ((TTObject **) array->data)[i];

        gen(innerExpr, byteCode);
    }

    TTObject *sizeObj = TTLiteral::createIntegerLiteral((uint32_t) len);

    putInstr(loadInteger, byteCode);
    putTTObj(sizeObj, byteCode);

    putInstr(loadArray, byteCode);
}

void BytecodeGen::gen(TTObject *expr, std::vector<uint8_t> &byteCode)
{
    if(expr == NULL || expr->type == TT_NIL)
    {
        putInstr(push, byteCode);
        putTTObj(Runtime::globalEnvironment->getField(TO_TT_STR("nul")), byteCode);
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

TTObject *BytecodeGen::generate(TTObject *expression)
{
#ifdef DEBUG
    std::cout << "(generate)" << std::endl;
#endif
    std::vector<uint8_t> byteCode;

    gen(expression, byteCode);

    TTObject *res = TTLiteral::createByteArray(byteCode);
#ifdef VERBOSE
    std::cout << "#generated bytecode of size: " << byteCode.size() << std::endl;
#endif

    return res;
}
