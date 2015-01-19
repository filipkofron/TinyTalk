#include <iostream>
#include <sstream>
#include <memory>

#include "Common.h"
#include "Reader.h"
#include "TokenizerException.h"
#include "MemAllocator.h"
#include "Interpreter.h"
#include "Runtime.h"

using namespace std;

int main()
{
    std::cout << ">> Tinytalk Interpreter" << std::endl;
    std::cout << ">> Written by Filip Kofron" << std::endl;
    MemAllocator::initializeDefaultAllocator(Runtime::allocSize);


    std::cout << std::endl;
    std::cout << std::endl;

    // Register the main thread, this is the only safe place to do it

    Interpreter interpreter;
    interpreter.interpretCommandLine(std::cin);

    // Unregister the main thread
    std::cout << std::endl;
    std::cout << ">> Bye." << std::endl;

    MemAllocator::cleanupDefaultAllocator();
    return 0;
}
