#include "Interpreter.h"
#include "TokenizerException.h"
#include "Parser.h"
#include <iostream>

Interpreter::Interpreter(std::shared_ptr<Reader> &reader)
    : tokenizer(new Tokenizer(reader))
{
    globalEnvironment = TTObject::createObject(TT_ENV);
}

Interpreter::~Interpreter()
{

}

void Interpreter::startInterpreting()
{
    Parser parser(tokenizer);

    do
    {
        try
        {
            std::cout << "> ";
            std::flush(std::cout);

            TTObject *expression = parser.parse();

            std::cout << "Expression result: " << std::endl << expression << std::endl;
        }
        catch (TokenizerException &e)
        {
            std::cerr << "Caught exception: " << e.what() << std::endl;
            break;
        }
    } while (!tokenizer->hasReachedEOF());
}