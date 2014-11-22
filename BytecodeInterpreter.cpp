#include "BytecodeInterpreter.h"
#include "common.h"
#include "Runtime.h"

void createVar(BytecodeInterpreter &bi)
{
    bi.env->addField(TO_TT_STR(bi.stack.popPtr()), bi.nil);
}

void loadInteger(BytecodeInterpreter &bi)
{
    bi.stack.pushInt(*(int32_t *)(intptr_t)&bi.byteCode[bi.pc]);
    bi.pc += sizeof(int32_t);
}

void loadString(BytecodeInterpreter &bi)
{
    bi.stack.pushPtr(*(intptr_t *)(intptr_t)&bi.byteCode[bi.pc]);
    bi.pc += sizeof(intptr_t);
}

void loadSymbol(BytecodeInterpreter &bi)
{
    TTObject *nextEnv = bi.env;
    TTObject *val = NULL;
    TTObject *name = (TTObject *) bi.stack.popPtr();
    do
    {
        val = Runtime::findSymbol(TO_TT_STR(name->getLiteral()->data), nextEnv, &nextEnv);
    } while(!val && nextEnv);

    if(!val)
    {
        std::cerr << "Cannot find symbol: " << (const char *) name->getLiteral()->data << std::endl;
        throw std::exception();
    }

    bi.stack.pushPtr((intptr_t) val);
}

void loadBlock(BytecodeInterpreter &bi)
{
    TTObject *block = (TTObject *) bi.stack.popPtr();
    TTObject *newBlock = TTObject::clone(block);

    bool success = newBlock->addField(TO_TT_STR("blockEnv"), bi.env);
    if(!success)
    {
        newBlock->setField(TO_TT_STR("blockEnv"), bi.env);
    }
    bi.stack.pushPtr((intptr_t)  newBlock);
}

void sendSimple(BytecodeInterpreter &bi)
{
    TTObject *dest = (TTObject *) bi.stack.popPtr();
    TTObject *name = (TTObject *) bi.stack.popPtr();

    TTObject *thiz = NULL;
    std::string safeName = (const char *) name->getLiteral()->data;

    if(dest->type == TT_LITERAL)
    {
        std::vector<std::string> singleList;
        singleList.push_back(safeName);
        TTObject *res = dest->getLiteral()->onMessage(dest, safeName, singleList, std::vector<TTObject *>());
        bi.stack.pushPtr((intptr_t) res);
        return;
    }

    TTObject *expression = Runtime::findBlock(TO_TT_STR(safeName.c_str()), dest, bi.env, &thiz);
    TTObject *blockNativeName = expression->getField(TO_TT_STR("blockNativeName"));
    if(blockNativeName)
    {
        std::string nativeName = (char *) blockNativeName->getLiteral()->data;
        TTObject *res = Runtime::executeSimpleNativeMessage(nativeName, dest, safeName, thiz);
        bi.stack.pushPtr((intptr_t) res);
    }

    TTObject *pc = bi.stackFrame->getField(TO_TT_STR("pc"));
    *((uint32_t *) pc->getLiteral()->data) = bi.pc;
    bi.stack.pushPtr((intptr_t) bi.stackFrame);
    TTObject *blockEnv = expression->getField(TO_TT_STR("blockEnv"));
    bi.setupStackFrame(expression, blockEnv, thiz);
    bi.bindStackFrame();
}

