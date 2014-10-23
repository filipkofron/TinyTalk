#include "common.h"
#include "Interpreter.h"
#include "TokenizerException.h"
#include "Parser.h"
#include "Evaluator.h"

Interpreter::Interpreter(std::shared_ptr<Reader> &reader)
    : tokenizer(new Tokenizer(reader))
{
    globalEnvironment = TTObject::createObject(TT_ENV);
    globalEnvironment->addField(TO_TT_STR("parent"), TTObject::createObject(TT_NIL));
    TTObject *lit = TTObject::createObject(TT_LITERAL);
    lit->setLiteral(TTLiteral::createStringLiteral(TO_TT_STR("Hello, World!")));
    globalEnvironment->addField(TO_TT_STR("testStr"), lit);

    std::cout << "### test global env: " << globalEnvironment << std::endl;

    std::cout << "### test field parent: " << globalEnvironment->getField(TO_TT_STR("parent")) << std::endl;
}

Interpreter::~Interpreter()
{

}

void Interpreter::startInterpreting()
{
    Parser parser(tokenizer);
    Evaluator evaluator;

    do
    {
        try
        {
            std::cout << "> ";
            std::flush(std::cout);

            TTObject *expression = parser.parse(false);

            std::cout << std::endl << "<<< ======================================" << std::endl;
            std::cout << "Expression result: " << std::endl;
            expression->print(std::cout, 1, true);
            std::cout << std::endl;

            if(expression != NULL)
            {
                TTObject *result = evaluator.evaluate(expression, globalEnvironment);

                std::cout << std::endl << ">>> ======================================" << std::endl;
                std::cout << "Evaluator result: " << std::endl;
                result->print(std::cout, 1, false);
                std::cout << std::endl;
            }
        }
        catch (TokenizerException &e)
        {
            std::cerr << "Caught exception: " << e.what() << std::endl;
            break;
        }
    } while (!tokenizer->hasReachedEOF());
}