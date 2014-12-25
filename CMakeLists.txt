cmake_minimum_required(VERSION 2.8.4)
project(TinyTalk)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -g -Wall -march=native -O0 -fbuiltin")

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
                    Interpreter.cpp
                    main.cpp
                    MemAllocator.cpp
                    Parser.cpp
                    Ptr.cpp
                    Reader.cpp
                    RefPtr.cpp
                    RefPtrMap.cpp
                    Runtime.cpp
                    Stack.cpp
                    Token.cpp
                    Tokenizer.cpp
                    TokenizerException.cpp
                    TTLiteral.cpp
                    TTObject.cpp)
add_executable(TinyTalk ${SOURCE_FILES})
