#include <cstdint>
#include "TTLiteral.h"
#include "BytecodeInterpreter.h"
#include "Common.h"
#include "Runtime.h"

void createVar(BytecodeInterpreter &bi)
{
#ifdef DEBUG
    std::cout << "<createVar>" << std::endl;
#endif
    RefPtr<TTObject> nameObj = (TTObject *) bi.stack.popPtr();
    std::string name = (const char *) nameObj->getLiteral()->data;
    bi.env->addField(TO_TT_STR(name.c_str()), bi.nil);
    bi.stack.pushPtr((uintptr_t) &bi.nil);
}

void loadInteger(BytecodeInterpreter &bi)
{
#ifdef DEBUG
    std::cout << "<loadInteger>" << std::endl;
#endif
    bi.stack.pushPtr(*(uintptr_t *) (uintptr_t) &bi.byteCode[bi.pc]);
    bi.pc += sizeof(uintptr_t);
}

void loadString(BytecodeInterpreter &bi)
{
#ifdef DEBUG
    std::cout << "<loadString>: addr: " << (unsigned long) *(uintptr_t *) (uintptr_t) &bi.byteCode[bi.pc] << std::endl;
    std::cout << "<loadString>: '" << (const char *)((TTObject *)(*(uintptr_t *) (uintptr_t) &bi.byteCode[bi.pc]))->getLiteral()->data << "'" << std::endl;
#endif
    bi.stack.pushPtr(*(uintptr_t *) (uintptr_t) &bi.byteCode[bi.pc]);
    bi.pc += sizeof(uintptr_t);
}

void loadSymbol(BytecodeInterpreter &bi)
{
#ifdef DEBUG
    std::cout << "<loadSymbol>" << std::endl;
#endif
    RefPtr<TTObject> nextEnv = bi.env;
    RefPtr<TTObject> val = NULL;
    RefPtr<TTObject> name = (TTObject *) bi.stack.popPtr();

    do
    {
        val = Runtime::findSymbol(TO_TT_STR(name->getLiteral()->data), nextEnv, nextEnv);
    } while (!&val && &nextEnv);

    if (!&val)
    {
        std::cerr << "Cannot find symbol: " << (const char *) name->getLiteral()->data << std::endl;
        throw std::exception();
    }

    bi.stack.pushPtr((uintptr_t) &val);
}

void saveSymbol(BytecodeInterpreter &bi)
{
#ifdef DEBUG
    std::cout << "<saveSymbol>" << std::endl;
#endif
    RefPtr<TTObject> name = (TTObject *) bi.stack.popPtr();
    RefPtr<TTObject> val = (TTObject *) bi.stack.popPtr();

    std::string nameStr = (char *) name->getLiteral()->data;

    RefPtr<TTObject> nextEnv = bi.env;

    RefPtr<TTObject> res = NULL;

    do
    {
        res = Runtime::assignSymbol(TO_TT_STR(nameStr.c_str()), val, nextEnv, nextEnv);
    } while (!&res && &nextEnv);

    bi.stack.pushPtr((uintptr_t) &res);
}

// lock so that we generate code only once
Lock genCodeLock;

void loadBlock(BytecodeInterpreter &bi)
{
#ifdef DEBUG
    std::cout << "<loadBlock>" << std::endl;
#endif
    RefPtr<TTObject> block = (TTObject *) *(uintptr_t *) (uintptr_t) &bi.byteCode[bi.pc];
    bi.pc += sizeof(uintptr_t);

    genCodeLock.lock();
    RefPtr<TTObject> byteCodeObj = block->getField(TO_TT_STR("blockByteCode"));
    if(!&byteCodeObj)
    {
#ifdef DEBUG
        std::cout << "(<setupStackFrame>): compiling" << std::endl;
#endif
        byteCodeObj = Runtime::bytecodeGen.generate(block->getField(TO_TT_STR("blockExpr")));
        block->addField(TO_TT_STR("blockByteCode"), byteCodeObj);
    }
    genCodeLock.unlock();

    RefPtr<TTObject> newBlock = TTObject::clone(block);

    bool success = newBlock->addField(TO_TT_STR("blockEnv"), bi.env);
    if(!success)
    {
        newBlock->setField(TO_TT_STR("blockEnv"), bi.env);
    }
    bi.stack.pushPtr((uintptr_t) &newBlock);
}