void sendMultiple(BytecodeInterpreter &bi)
{
    TTObject *argCountObj = (TTObject *) bi.stack.popPtr();
    TTObject *fullNameObj = (TTObject *) bi.stack.popPtr();
    TTObject *dest= (TTObject *) bi.stack.popPtr();

    std::string fullName = (const char *) fullNameObj->getLiteral()->data;
    uint32_t argCount = *(uint32_t *) argCountObj->getLiteral()->data;

    TTObject *thiz = NULL;

    if(dest->type == TT_LITERAL)
    {
        std::vector<std::string> argNames(argCount);
        std::vector<TTObject *> argValues(argCount);
        for(uint32_t i = 0; i < argCount; i++)
        {
            argValues[i] = (TTObject *) bi.stack.popPtr();
            TTObject *argName = (TTObject *) bi.stack.popPtr();
            argNames[i] = (const char *) argName->getLiteral()->data;
        }
        TTObject *res = dest->getLiteral()->onMessage(dest, fullName, argNames, argValues);
        bi.stack.pushPtr((intptr_t) res);
        return;
    }

    TTObject *expression = Runtime::findBlock(TO_TT_STR(fullName.c_str()), dest, bi.env, &thiz);
    TTObject *blockNativeName = expression->getField(TO_TT_STR("blockNativeName"));

    if(blockNativeName)
    {
        std::vector<std::string> argNames(argCount);
        std::vector<TTObject *> argValues(argCount);
        std::string nativeName = (char *) blockNativeName->getLiteral()->data;
        for(uint32_t i = 0; i < argCount; i++)
        {
            argValues[i] = (TTObject *) bi.stack.popPtr();
            TTObject *argName = (TTObject *) bi.stack.popPtr();
            argNames[i] = (const char *) argName->getLiteral()->data;
        }
        TTObject *res = Runtime::executeMultipleNativeMessage(nativeName, dest, fullName, argNames, argValues, thiz);
        bi.stack.pushPtr((intptr_t) res);
        return;
    }

    for(uint32_t i = 0; i < argCount; i++)
    {
        TTObject *argVal = (TTObject *) bi.stack.popPtr();
        TTObject *argName = (TTObject *) bi.stack.popPtr();

        std::string name = (const char *) argName->getLiteral()->data;

        bi.env->addField(TO_TT_STR(name.c_str()), argVal);
    }

    TTObject *pc = bi.stackFrame->getField(TO_TT_STR("pc"));
    *((uint32_t *) pc->getLiteral()->data) = bi.pc;
    bi.stack.pushPtr((intptr_t) bi.stackFrame);
    TTObject *blockEnv = expression->getField(TO_TT_STR("blockEnv"));
    bi.setupStackFrame(expression, blockEnv, thiz);
    bi.bindStackFrame();
}

void BytecodeInterpreter::bindStackFrame()
{
    TTObject *byteCodeObj = stackFrame->getField(TO_TT_STR("byteCode"));
    byteCode = byteCodeObj->getLiteral()->data;
    pcMax = byteCodeObj->getLiteral()->length;
    pc =*((uint32_t *) (stackFrame->getField(TO_TT_STR("pc")))->getLiteral()->data);
    env = stackFrame->getField(TO_TT_STR("env"));
}

void BytecodeInterpreter::setupStackFrame(TTObject *block, TTObject *parentEnv, TTObject *thiz)
{
    stackFrame = TTObject::createObject(TT_STACK_FRAME);

    TTObject *byteCodeObj = block->getField(TO_TT_STR("blockByteCode"));
    if(!byteCodeObj)
    {
        byteCodeObj = Runtime::bytecodeGen.generate(block->getField(TO_TT_STR("blockExpr")));
        block->setField(TO_TT_STR("blockByteCode"), byteCodeObj);
    }

    stackFrame->addField(TO_TT_STR("byteCode"), byteCodeObj);

    TTObject *newEnv = TTObject::createObject(TT_ENV);
    newEnv->addField(TO_TT_STR("parentEnv"), parentEnv);

    stackFrame->addField(TO_TT_STR("env"), newEnv);

    TTObject *pc = TTObject::createObject(TT_LITERAL);
    TTLiteral *pcLit = TTLiteral::createByteArray(sizeof(uint32_t));
    *((uint32_t *)pcLit->data) = 0;
    pc->setLiteral(pcLit);
    stackFrame->addField(TO_TT_STR("pc"), pc);

    if(thiz)
    {
        newEnv->addField(TO_TT_STR("this"), thiz);
    }
}

BytecodeInterpreter::BytecodeInterpreter()
    : byteCode(NULL), pc(0), pcMax(0)
{
    nil = Runtime::globalEnvironment->getField(TO_TT_STR("nil"));
}

BytecodeInterpreter::~BytecodeInterpreter()
{
    delete [] byteCode;
}

TTObject *BytecodeInterpreter::interpret(TTObject *block, TTObject *env, TTObject *thiz)
{
    setupStackFrame(block, env, thiz);
    bindStackFrame();

    void (*instr)(BytecodeInterpreter &bi);

    while(pc < pcMax)
    {
        *(uintptr_t *) &instr = *(uintptr_t *) byteCode;
        byteCode += sizeof(uintptr_t);
        (*instr)(*this);
    }

    return (TTObject *) stack.popPtr();
}
