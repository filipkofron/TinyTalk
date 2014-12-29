#include <fstream>
#include <sstream>
#include <thread>
#include "BuiltinSystem.h"
#include "BytecodeInterpreter.h"
#include "Reader.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Expression.h"
#include "Runtime.h"

RefPtr<TTObject> BuiltinSystemRunFile::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);

    std::fstream file((const char *) values[0]->getLiteral()->data, std::ios::in);

    if(file.fail())
    {
        std::cerr << "[Builtin]: Error: Cannot run file: '" << (const char *) values[0]->getLiteral()->data << "'" << std::endl;
        throw std::exception();
    }

    std::shared_ptr<Reader> reader(new Reader(&file));
    std::shared_ptr<Tokenizer> tokenizer(new Tokenizer(reader));

    Parser parser(tokenizer);

    RefPtr<TTObject> expression = parser.parse(false, false);
    RefPtr<TTObject> expr = Expression::createNaiveBlock(expression);

    BytecodeInterpreter bytecodeInterpreter;
    RefPtr<TTObject> result = bytecodeInterpreter.interpret(expr, Runtime::globalEnvironment, NULL);

    return result;
}

RefPtr<TTObject> BuiltinSystemParse::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);

    std::stringstream ss;
    ss << (const char *) values[0]->getLiteral()->data;

    std::shared_ptr<Reader> reader(new Reader(&ss));
    std::shared_ptr<Tokenizer> tokenizer(new Tokenizer(reader));
    Parser parser(tokenizer);
    RefPtr<TTObject> expression = parser.parse(false, false);

    return expression;
}

RefPtr<TTObject> BuiltinSystemGenerateBytecode::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    RefPtr<TTObject> expr = values[0];

    if(values[0]->type != TT_EXPR)
    {
        if(&values[0] != Runtime::globalEnvironment->getField(TO_TT_STR("nil")))
        {
            std::cerr << "[Builtin]: GenerateBytecode Error: Invalid expression (must be expression or nil)" << std::endl;
            throw std::exception();
        }
        RefPtr<TTObject> name = TTLiteral::createStringLiteral(TO_TT_STR("nil"));
        expr = Expression::createSymbolValue(name->getLiteral());
    }

    RefPtr<TTObject> byteCode = Runtime::bytecodeGen.generate(expr);

    return byteCode;
}

RefPtr<TTObject> BuiltinSystemBindIn::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    RefPtr<TTObject> file = values[0];

    RefPtr<TTObject> fd = TTLiteral::createByteArray(sizeof(FILE *));
    FILE **fileHandle = (FILE **) fd->getLiteral()->data;
    *fileHandle = stdin;
    file->addField(TO_TT_STR("fd"), fd);

    return dest;
}

RefPtr<TTObject> BuiltinSystemBindOut::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    RefPtr<TTObject> file = values[0];

    RefPtr<TTObject> fd = TTLiteral::createByteArray(sizeof(FILE *));
    FILE **fileHandle = (FILE **) fd->getLiteral()->data;
    *fileHandle = stdout;
    file->addField(TO_TT_STR("fd"), fd);

    return dest;
}

RefPtr<TTObject> BuiltinSystemBindErr::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    RefPtr<TTObject> file = values[0];

    RefPtr<TTObject> fd = TTLiteral::createByteArray(sizeof(FILE *));
    FILE **fileHandle = (FILE **) fd->getLiteral()->data;
    *fileHandle = stderr;
    file->addField(TO_TT_STR("fd"), fd);

    return dest;
}

static void thread_entry(RefPtr<TTObject> block, RefPtr<TTObject> thiz)
{
    Runtime::gcBarrier.reg();
    BytecodeInterpreter bytecodeInterpreter;

    // we must create the new environment manually, since the entry for Bytecode Interpreter
    // forces the outer environment to be the actual (why..)
    RefPtr<TTObject> parentEnv = block->getField(TO_TT_STR("blockEnv"));
    RefPtr<TTObject> newEnv = TTObject::createObject(TT_ENV);
    newEnv->addField(TO_TT_STR("parentEnv"), parentEnv);

    if(&thiz)
    {
        newEnv->addField(TO_TT_STR("this"), thiz);
    }

    bytecodeInterpreter.interpret(block, newEnv, thiz);
    Runtime::gcBarrier.unreg();
}

RefPtr<TTObject> BuiltinSystemStartThread::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values, RefPtr<TTObject> env, RefPtr<TTObject> thiz)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    if(values[0]->type != TT_EXPR || values[0]->flags != EXPRESSION_BLOCK)
    {
        std::cerr << "[Builtin]: StartThread Error: Invalid expression (must be block)" << std::endl;
        throw std::exception();
    }

    std::thread myThread(thread_entry, values[0], thiz);
    myThread.detach();

    return dest;
}