void sendSimple(BytecodeInterpreter &bi)
{
#ifdef DEBUG
    std::cout << "<sendSimple>" << std::endl;
#endif
    Runtime::gcBarrier.tryCatchAnyWhere();

    RefPtr<TTObject> dest = (TTObject *) bi.stack.popPtr();
    RefPtr<TTObject> name = (TTObject *) bi.stack.popPtr();

    TTObject *thizPtr = NULL;
    std::string safeName = (const char *) name->getLiteral()->data;

#ifdef VERBOSE
    std::cout << "([" << std::this_thread::get_id() << "]*" << safeName <<")" << std::endl;
#endif

    RefPtr<TTObject> expression = Runtime::findBlock(TO_TT_STR(safeName.c_str()), &dest, &bi.env, &thizPtr);

    RefPtr<TTObject> thiz = thizPtr;

    RefPtr<TTObject> blockEnv = expression->getField(TO_TT_STR("blockEnv"));

    RefPtr<TTObject> blockNativeName = expression->getField(TO_TT_STR("blockNativeName"));
    if(&blockNativeName)
    {
        std::string nativeName = (char *) blockNativeName->getLiteral()->data;
        RefPtr<TTObject> res = Runtime::executeSimpleNativeMessage(nativeName, dest, safeName, blockEnv, thiz);
        bi.stack.pushPtr((uintptr_t) &res);
        return;
    }

    RefPtr<TTObject> pc = bi.stackFrame->getField(TO_TT_STR("pc"));
    *((uint32_t *) pc->getLiteral()->data) = bi.pc;
    if(bi.pc < bi.pcMax)
    {
        bi.stack.pushPtr((uintptr_t) &bi.stackFrame);
    }
    bi.setupStackFrame(expression, blockEnv, thiz);
    bi.bindStackFrame();
}

