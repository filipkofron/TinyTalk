#include <iostream>
#include <sstream>
#include <memory>
#include <cstdio>

#include "common.h"
#include "Reader.h"
#include "Tokenizer.h"
#include "TokenizerException.h"
#include "MemAllocator.h"
#include "Interpreter.h"

using namespace std;

int main()
{
    std::cout << ">> Tinytalk Interpreter" << std::endl;
    std::cout << ">> Written by Filip Kofron" << std::endl;
    MemAllocator::initializeDefaultAllocator(MEMORY_ALLCOATOR_SIZE_DEFAULT);

    std::cout << std::endl;
    std::cout << std::endl;

    std::shared_ptr<Reader> reader(new Reader(&std::cin));
    Interpreter interpreter(reader);
    interpreter.startInterpreting();

    MemAllocator::cleanupDefaultAllocator();
    return 0;
}
