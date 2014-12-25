#include "Common.h"
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
    std::cout << "%% Interpreter::initializing." << std::endl;

    Runtime::globalEnvironment = &TTObject::createObject(TT_ENV);
    Runtime::globalEnvironment->addField(TO_TT_STR("nil"), TTObject::createObject(TT_NIL));
    Runtime::globalEnvironment->addField(TO_TT_STR("parentEnv"), Runtime::globalEnvironment->getField(TO_TT_STR("nil")));
    Runtime::globalEnvironment->addField(TO_TT_STR("Global"), Runtime::globalEnvironment);

    setupObject();
    setupLiterals();
    setupFile();
    setupSocket();
    setupSystem();

    std::cout << "%% Loading TTLib." << std::endl;
    loadTTLib();
    std::cout << "%% Loading TTLib done." << std::endl;

   // std::cout << "### test global env: " << Runtime::globalEnvironment << std::endl;
   // std::cout << "### test field parent: " << Runtime::globalEnvironment->getField(TO_TT_STR("parent")) << std::endl;
    std::cout << "%% Interpreter::initialized." << std::endl;
}

void Interpreter::setupObject()
{
#ifdef DEBUG
    std::cout << "//setupObject" << std::endl;
#endif
    RefPtr<TTObject> object = TTObject::createObject(TT_OBJECT);

    BuiltinUtil::addSimpleMethod(object, "alloc", "object_alloc");
    BuiltinUtil::addMultipleMethod(object, "object:addField:", {"object", "addField"},"object_add");
    BuiltinUtil::addMultipleMethod(object, "object:get:", {"object", "get"},"object_get");
    BuiltinUtil::addSimpleMethod(object, "getFieldNames", "object_getFieldNames");
    BuiltinUtil::addSimpleMethod(object, "getFieldValues", "object_getFieldValues");
    BuiltinUtil::addMultipleMethod(object, "object:set:value:", {"object", "set", "value"},"object_set");
    BuiltinUtil::addMultipleMethod(object, "clone:", {"clone"},"object_clone");
    BuiltinUtil::addSimpleMethod(object, "new", "object_new");
    BuiltinUtil::addSimpleMethod(object, "toString", "object_tostring");

    TTObject::laterFields.push_back(std::make_pair("parent", RefPtr<TTObject>(object)));

    RefPtr<TTObject> debug = TTObject::createObject(TT_OBJECT);
    BuiltinUtil::addMultipleMethod(debug, "print:", {"print"}, "object_debugprint");
    BuiltinUtil::addMultipleMethod(debug, "printrec:", {"printrec"}, "object_debugprintrec");
    BuiltinUtil::addMultipleMethod(debug, "printString:", {"printString"}, "object_debugprintstring");
    BuiltinUtil::addSimpleMethod(debug, "gc", "object_debuggc");


    object->addField(TO_TT_STR("nil"), Runtime::globalEnvironment->getField(TO_TT_STR("nil")));

    Runtime::globalEnvironment->addField(TO_TT_STR("Object"), object);

    Runtime::globalEnvironment->addField(TO_TT_STR("Debug"), debug);
}

