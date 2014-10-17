#include "common.h"
#include "Interpreter.h"
#include "TokenizerException.h"
#include "Parser.h"
#include <iostream>

Interpreter::Interpreter(std::shared_ptr<Reader> &reader)
    : tokenizer(new Tokenizer(reader))
{
    globalEnvironment = TTObject::createObject(TT_ENV);
    globalEnvironment->addField(TO_TT_STR("parent"), TTObject::createObject(TT_NIL));

    std::cout << "### test global env: " << globalEnvironment << std::endl;

    std::cout << "### test field parent: " << globalEnvironment->getField(TO_TT_STR("parent")) << std::endl;
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