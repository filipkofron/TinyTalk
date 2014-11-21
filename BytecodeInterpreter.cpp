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

    TTObject *pc = bi.stackFrame->getField(TO_TT_STR("pc")));
    *((uint32_t *) pc->getLiteral()->data) = bi.pc;
    bi.stack.pushPtr((intptr_t) bi.stackFrame);
    TTObject *blockEnv = expression->getField(TO_TT_STR("blockEnv"));
    bi.setupStackFrame(expression, blockEnv, thiz);
}

void sendMultiple(BytecodeInterpreter &bi)
{
    // TODO: Wizardy shit.
}

void BytecodeInterpreter::setupStackFrame(TTObject *block, TTObject *parentEnv, TTObject *thiz)
{
    stackFrame = TTObject::createObject(TT_STACK_FRAME);

    TTObject *byteCode = block->getField(TO_TT_STR("blockByteCode"));
    if(!byteCode)
    {
        byteCode = Runtime::bytecodeGen.generate(block->getField(TO_TT_STR("blockExpr")));
        block->setField(TO_TT_STR("blockByteCode"), byteCode);
    }

    stackFrame->addField(TO_TT_STR("byteCode"), byteCode);

    TTObject *env = TTObject::createObject(TT_ENV);
    env->addField(TO_TT_STR("parentEnv"), parentEnv);

    stackFrame->addField(TO_TT_STR("env"), env);

    TTObject *pc = TTObject::createObject(TT_LITERAL);
    TTLiteral *pcLit = TTLiteral::createByteArray(sizeof(uint32_t));
    *((uint32_t *)pcLit->data) = 0;
    pc->setLiteral(pcLit);
    stackFrame->addField(TO_TT_STR("pc"), pc);

    if(thiz)
    {
        env->addField(TO_TT_STR("this"), thiz);
    }
}

BytecodeInterpreter::BytecodeInterpreter(TTObject *block)
    : byteCode(NULL), pc(0)
{
    nil = Runtime::globalEnvironment->getField(TO_TT_STR("nil"));
    // TODO: setup
}

BytecodeInterpreter::~BytecodeInterpreter()
{
    delete [] byteCode;
}