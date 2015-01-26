cmake_minimum_required(VERSION 2.8.4)
project(TinyTalk C CXX)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -pedantic -fno-strict-aliasing -g -rdynamic -Wall -march=native -O2 -fbuiltin -pthread")

set(SOURCE_FILES    Builtin.h
                    BuiltinFile.h
                    BuiltinFile.cpp
                    BuiltinInteger.cpp
                    BuiltinObject.cpp
                    BuiltinObjectArray.cpp
                    BuiltinPool.cpp
                    BuiltinSocket.h
                    BuiltinSocket.cpp
                    BuiltinString.h
                    BuiltinString.cpp
                    BuiltinSystem.h
                    BuiltinSystem.cpp
                    BuiltinUtil.cpp
                    BytecodeGen.h
                    BytecodeGen.cpp
                    BytecodeInterpreter.h
                    BytecodeInterpreter.cpp
                    Common.h
                    Common.cpp
                    Evaluator.cpp
                    Expression.cpp
                    GCBarrier.h
                    GCBarrier.cpp
                    Interpreter.cpp
                    main.cpp
                    MemAllocator.cpp
                    Parser.cpp
                    Ptr.cpp
                    Reader.cpp
                    RefPtr.cpp
                    RefPtrMap.cpp
                    Runtime.cpp
                    Lock.h
                    Lock.cpp
                    Stack.h
                    Stack.cpp
                    Token.h
                    Token.cpp
                    Tokenizer.h
                    Tokenizer.cpp
                    TokenizerException.h
                    TokenizerException.cpp
                    TTLiteral.h
                    TTLiteral.cpp
                    TTObject.h
                    TTObject.cpp)

add_executable(TinyTalk ${SOURCE_FILES})
