#include "common.h"
#include "Interpreter.h"
#include "TokenizerException.h"
#include "Parser.h"
#include "Evaluator.h"
#include "Expression.h"
#include <fstream>

Interpreter::Interpreter()
{
    initialize();
}

void Interpreter::initialize()
{
    globalEnvironment = TTObject::createObject(TT_ENV);
    globalEnvironment->addField(TO_TT_STR("parent"), TTObject::createObject(TT_NIL));

    globalEnvironment->addField(TO_TT_STR("nil"), TTObject::createObject(TT_NIL));

    TTObject *lit = TTObject::createObject(TT_LITERAL);
    lit->setLiteral(TTLiteral::createStringLiteral(TO_TT_STR("Hello, World!")));
    globalEnvironment->addField(TO_TT_STR("testStr"), lit);

    setupObject();

    std::cout << "### test global env: " << globalEnvironment << std::endl;
    std::cout << "### test field parent: " << globalEnvironment->getField(TO_TT_STR("parent")) << std::endl;
}

void Interpreter::setupObject()
{
    TTObject *object = TTObject::createObject(TT_OBJECT);
    TTObject *debug = TTObject::createObject(TT_OBJECT);

    addSimpleMethod(object, "alloc", "object_alloc");
    addMultipleMethod(object, "object:addField:", {"object", "addField"},"object_add");
    addMultipleMethod(object, "object:get:", {"object", "get"},"object_get");
    addMultipleMethod(object, "object:set:value:", {"object", "set", "value"},"object_set");
    addMultipleMethod(debug, "print:", {"print"}, "object_debugprint");
    addMultipleMethod(debug, "printrec:", {"printrec"}, "object_debugprintrec");

    globalEnvironment->addField(TO_TT_STR("Object"), object);
    globalEnvironment->addField(TO_TT_STR("Debug"), debug);

    std::cout << "%% Loading TTLib." << std::endl;
    loadTTLib();
    std::cout << "%% Loading TTLib done." << std::endl;
}

void Interpreter::loadTTLib()
{
    std::ifstream init("../TinyTalk/ttlib/init.tt", std::ifstream::in);
    std::ifstream control("../TinyTalk/ttlib/control.tt", std::ifstream::in);
    std::ifstream clazz("../TinyTalk/ttlib/class.tt", std::ifstream::in);

    if(init.fail() || control.fail() || clazz.fail())
    {
        std::cerr << "TTLib: Cannot open ttlib files!!" << std::endl;
        throw std::exception();
    }

    interpret(init);
    interpret(control);
    interpret(clazz);
}

void Interpreter::addSimpleMethod(TTObject *dest, const std::string &msgName, const std::string &buitlinName)
{
    std::vector<TTObject *> names;
    TTObject *objectLit = TTObject::createObject(TT_LITERAL);
    objectLit->setLiteral(TTLiteral::createStringLiteral(TO_TT_STR(msgName.c_str())));
    names.push_back(objectLit);
    TTLiteral *objArray = TTLiteral::createObjectArray(names);
    TTObject *allocBlock = Expression::createBlock(objArray, TTLiteral::createStringLiteral(TO_TT_STR(msgName.c_str())), NULL, TTLiteral::createStringLiteral(TO_TT_STR(buitlinName.c_str())));

    dest->addField(TO_TT_STR(msgName.c_str()), allocBlock);
}

void Interpreter::addMultipleMethod(TTObject *dest, const std::string &msgName, const std::vector<std::string> &msgArgs, const std::string &buitlinName)
{
    std::vector<TTObject *> names;
    for(auto arg : msgArgs)
    {
        TTObject *objectLit = TTObject::createObject(TT_LITERAL);
        objectLit->setLiteral(TTLiteral::createStringLiteral(TO_TT_STR(arg.c_str())));
        names.push_back(objectLit);
    }
    TTLiteral *objArray = TTLiteral::createObjectArray(names);
    TTObject *allocBlock = Expression::createBlock(objArray, TTLiteral::createStringLiteral(TO_TT_STR(msgName.c_str())), NULL, TTLiteral::createStringLiteral(TO_TT_STR(buitlinName.c_str())));

    dest->addField(TO_TT_STR(msgName.c_str()), allocBlock);
}

Interpreter::~Interpreter()
{

}

void Interpreter::interpret(std::istream &is)
{
    std::shared_ptr<Reader> reader(new Reader(&is));
    std::shared_ptr<Tokenizer> tokenizer(new Tokenizer(reader));
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