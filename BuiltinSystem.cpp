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
