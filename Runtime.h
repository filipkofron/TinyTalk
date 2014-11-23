#ifndef RUNTIME_H
#define RUNTIME_H

struct Runtime;

#include "BuiltinPool.h"
#include "RefPtrMap.h"
#include "BytecodeGen.h"

struct Runtime
{
    static BuiltinPool builtinPool;
    static RefPtrMap refPtrMap;
    static BytecodeGen bytecodeGen;
    static TTObject *globalEnvironment;

    static TTObject *findSymbolValueInThis(TTObject *thiz, const uint8_t *name, TTObject **nextThis);
    static TTObject *findSymbol(const uint8_t *name, TTObject *env, TTObject **nextEnv);

    static TTObject *assignSymbol(const uint8_t *safeName, TTObject *assignedVal, TTObject *env, TTObject **nextEnv);

    static TTObject *findBlockAtNonExpression(TTObject *object, const uint8_t *safeName, TTObject **next);
    static TTObject *findBlock(const uint8_t *safeName, TTObject *obj, TTObject *env, TTObject **thiz);

    static TTObject *executeSimpleNativeMessage(std::string &nativeName, TTObject *dest, std::string &msgName, TTObject *thiz);
    static TTObject *executeMultipleNativeMessage(std::string &nativeName, TTObject *dest, std::string &msgName, std::vector<std::string> &argNames, std::vector<TTObject *> values, TTObject *thiz);
};

#endif
