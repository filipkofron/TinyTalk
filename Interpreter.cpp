#include "common.h"
#include "Interpreter.h"
#include "TokenizerException.h"
#include "Parser.h"
#include "Evaluator.h"
#include "BuiltinUtil.h"
#include "Runtime.h"
#include <fstream>
#include <sstream>

Interpreter::Interpreter()
{
    initialize();
}

void Interpreter::initialize()
{
    Runtime::globalEnvironment = TTObject::createObject(TT_ENV);
    Runtime::globalEnvironment->addField(TO_TT_STR("nil"), TTObject::createObject(TT_NIL));
    Runtime::globalEnvironment->addField(TO_TT_STR("parentEnv"), Runtime::globalEnvironment->getField(TO_TT_STR("nil")));

    setupObject();

   // std::cout << "### test global env: " << Runtime::globalEnvironment << std::endl;
   // std::cout << "### test field parent: " << Runtime::globalEnvironment->getField(TO_TT_STR("parent")) << std::endl;
}

void Interpreter::setupObject()
{
    TTObject *object = TTObject::createObject(TT_OBJECT);

    BuiltinUtil::addSimpleMethod(object, "alloc", "object_alloc");
    BuiltinUtil::addMultipleMethod(object, "object:addField:", {"object", "addField"},"object_add");
    BuiltinUtil::addMultipleMethod(object, "object:get:", {"object", "get"},"object_get");
    BuiltinUtil::addMultipleMethod(object, "object:set:value:", {"object", "set", "value"},"object_set");
    BuiltinUtil::addMultipleMethod(object, "clone:", {"clone"},"object_clone");
    BuiltinUtil::addSimpleMethod(object, "new", "object_new");
    BuiltinUtil::addSimpleMethod(object, "toString", "object_tostring");

    TTObject::laterFields.push_back(std::make_pair("parent", RefPtr<TTObject>(object)));

    TTObject *debug = TTObject::createObject(TT_OBJECT);
    BuiltinUtil::addMultipleMethod(debug, "print:", {"print"}, "object_debugprint");
    BuiltinUtil::addMultipleMethod(debug, "printrec:", {"printrec"}, "object_debugprintrec");
    BuiltinUtil::addMultipleMethod(debug, "printString:", {"printString"}, "object_debugprintstring");


    object->addField(TO_TT_STR("nil"), Runtime::globalEnvironment->getField(TO_TT_STR("nil")));

    Runtime::globalEnvironment->addField(TO_TT_STR("Object"), object);
    Runtime::globalEnvironment->addField(TO_TT_STR("Debug"), debug);

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

    interpretFile(init, true);
    interpretFile(control, true);
    interpretFile(clazz, true);
}

Interpreter::~Interpreter()
{

}

void Interpreter::interpretFile(std::istream &is, bool silent)
{
    std::shared_ptr<Reader> reader(new Reader(&is));
    std::shared_ptr<Tokenizer> tokenizer(new Tokenizer(reader));
    Parser parser(tokenizer);
    Evaluator evaluator;
    BytecodeInterpreter bytecodeInterpreter;

    do
    {
        try
        {
            TTObject *expression = parser.parse(false);

#ifdef DEBUG
            std::cout << std::endl << "<<< ======================================" << std::endl;
            std::cout << "Expression result: " << std::endl;
            expression->print(std::cout, 1, false);
            std::cout << std::endl;
#endif

            if(expression != NULL)
            {
                //TTObject *result = (expression, Runtime::globalEnvironment);

                TTObject *expr = Expression::createNaiveBlock(expression);

                TTObject *result = bytecodeInterpreter.interpret(expr, Runtime::globalEnvironment, NULL);

                if(!silent)
                {
#ifdef DEBUG
                    std::cout << std::endl << ">>> ======================================" << std::endl;
                    std::cout << "Evaluator result: " << std::endl;
#endif

                    if(result)
                    {
                        std::string toStringName = "toString";
                        result = evaluator.sendSimpleMessage(result, toStringName);
                    }
                    if(result)
                    {
                        if(result->type == TT_LITERAL && result->getLiteral()->type == LITERAL_TYPE_STRING)
                        {
                            std::string resultStr = (char *) result->getLiteral()->data;
                            std::cout << resultStr << std::endl;
                        }
                        else
                        {
                            std::cerr << "Runtime error: Unknown literal result!: ";
                            result->print(std::cerr, 1, false);
                            std::cerr << std::endl;
                        }
                    }
                    else
                    {
                        std::cerr << "Runtime error: NULL result" << std::endl;
                    }
                    std::cout << std::endl;
                }
            }
        }
        catch (TokenizerException &e)
        {
            std::cerr << "Runtime error: Caught exception: " << e.what() << std::endl;
            break;
        }
        catch (std::exception &ex)
        {
            std::cerr << "Runtime error: Caught exception: " << ex.what() << std::endl;
            break;
        }
    } while (!tokenizer->hasReachedEOF());
}

void Interpreter::interpretCommandLine(std::istream &is)
{
    bool notEOF = true;

    do
    {
        std::stringstream ss;

        std::cout << "> ";
        std::flush(std::cout);

        bool firstEnd = false;

        while(true)
        {
            int c = is.get();
            if(c == '\n' || c == '\r' || c == EOF || ss.fail())
            {
                if(firstEnd)
                {
                    break;
                }
                firstEnd = true;
                if(c == EOF || ss.fail())
                {
                    notEOF = false;
                    break;
                }
            }
            else
            {
                firstEnd = false;
            }
            ss << (char) c;
        }

        ss.seekg(0, std::stringstream::end);
        size_t size = ss.tellg();
        ss.seekg(0, std::stringstream::beg);
        if(size != 0)
        {
            interpretFile(ss, false);
        }
    }
    while (notEOF && !is.fail());
}