void sendMultiple(BytecodeInterpreter &bi)
{
#ifdef DEBUG
    std::cout << "<sendMultiple>" << std::endl;
#endif
    Runtime::gcBarrier.tryCatchAnyWhere();

    RefPtr<TTObject> argCountObj = (TTObject *) bi.stack.popPtr();
    int32_t argCount = *(int32_t *) argCountObj->getLiteral()->data;
    RefPtr<TTObject> fullNameObj = (TTObject *) bi.stack.popPtr();
    RefPtr<TTObject> dest = (TTObject *) bi.stack.popPtr();

    std::string fullName = (const char *) fullNameObj->getLiteral()->data;

#ifdef VERBOSE
    std::cout << "([" << std::this_thread::get_id() << "]*" << fullName <<")" << std::endl;
#endif

    TTObject *thizPtr = NULL;

    RefPtr<TTObject> expression = Runtime::findBlock(TO_TT_STR(fullName.c_str()), &dest, &bi.env, &thizPtr);
    RefPtr<TTObject> thiz = thizPtr;

    RefPtr<TTObject> blockNativeName = expression->getField(TO_TT_STR("blockNativeName"));

    RefPtr<TTObject> blockEnv = expression->getField(TO_TT_STR("blockEnv"));

    if(&blockNativeName)
    {
        std::vector<std::string> argNames(argCount);
        std::vector<RefPtr<TTObject> > argValues(argCount);
        std::string nativeName = (char *) blockNativeName->getLiteral()->data;
        for(int32_t i = 0; i < argCount; i++)
        {
            argValues[i] = (TTObject *) bi.stack.popPtr();
            RefPtr<TTObject> argName = (TTObject *) bi.stack.popPtr();
            argNames[i] = (const char *) argName->getLiteral()->data;
        }
        if(fullName == "value:")
        {
            argNames[0] = (const char *) ((TTObject **) expression->getField(TO_TT_STR("blockArgNames"))->getLiteral()->data)[0]->getLiteral()->data;
        }
        RefPtr<TTObject> res = Runtime::executeMultipleNativeMessage(nativeName, dest, fullName, argNames, argValues, blockEnv, thiz);
        bi.stack.pushPtr((uintptr_t) &res);
        return;
    }

    RefPtr<TTObject> pc = bi.stackFrame->getField(TO_TT_STR("pc"));
    *((uint32_t *) pc->getLiteral()->data) = bi.pc;
    RefPtr<TTObject> oldStackFrame = bi.stackFrame;

    bi.setupStackFrame(expression, blockEnv, thiz);
    bi.bindStackFrame();

    std::string valueReplacementName;
    bool replace = false;
    if(fullName == "value:")
    {
        valueReplacementName = (const char *) ((TTObject **) expression->getField(TO_TT_STR("blockArgNames"))->getLiteral()->data)[0]->getLiteral()->data;
        replace = true;
    }

    for(int32_t i = 0; i < argCount; i++)
    {
        RefPtr<TTObject> argVal = (TTObject *) bi.stack.popPtr();
        RefPtr<TTObject> argName = (TTObject *) bi.stack.popPtr();

        std::string name = (const char *) argName->getLiteral()->data;
#ifdef DEBUG
        std::cout << "arg name: '" << name << "' val:" << std::endl;
        argVal->print(std::cout, 1, false);
#endif

        if(replace)
        {
            name = valueReplacementName;
        }

        bi.env->addField(TO_TT_STR(name.c_str()), argVal);
    }

    if(bi.pc < bi.pcMax)
    {
        bi.stack.pushPtr((uintptr_t) &oldStackFrame);
    }
}

void loadArray(BytecodeInterpreter &bi)
{
#ifdef DEBUG
    std::cout << "(<loadArray>)" << std::endl;
#endif

    RefPtr<TTObject> sizeObj = (TTObject *) bi.stack.popPtr();
    int32_t size = (int32_t)  *(uint32_t *) sizeObj->getLiteral()->data;

    RefPtr<TTObject> arrayObj = TTLiteral::createObjectArray(size);

    for(int32_t i = size - 1; i >= 0; i--)
    {
        ((TTObject **) arrayObj->getLiteral()->data)[i] = (TTObject *) bi.stack.popPtr();
    }

    bi.stack.pushPtr((uintptr_t) &arrayObj);
}

void pop(BytecodeInterpreter &bi)
{
    bi.stack.popPtr();
}

void push(BytecodeInterpreter &bi)
{
    bi.stack.pushPtr(*(uintptr_t *) (uintptr_t) &bi.byteCode[bi.pc]);
    bi.pc += sizeof(uintptr_t);
}

void BytecodeInterpreter::bindStackFrame()
{
#ifdef DEBUG
    std::cout << "(<bindStackFrame>)" << std::endl;
#endif
    RefPtr<TTObject> byteCodeObj = stackFrame->getField(TO_TT_STR("byteCode"));
    byteCode = byteCodeObj->getLiteral()->data;

    pcMax = byteCodeObj->getLiteral()->length;
    pc = *((uint32_t *) (stackFrame->getField(TO_TT_STR("pc")))->getLiteral()->data);
    env = stackFrame->getField(TO_TT_STR("env"));
}

Lock HAXLOCK;

