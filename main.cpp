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


    Interpreter interpreter;
    interpreter.interpretCommandLine(std::cin);

    if(Runtime::interpretersAlive.size())
    {
        std::cout << std::endl << "[Runtime]: Other threads still running..." << std::endl;
    }

    while(Runtime::interpretersAlive.size())
    {
        std::chrono::milliseconds miliseconds(300);

        std::this_thread::sleep_for(miliseconds);
    }

    std::cout << std::endl;
    std::cout << ">> Bye." << std::endl;

    MemAllocator::cleanupDefaultAllocator();
    return 0;
}
