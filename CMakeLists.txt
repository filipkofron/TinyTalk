cmake_minimum_required(VERSION 2.8.4)
project(TinyTalk)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp MemAllocator.cpp TTObject.cpp Reader.cpp Token.cpp Tokenizer.cpp TokenizerException.cpp)
add_executable(TinyTalk ${SOURCE_FILES})
