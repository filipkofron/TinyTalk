#include <fstream>
#include <sstream>
#include "BuiltinSystem.h"
#include "BytecodeInterpreter.h"
#include "Reader.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Expression.h"
#include "Runtime.h"

RefPtr<TTObject> BuiltinSystemRunFile::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_LITERAL(0);
    BUILTIN_CHECK_STRING(0);

    std::fstream file((const char *) values[0]->getLiteral()->data, std::ios::in);

    if(file.fail())
    {
        std::cerr << "Error: Cannot run file: '" << (const char *) values[0]->getLiteral()->data << "'" << std::endl;
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

RefPtr<TTObject> BuiltinSystemParse::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values)
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

RefPtr<TTObject> BuiltinSystemGenerateBytecode::invoke(RefPtr<TTObject> dest, std::vector<std::string> &argNames, std::vector<RefPtr<TTObject> > values)
{
    BUILTIN_CHECK_ARGS_COUNT(1, 1);

    BUILTIN_CHECK_EXPRESSION(0);

    RefPtr<TTObject> byteCode = Runtime::bytecodeGen.generate(values[0]);

    return byteCode;
}