void BytecodeInterpreter::setupStackFrame(RefPtr<TTObject> block, RefPtr<TTObject> parentEnv, RefPtr<TTObject> thiz)
{
#ifdef DEBUG
    std::cout << "(<setupStackFrame>)" << std::endl;
    std::cout << "(<setupStackFrame>): block:";
    block->print(std::cout, 3, false);
    std::cout << std::endl;
    std::cout << "(<setupStackFrame>): parentEnv:";
    parentEnv->print(std::cout, 3, false);
    std::cout << std::endl;
    if(&thiz)
    {
        std::cout << "(<setupStackFrame>): thiz:";
        thiz->print(std::cout, 3, false);
        std::cout << std::endl;
    }
#endif
    stackFrame = TTObject::createObject(TT_STACK_FRAME);

    genCodeLock.lock();
    RefPtr<TTObject> byteCodeObj = block->getField(TO_TT_STR("blockByteCode"));
    if(!&byteCodeObj)
    {
#ifdef DEBUG
        std::cout << "(<setupStackFrame>): compiling" << std::endl;
#endif
        byteCodeObj = Runtime::bytecodeGen.generate(block->getField(TO_TT_STR("blockExpr")));
        block->addField(TO_TT_STR("blockByteCode"), byteCodeObj);
    }
    genCodeLock.unlock();

#ifdef DEBUG
    std::cout << "print bytecode:" << std::endl;
    byteCodeObj->print(std::cout, 1, false);
    std::cout << "print literal:" << std::endl;
    byteCodeObj->getLiteral()->printValue(std::cout, 1, false);
    std::cout << "print bytecode bytes:" << std::endl;
    print_bytes(byteCodeObj->getLiteral()->length, byteCodeObj->getLiteral()->data);
#endif

    stackFrame->addField(TO_TT_STR("byteCode"), byteCodeObj);

    RefPtr<TTObject> newEnv = TTObject::createObject(TT_ENV);
    newEnv->addField(TO_TT_STR("parentEnv"), parentEnv);

    stackFrame->addField(TO_TT_STR("env"), newEnv);

    RefPtr<TTObject> pc = TTLiteral::createByteArray(sizeof(uint32_t));
    *((uint32_t *)pc->getLiteral()->data) = 0;

    stackFrame->addField(TO_TT_STR("pc"), pc);

    if(&thiz)
    {
        newEnv->addField(TO_TT_STR("this"), thiz);
    }
}

BytecodeInterpreter::BytecodeInterpreter()
    : byteCode(NULL), pc(0), pcMax(0)
{
    Runtime::criticalRuntimeGCLock.lock();
    Runtime::interpretersAlive.insert(this);
    nil = Runtime::globalEnvironment->getField(TO_TT_STR("nil"));
    Runtime::criticalRuntimeGCLock.unlock();
}

BytecodeInterpreter::~BytecodeInterpreter()
{
    Runtime::criticalRuntimeGCLock.lock();
    Runtime::interpretersAlive.erase(this);
    Runtime::criticalRuntimeGCLock.unlock();
}

void BytecodeInterpreter::refreshAfterGC()
{
#ifdef DEBUG
    std::cout << "Refresh after GC, stackframe: " << (unsigned long) &stackFrame << std::endl;
#endif
    TTObject *stackFramePtr = &stackFrame;
    if(stackFramePtr)
    {
        TTObject *tempGCHack = stackFramePtr->getField(TO_TT_STR("byteCode"));
        if(tempGCHack)
        {
            byteCode = tempGCHack->getLiteral()->data;
        }
    }
}

