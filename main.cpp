#include <iostream>
#include <sstream>
#include <memory>
#include <cstdio>

#include "common.h"
#include "Reader.h"
#include "Tokenizer.h"
#include "TokenizerException.h"
#include "MemAllocator.h"

using namespace std;

void testReader()
{
    //std::shared_ptr<std::stringstream> ss(new std::stringstream);
    //*ss << "Class print";
    //

    std::cout << "> ";
    std::flush(std::cout);

    Tokenizer tokenizer(std::shared_ptr<Reader>(new Reader(&std::cin)));

    Token token;
    do
    {
        try
        {
            token = tokenizer.readNextToken();
            std::cout << "Got token: " << token.getTypeInfo()
                    << ": " << token.getValue() << std::endl;
        }
        catch (TokenizerException &e)
        {
            std::cerr << "Caught exception: " << e.what() << std::endl;
            break;
        }
    } while(token.getType() != Token::TEOF);
}

void testObjects()
{
    MemAllocator memAllocator(1024);
    std::cout << "testObjects(): creating object." << std::endl;
    TTObject *object = TTObject::createObject(OBJECT_MASK);
    std::cout << "testObjects(): creating object copy." << std::endl;
    TTObject *objectCopy = object->copy(&memAllocator);
    std::cout << "testObjects(): done." << std::endl;
}

int main()
{
    std::cout << ">> Tinytalk Interpreter" << std::endl;
    std::cout << ">> Written by Filip Kofron" << std::endl;
    MemAllocator::initializeDefaultAllocator(MEMORY_ALLCOATOR_SIZE_DEFAULT);

    std::cout << std::endl;
    std::cout << std::endl;

    testReader();

    //testObjects();

    std::cout << ">> TESTS Done." << std::endl;
    MemAllocator::cleanupDefaultAllocator();
    return 0;
}