void Interpreter::setupLiterals()
{
    RefPtr<TTObject> object = Runtime::globalEnvironment->getField(TO_TT_STR("Object"));

    RefPtr<TTObject> integer = TTObject::createObject(TT_LITERAL);
    RefPtr<TTLiteral> integerLit = TTLiteral::createIntegerLiteral()->getLiteral(); // to override null parent
    integer->setLiteral(integerLit);
    integer->setField(TO_TT_STR("parent"), object);
    BuiltinUtil::addMultipleMethod(integer, "add:", {"add"},"integer_add:");
    BuiltinUtil::addMultipleMethod(integer, "minus:", {"minus"},"integer_minus:");
    BuiltinUtil::addMultipleMethod(integer, "mul:", {"mul"},"integer_mul:");
    BuiltinUtil::addMultipleMethod(integer, "div:", {"div"},"integer_div:");
    BuiltinUtil::addMultipleMethod(integer, "mod:", {"mod"},"integer_mod:");
    BuiltinUtil::addSimpleMethod(integer, "toString", "integer_toString");
    BuiltinUtil::addSimpleMethod(integer, "charValue", "integer_charValue");
    BuiltinUtil::addMultipleMethod(integer, "lessThan:", {"lessThan"},"integer_lessThan:");
    BuiltinUtil::addMultipleMethod(integer, "greaterThan:", {"greaterThan"},"integer_greaterThan:");
    BuiltinUtil::addMultipleMethod(integer, "equals:", {"equals"},"integer_equals:");
    BuiltinUtil::addMultipleMethod(integer, "lessThanOrEqual:", {"lessThanOrEqual"},"integer_lessThanOrEqual:");
    BuiltinUtil::addMultipleMethod(integer, "greaterThanOrEqual:", {"greaterThanOrEqual"},"integer_greaterThanOrEqual:");
    BuiltinUtil::addMultipleMethod(integer, "fromString:", {"fromString"},"integer_fromString:");


    RefPtr<TTObject> string = TTObject::createObject(TT_LITERAL);
    RefPtr<TTLiteral> stringLit = TTLiteral::createStringLiteral(TO_TT_STR(""))->getLiteral(); // to override null parent
    string->setLiteral(stringLit);
    string->setField(TO_TT_STR("parent"), object);
    BuiltinUtil::addMultipleMethod(string, "charAt:", {"charAt"},"string_charAt:");
    BuiltinUtil::addMultipleMethod(string, "charAsIntAt:", {"charAsIntAt"},"string_charAsIntAt:");
    BuiltinUtil::addMultipleMethod(string, "setChar:at:", {"setChar", "at"},"string_setChar:at:");
    BuiltinUtil::addSimpleMethod(string, "toLower", "string_toLower");
    BuiltinUtil::addSimpleMethod(string, "toUpper", "string_toUpper");
    BuiltinUtil::addSimpleMethod(string, "trim", "string_trim");
    BuiltinUtil::addMultipleMethod(string, "append:", {"append"},"string_append:");
    BuiltinUtil::addSimpleMethod(string, "length", "string_length");
    BuiltinUtil::addSimpleMethod(string, "toString", "string_toString");
    BuiltinUtil::addMultipleMethod(string, "startsWith:", {"startsWith"},"string_startsWith:");
    BuiltinUtil::addMultipleMethod(string, "equals:", {"equals"},"string_equals:");

    RefPtr<TTObject> array = TTObject::createObject(TT_LITERAL);
    RefPtr<TTLiteral> arrayLit = TTLiteral::createObjectArray(0)->getLiteral(); // to override null parent
    array->setLiteral(arrayLit);
    array->setField(TO_TT_STR("parent"), object);
    BuiltinUtil::addSimpleMethod(array, "size", "array_size");
    BuiltinUtil::addSimpleMethod(array, "length", "array_size");
    BuiltinUtil::addMultipleMethod(array, "at:", {"at"}, "array_at:");
    BuiltinUtil::addMultipleMethod(array, "at:set:", {"at", "set"}, "array_at:set:");
    BuiltinUtil::addSimpleMethod(array, "empty", "array_empty");
    BuiltinUtil::addSimpleMethod(array, "toString", "array_toString");

    Runtime::globalEnvironment->addField(TO_TT_STR("Integer"), integer);
    Runtime::globalEnvironment->addField(TO_TT_STR("String"), string);
    Runtime::globalEnvironment->addField(TO_TT_STR("Array"), array);
}

void Interpreter::setupFile()
{
    RefPtr<TTObject> object = Runtime::globalEnvironment->getField(TO_TT_STR("Object"));
    RefPtr<TTObject> file = TTObject::createObject(TT_OBJECT);
    file->setField(TO_TT_STR("parent"), object);

    BuiltinUtil::addMultipleMethod(file, "openPath:mode:file:", {"openPath", "mode", "file"}, "fileio_openPath:mode:file:");
    BuiltinUtil::addMultipleMethod(file, "close:", {"close"}, "fileio_close:");
    BuiltinUtil::addMultipleMethod(file, "read:", {"read"}, "fileio_read:");
    BuiltinUtil::addMultipleMethod(file, "write:file:", {"write", "file"}, "fileio_write:file:");
    BuiltinUtil::addMultipleMethod(file, "writeString:file:", {"writeString", "file"}, "fileio_writeString:file:");
    BuiltinUtil::addMultipleMethod(file, "isOK:", {"isOK"}, "fileio_isOK:");
    BuiltinUtil::addMultipleMethod(file, "readLine:", {"readLine"}, "fileio_readLine:");

    Runtime::globalEnvironment->addField(TO_TT_STR("FileIO"), file);
}

