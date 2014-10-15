#include "Interpreter.h"
#include "TokenizerException.h"
#include <iostream>

Interpreter::Interpreter(std::shared_ptr<Reader> &reader)
    : tokenizer(reader)
{
    globalEnvironment = TTObject::createObject(ENV_MASK);
}

Interpreter::~Interpreter()
{

}

void Interpreter::startInterpreting()
{
    do
    {
        try
        {
            std::cout << "> ";
            std::flush(std::cout);


        }
        catch (TokenizerException &e)
        {
            std::cerr << "Caught exception: " << e.what() << std::endl;
            break;
        }
    } while (!tokenizer.hasReachedEOF());
}