void BytecodeInterpreter::runGC(MemAllocator *oldMem, MemAllocator *newMem)
{
#ifdef DEBUG
    std::cout << "BytecodeInterpreter::runGC ==== BEGIN ====" << std::endl;
#endif
    TTObject *stackFramePtr = &stackFrame;
    TTObject::_gc_COPY_copy(&stackFramePtr, oldMem, newMem);
    if(stackFramePtr)
    {
#ifdef DEBUG
        std::cout << "stackframe: " << (unsigned long) stackFramePtr << std::endl;
        std::cout << "stackframe cnt: " << stackFramePtr->count << std::endl;
        stackFramePtr->print(std::cout, 0, false);
#endif
        TTObject *obj = stackFramePtr->getField(TO_TT_STR("byteCode"));
#ifdef DEBUG
        std::cout << "byteCodeObj: " << (unsigned long) obj << std::endl;
#endif
        if(obj)
        {
            TTLiteral *byteCodeLit = obj->getLiteral();
            for (uint32_t i = 0; i < pcMax / sizeof(TTObject *); i++)
            {
#ifdef DEBUG
                std::cout << "GC: BC: i/pcMax: " << i << "/" << pcMax / sizeof(TTObject *) << std::endl;
#endif
                TTObject::_gc_COPY_copy(&(((TTObject **) byteCodeLit->data)[i]), oldMem, newMem);

#ifdef DEBUG
                test(oldMem, newMem);
#endif
            }
        }
    }
#ifdef DEBUG
    std::cout << "BytecodeInterpreter::runGC ==== END ====" << std::endl;
#endif
    stack.runGC(oldMem, newMem);
}

void BytecodeInterpreter::test(MemAllocator *oldMem, MemAllocator *newMem)
{
#ifdef DEBUG
    std::cout << " --> Test fail" << std::endl;
    TTObject *stackFramePtr = &stackFrame;
   // TTObject::_gc_COPY_copy(&stackFramePtr, oldMem, newMem);
    std::cout << " --> Test fail2" << std::endl;
   // TTObject::_gc_COPY_copy(&stackFramePtr, oldMem, newMem);
    if(stackFramePtr)
    {
        TTObject *byteCodeObj = stackFramePtr->getField(TO_TT_STR("byteCode"));

        std::cout << "byteCodeObj: " << (unsigned long ) byteCodeObj << std::endl;
        if(byteCodeObj)
        {
            TTLiteral *byteCodeLit = byteCodeObj->getLiteral();
            std::cout << "byte code LIT SUCCESS: " << (int) byteCodeLit->type << std::endl;
        }
    }
    std::cout << "#$$$$# before addr: " << (unsigned long ) stackFramePtr << std::endl;
   // TTObject::_gc_COPY_copy(&stackFramePtr, oldMem, newMem);
    std::cout << "#$$$$# after addr: " << (unsigned long ) stackFramePtr << std::endl;
    if(stackFramePtr)
    {
        TTObject *byteCodeObj = stackFramePtr->getField(TO_TT_STR("byteCode"));

        std::cout << "byteCodeObj: " << (unsigned long ) byteCodeObj << std::endl;
        if(byteCodeObj)
        {
            TTLiteral *byteCodeLit = byteCodeObj->getLiteral();
            std::cout << "byte code LIT SUCCESS: " << (int) byteCodeLit->type << std::endl;
        }
    }
#endif
}

RefPtr<TTObject> BytecodeInterpreter::interpret(RefPtr<TTObject> block, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    setupStackFrame(block, env, thiz);
    bindStackFrame();

    // hack so that we have correct env at the beginning
    this->env = env;
    stackFrame->setField(TO_TT_STR("env"), env);

    void (*instr)(BytecodeInterpreter &bi);

    do
    {
        while (pc < pcMax)
        {
            *(uintptr_t *) &instr = *(uintptr_t *) &byteCode[pc];
            pc += sizeof(uintptr_t);

            (*instr)(*this);
        }

        if(stack.sp == sizeof(uintptr_t))
        {
            break;
        }
        uintptr_t retVal = stack.popPtr();
        stackFrame = (TTObject *) stack.popPtr();
        stack.pushPtr(retVal);
        bindStackFrame();
    } while(true);

#ifdef DEBUG
    std::cout << "End sp: " << stack.sp << std::endl;
#endif

    return (TTObject *) stack.popPtr();
}
