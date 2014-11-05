#include <iostream>
#include <sstream>
#include <memory>

#include "common.h"
#include "Reader.h"
#include "TokenizerException.h"
#include "MemAllocator.h"
#include "Interpreter.h"
#include "RefPtr.h"

using namespace std;

int main()
{
    std::cout << ">> Tinytalk Interpreter" << std::endl;
    std::cout << ">> Written by Filip Kofron" << std::endl;
    MemAllocator::initializeDefaultAllocator(MEMORY_ALLCOATOR_SIZE_DEFAULT);

    std::cout << std::endl;
    std::cout << std::endl;

    Interpreter interpreter;
    interpreter.interpretCommandLine(std::cin);

    RefPtr<TTObject> ptr(TTObject::createObject(TT_OBJECT));
    ptr->print(std::cout, 1, false);

    MemAllocator::cleanupDefaultAllocator();
    return 0;
}
