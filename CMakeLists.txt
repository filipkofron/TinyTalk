cmake_minimum_required(VERSION 2.8.4)
project(TinyTalk)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall")

set(SOURCE_FILES Expression.cpp Interpreter.cpp main.cpp MemAllocator.cpp Message.cpp Parser.cpp TTLiteral.cpp TTObject.cpp Reader.cpp Token.cpp Tokenizer.cpp TokenizerException.cpp)
add_executable(TinyTalk ${SOURCE_FILES})
