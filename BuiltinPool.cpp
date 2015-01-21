#include "BuiltinPool.h"
#include "BuiltinFile.h"
#include "BuiltinInteger.h"
#include "BuiltinObject.h"
#include "BuiltinString.h"
#include "BuiltinObjectArray.h"
#include "BuiltinSystem.h"
#include "BuiltinSocket.h"

BuiltinPool::BuiltinPool()
{
    std::cout << "[BuiltinPool]: initializing" << std::endl;

    registerBultin("object_alloc", std::shared_ptr<Builtin> (new BuiltinObjectAlloc()));
    registerBultin("object_add", std::shared_ptr<Builtin> (new BuiltinObjectAddField));
    registerBultin("object_get", std::shared_ptr<Builtin> (new BuiltinObjectGetter()));
    registerBultin("object_set", std::shared_ptr<Builtin> (new BuiltinObjectSetter()));
    registerBultin("object_getFieldNames", std::shared_ptr<Builtin> (new BuiltinObjectGetFieldNames()));
    registerBultin("object_getFieldValues", std::shared_ptr<Builtin> (new BuiltinObjectGetFieldValues()));
    registerBultin("object_debugprint", std::shared_ptr<Builtin> (new BuiltinObjectDebugPrint));
    registerBultin("object_debugprintrec", std::shared_ptr<Builtin> (new BuiltinObjectDebugPrintRec));
    registerBultin("object_debugprintstring", std::shared_ptr<Builtin> (new BuiltinObjectDebugPrintString));
    registerBultin("object_debuggc", std::shared_ptr<Builtin> (new BuiltinObjectDebugGC));
    registerBultin("object_clone", std::shared_ptr<Builtin> (new BuiltinObjectClone));
    registerBultin("object_new", std::shared_ptr<Builtin> (new BuiltinObjectNew));
    registerBultin("object_tostring", std::shared_ptr<Builtin> (new BuiltinObjectToString));

    registerBultin("integer_add:", std::shared_ptr<Builtin> (new BuiltinIntegerAdd));
    registerBultin("integer_minus:", std::shared_ptr<Builtin> (new BuiltinIntegerMinus));
    registerBultin("integer_mul:", std::shared_ptr<Builtin> (new BuiltinIntegerMul));
    registerBultin("integer_div:", std::shared_ptr<Builtin> (new BuiltinIntegerDiv));
    registerBultin("integer_mod:", std::shared_ptr<Builtin> (new BuiltinIntegerMod));
    registerBultin("integer_toString", std::shared_ptr<Builtin> (new BuiltinIntegerToString));
    registerBultin("integer_lessThan:", std::shared_ptr<Builtin> (new BuiltinIntegerLessThan));
    registerBultin("integer_greaterThan:", std::shared_ptr<Builtin> (new BuiltinIntegerGreaterThan));
    registerBultin("integer_equals:", std::shared_ptr<Builtin> (new BuiltinIntegerEquals));
    registerBultin("integer_lessThanOrEqual:", std::shared_ptr<Builtin> (new BuiltinIntegerLessThanOrEqual));
    registerBultin("integer_greaterThanOrEqual", std::shared_ptr<Builtin> (new BuiltinIntegerGreaterThanOrEqual));
    registerBultin("integer_fromString:", std::shared_ptr<Builtin> (new BuiltinIntegerFromString));
    registerBultin("integer_charValue", std::shared_ptr<Builtin> (new BuiltinIntegerCharValue));

    registerBultin("string_charAt:", std::shared_ptr<Builtin> (new BuiltinStringCharAt));
    registerBultin("string_charAsIntAt:", std::shared_ptr<Builtin> (new BuiltinStringCharAsIntAt));
    registerBultin("string_setChar:at:", std::shared_ptr<Builtin> (new BuiltinStringSetCharAt));
    registerBultin("string_toLower", std::shared_ptr<Builtin> (new BuiltinStringToLower));
    registerBultin("string_toUpper", std::shared_ptr<Builtin> (new BuiltinStringToUpper));
    registerBultin("string_trim", std::shared_ptr<Builtin> (new BuiltinStringTrim));
    registerBultin("string_append:", std::shared_ptr<Builtin> (new BuiltinStringAppend));
    registerBultin("string_length", std::shared_ptr<Builtin> (new BuiltinStringLength));
    registerBultin("string_toString", std::shared_ptr<Builtin> (new BuiltinStringToString));
    registerBultin("string_startsWith:", std::shared_ptr<Builtin> (new BuiltinStringStartsWith));
    registerBultin("string_contains:", std::shared_ptr<Builtin> (new BuiltinStringContains));
    registerBultin("string_splitBy:", std::shared_ptr<Builtin> (new BuiltinStringSplitBy));
    registerBultin("string_equals:", std::shared_ptr<Builtin> (new BuiltinStringEquals));

    registerBultin("array_size", std::shared_ptr<Builtin> (new BuiltinObjectArraySize));
    registerBultin("array_newWithSize:", std::shared_ptr<Builtin> (new BuiltinObjectArrayNewWithSize));
    registerBultin("array_at:", std::shared_ptr<Builtin> (new BuiltinObjectArrayAt));
    registerBultin("array_at:set:", std::shared_ptr<Builtin> (new BuiltinObjectArrayAtSet));
    registerBultin("array_empty", std::shared_ptr<Builtin> (new BuiltinObjectArrayEmpty));
    registerBultin("array_toString", std::shared_ptr<Builtin> (new BuiltinObjectArrayToString));

    registerBultin("fileio_openPath:mode:file:", std::shared_ptr<Builtin> (new BuiltinFileIOOpenModeFile));
    registerBultin("fileio_close:", std::shared_ptr<Builtin> (new BuiltinFileIOClose));
    registerBultin("fileio_read:", std::shared_ptr<Builtin> (new BuiltinFileIORead));
    registerBultin("fileio_write:file:", std::shared_ptr<Builtin> (new BuiltinFileIOWriteFile));
    registerBultin("fileio_writeString:file:", std::shared_ptr<Builtin> (new BuiltinFileIOWriteStringFile));
    registerBultin("fileio_isOK:", std::shared_ptr<Builtin> (new BuiltinFileIOIsOK));
    registerBultin("fileio_readLine:", std::shared_ptr<Builtin> (new BuiltinFileIOReadLine));

    registerBultin("socketio_createTCPServerSocketAddress:port:file:", std::shared_ptr<Builtin> (new BuiltinSocketCreateTCPServerSocketAddressPortFile));
    registerBultin("socketio_closeTCPServerSocketFile:", std::shared_ptr<Builtin> (new BuiltinSocketCloseTCPServerSocketFile));
    registerBultin("socketio_TCPServerSocket:acceptClientFile:", std::shared_ptr<Builtin> (new BuiltinSocketTCPServerSocketAcceptClientFile));
    registerBultin("socketio_closeTCPClientSocketFile:", std::shared_ptr<Builtin> (new BuiltinSocketCloseTCPClientSocketFile));

    registerBultin("system_runFile:", std::shared_ptr<Builtin> (new BuiltinSystemRunFile));
    registerBultin("system_parse:", std::shared_ptr<Builtin> (new BuiltinSystemParse));
    registerBultin("system_bindIn:", std::shared_ptr<Builtin> (new BuiltinSystemBindIn));
    registerBultin("system_bindOut:", std::shared_ptr<Builtin> (new BuiltinSystemBindOut));
    registerBultin("system_bindErr:", std::shared_ptr<Builtin> (new BuiltinSystemBindErr));
    registerBultin("system_generateBytecode:", std::shared_ptr<Builtin> (new BuiltinSystemGenerateBytecode));
    registerBultin("system_startThread:", std::shared_ptr<Builtin> (new BuiltinSystemStartThread));

    std::cout << "[BuiltinPool]: initialized." << std::endl;
}

void BuiltinPool::registerBultin(const std::string &name, std::shared_ptr<Builtin> builtin)
{
    builtins.insert(std::pair<std::string, std::shared_ptr<Builtin> >(name, builtin));
}

std::shared_ptr<Builtin> BuiltinPool::lookupBultin(const std::string &name)
{
    auto it = builtins.find(name);
    if(it != builtins.end())
    {
        return it->second;
    }
    return std::shared_ptr<Builtin> (NULL);
}
