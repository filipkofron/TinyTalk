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
    static RefPtr<TTObject> globalEnvironment;

    static RefPtr<TTObject> findSymbolValueInThis(RefPtr<TTObject> thiz, const uint8_t *name, RefPtr<TTObject> &nextThis);
    static RefPtr<TTObject> findSymbol(const uint8_t *name, RefPtr<TTObject> env, RefPtr<TTObject> &nextEnv);

    static RefPtr<TTObject> assignSymbol(const uint8_t *safeName, RefPtr<TTObject> assignedVal, RefPtr<TTObject> env, RefPtr<TTObject> &nextEnv);

    static TTObject *findBlockAtNonExpression(TTObject *object, const uint8_t *safeName, TTObject **next);
    static RefPtr<TTObject> findBlock(const uint8_t *safeName, TTObject *obj, TTObject *env, TTObject **thiz);

    static RefPtr<TTObject> executeSimpleNativeMessage(std::string &nativeName, RefPtr<TTObject> dest, std::string &msgName, RefPtr<TTObject> thiz);
    static RefPtr<TTObject> executeMultipleNativeMessage(std::string &nativeName, RefPtr<TTObject> dest, std::string &msgName, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> thiz);

    static void runCopyGC();
};

#endif
