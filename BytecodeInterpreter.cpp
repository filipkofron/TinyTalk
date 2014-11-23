#include "BytecodeInterpreter.h"
#include "common.h"
#include "Runtime.h"

void createVar(BytecodeInterpreter &bi)
{
    std::cout << "<createVar>" << std::endl;
    TTObject *nameObj = (TTObject *) bi.stack.popPtr();
    std::string name = (const char *) nameObj->getLiteral()->data;
    bi.env->addField(TO_TT_STR(name.c_str()), bi.nil);
    bi.stack.pushPtr((intptr_t)  bi.nil);
}

void loadInteger(BytecodeInterpreter &bi)
{
    std::cout << "<loadInteger>" << std::endl;
    bi.stack.pushPtr(*(intptr_t *) (intptr_t) &bi.byteCode[bi.pc]);
    bi.pc += sizeof(intptr_t);
}

void loadString(BytecodeInterpreter &bi)
{
    std::cout << "<loadString>" << std::endl;
    std::cout << "<loadString> string: *(intptr_t *) (intptr_t) &bi.byteCode[bi.pc]: '" << (const char *)((TTObject *)(*(intptr_t *) (intptr_t) &bi.byteCode[bi.pc]))->getLiteral()->data<< "'" << std::endl;
    bi.stack.pushPtr(*(intptr_t *) (intptr_t) &bi.byteCode[bi.pc]);
    bi.pc += sizeof(intptr_t);
}

void loadSymbol(BytecodeInterpreter &bi)
{
    std::cout << "<loadSymbol>" << std::endl;
    TTObject *nextEnv = bi.env;
    TTObject *val = NULL;
    TTObject *name = (TTObject *) bi.stack.popPtr();
    do
    {
        val = Runtime::findSymbol(TO_TT_STR(name->getLiteral()->data), nextEnv, &nextEnv);
    } while (!val && nextEnv);

    if (!val)
    {
        std::cerr << "Cannot find symbol: " << (const char *) name->getLiteral()->data << std::endl;
        throw std::exception();
    }

    bi.stack.pushPtr((intptr_t) val);
}

void saveSymbol(BytecodeInterpreter &bi)
{
    std::cout << "<saveSymbol>" << std::endl;
    TTObject *name = (TTObject *) bi.stack.popPtr();
    TTObject *val = (TTObject *) bi.stack.popPtr();

    std::string nameStr = (char *) name->getLiteral()->data;

    TTObject *nextEnv = bi.env;

    TTObject *res = NULL;

    do
    {
        res = Runtime::assignSymbol(TO_TT_STR(nameStr.c_str()), val, nextEnv, &nextEnv);
    } while (!res && nextEnv);

    bi.stack.pushPtr((intptr_t) res);
}

void loadBlock(BytecodeInterpreter &bi)
{
    std::cout << "<loadBlock>" << std::endl;
    TTObject *block = (TTObject *) *(intptr_t *) (intptr_t) &bi.byteCode[bi.pc];
    bi.pc += sizeof(intptr_t);
    TTObject *newBlock = TTObject::clone(block);

    bool success = newBlock->addField(TO_TT_STR("blockEnv"), bi.env);
    if(!success)
    {
        newBlock->setField(TO_TT_STR("blockEnv"), bi.env);
    }
    bi.stack.pushPtr((intptr_t) newBlock);
}