void Interpreter::setupSocket()
{
    RefPtr<TTObject> object = Runtime::globalEnvironment->getField(TO_TT_STR("Object"));
    RefPtr<TTObject> socket = TTObject::createObject(TT_OBJECT);
    socket->setField(TO_TT_STR("parent"), object);

    BuiltinUtil::addMultipleMethod(socket, "createTCPServerSocketAddress:port:file:", {"createTCPServerSocketAddress", "port", "file"}, "socketio_createTCPServerSocketAddress:port:file:");
    BuiltinUtil::addMultipleMethod(socket, "closeTCPServerSocketFile:", {"closeTCPServerSocketFile"}, "socketio_closeTCPServerSocketFile:");
    BuiltinUtil::addMultipleMethod(socket, "TCPServerSocket:acceptClientFile:", {"TCPServerSocket", "acceptClientFile"}, "socketio_TCPServerSocket:acceptClientFile:");
    BuiltinUtil::addMultipleMethod(socket, "closeTCPClientSocketFile:", {"closeTCPClientSocketFile"}, "socketio_closeTCPClientSocketFile:");

    Runtime::globalEnvironment->addField(TO_TT_STR("SocketIO"), socket);
}

void Interpreter::setupSystem()
{
    RefPtr<TTObject> object = Runtime::globalEnvironment->getField(TO_TT_STR("Object"));
    RefPtr<TTObject> system = TTObject::createObject(TT_OBJECT);
    system->setField(TO_TT_STR("parent"), object);

    BuiltinUtil::addMultipleMethod(system, "runFile:", {"runFile"}, "system_runFile:");
    BuiltinUtil::addMultipleMethod(system, "parse:", {"parse"}, "system_parse:");
    BuiltinUtil::addMultipleMethod(system, "bindIn:", {"bindIn"}, "system_bindIn:");
    BuiltinUtil::addMultipleMethod(system, "bindOut:", {"bindOut"}, "system_bindOut:");
    BuiltinUtil::addMultipleMethod(system, "bindErr:", {"bindErr"}, "system_bindErr:");
    BuiltinUtil::addMultipleMethod(system, "generateBytecode:", {"generateBytecode"}, "system_generateBytecode:");

    Runtime::globalEnvironment->addField(TO_TT_STR("System"), system);
}

void Interpreter::loadTTLibModule(std::string name)
{
    std::cout << "TTLib: Loading '" << name << "'." << std::endl;
    std::string path = "ttlib/";
    path.append(name);
    std::ifstream file(path, std::ifstream::in);

    if(file.fail())
    {
        std::cerr << "TTLib: Cannot open ttlib file: '" << name << "'!!" << std::endl;
        throw std::exception();
    }


    interpretFile(file, true);
}

void Interpreter::loadTTLib()
{
    // basic libs
    loadTTLibModule("init.tt");
    loadTTLibModule("control.tt");
    loadTTLibModule("cont.tt");
    loadTTLibModule("exec.tt");
    loadTTLibModule("file.tt");
    loadTTLibModule("socket.tt");
    loadTTLibModule("io.tt");

    // math
    loadTTLibModule("number.tt");
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
            RefPtr<TTObject> expression = parser.parse(false, false);

#ifdef DEBUG
            std::cout << std::endl << "<<< ======================================" << std::endl;
            std::cout << "Expression result: " << std::endl;
            expression->print(std::cout, 1, false);
            std::cout << std::endl;
#endif

            if(&expression != NULL)
            {
                //RefPtr<TTObject> result = evaluator.evaluate(expression, Runtime::globalEnvironment);

                RefPtr<TTObject> expr = Expression::createNaiveBlock(expression);
                RefPtr<TTObject> result = bytecodeInterpreter.interpret(expr, Runtime::globalEnvironment, NULL);

                if(!silent)
                {
#ifdef DEBUG
                    std::cout << std::endl << ">>> ======================================" << std::endl;
                    std::cout << "Evaluator result: " << std::endl;
#endif

                    if(&result)
                    {
                        std::string toStringName = "toString";
                        result = evaluator.sendSimpleMessage(result, toStringName);
                    }
                    if(&result)
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