void sendSimple(BytecodeInterpreter &bi)
{
    std::cout << "<sendSimple>" << std::endl;
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
        return;
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
    std::cout << "<sendMultiple>" << std::endl;
    TTObject *argCountObj = (TTObject *) bi.stack.popPtr();
    int32_t argCount = *(int32_t *) argCountObj->getLiteral()->data;
    TTObject *fullNameObj = (TTObject *) bi.stack.popPtr();
    TTObject *dest = (TTObject *) bi.stack.popPtr();

    std::string fullName = (const char *) fullNameObj->getLiteral()->data;

    TTObject *thiz = NULL;

    if(dest->type == TT_LITERAL)
    {
        std::vector<std::string> argNames(argCount);
        std::vector<TTObject *> argValues(argCount);
        for(int32_t i = 0; i < argCount; i++)
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
        for(int32_t i = 0; i < argCount; i++)
        {
            argValues[i] = (TTObject *) bi.stack.popPtr();
            TTObject *argName = (TTObject *) bi.stack.popPtr();
            argNames[i] = (const char *) argName->getLiteral()->data;
        }
        TTObject *res = Runtime::executeMultipleNativeMessage(nativeName, dest, fullName, argNames, argValues, thiz);
        bi.stack.pushPtr((intptr_t) res);
        return;
    }

    for(int32_t i = 0; i < argCount; i++)
    {
        TTObject *argVal = (TTObject *) bi.stack.popPtr();
        TTObject *argName = (TTObject *) bi.stack.popPtr();

        std::string name = (const char *) argName->getLiteral()->data;

        std::cout << "arg name: '" << name << "'" << std::endl;

        bi.env->addField(TO_TT_STR(name.c_str()), argVal);
    }

    TTObject *pc = bi.stackFrame->getField(TO_TT_STR("pc"));
    *((uint32_t *) pc->getLiteral()->data) = bi.pc;
    bi.stack.pushPtr((intptr_t) bi.stackFrame);
    TTObject *blockEnv = expression->getField(TO_TT_STR("blockEnv"));
    bi.setupStackFrame(expression, blockEnv, thiz);
    bi.bindStackFrame();
}

void pop(BytecodeInterpreter &bi)
{
    bi.stack.popPtr();
}

void BytecodeInterpreter::bindStackFrame()
{
    std::cout << "(<bindStackFrame>)" << std::endl;
    TTObject *byteCodeObj = stackFrame->getField(TO_TT_STR("byteCode"));
    byteCode = byteCodeObj->getLiteral()->data;
    pcMax = byteCodeObj->getLiteral()->length;
    pc = *((uint32_t *) (stackFrame->getField(TO_TT_STR("pc")))->getLiteral()->data);
    env = stackFrame->getField(TO_TT_STR("env"));
}

void BytecodeInterpreter::setupStackFrame(TTObject *block, TTObject *parentEnv, TTObject *thiz)
{
    std::cout << "(<setupStackFrame>)" << std::endl;
    stackFrame = TTObject::createObject(TT_STACK_FRAME);

    TTObject *byteCodeObj = block->getField(TO_TT_STR("blockByteCode"));
    if(!byteCodeObj)
    {
        std::cout << "(<setupStackFrame>): compiling" << std::endl;
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
}

TTObject *BytecodeInterpreter::interpret(TTObject *block, TTObject *env, TTObject *thiz)
{
    std::cout << " --> Interpreting!" << std::endl;
    setupStackFrame(block, env, thiz);
    bindStackFrame();

    // hack so that we have correct env
    this->env = env;
    stackFrame->setField(TO_TT_STR("env"), env);

    void (*instr)(BytecodeInterpreter &bi);

    do
    {
        std::cout << "[LOOP]: start pc: " << pc << " pcMax: " << pcMax << std::endl;
        while (pc < pcMax)
        {
            *(intptr_t *) &instr = *(intptr_t *) &byteCode[pc];
            std::cout << "load instr at pc: " << pc << std::endl;
            pc += sizeof(intptr_t);
            std::cout << "executing with: " << pc << std::endl;
            (*instr)(*this);

            std::cout << "[LOOP]: SP size now: " << (stack.len - stack.sp) << std::endl;
        }
        std::cout << "[LOOP]: END pc: " << pc << " pcMax: " << pcMax << std::endl;

        if(stack.sp > stack.len - 2 * sizeof(intptr_t))
        {
            break;
        }
        intptr_t retVal = stack.popPtr();
        stackFrame = (TTObject *) stack.popPtr();
        stack.pushPtr(retVal);
        bindStackFrame();
    } while(true);

    return (TTObject *) stack.popPtr();
}
