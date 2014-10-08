#include <iostream>
#include <sstream>
#include <memory>

#include "STReader.h"
#include "Tokenizer.h"
#include "TokenizerException.h"

using namespace std;

int main()
{
    std::cout << ">> Tinytalk Interpreter" << std::endl;
    std::cout << ">> Written by Filip Kofron" << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    std::shared_ptr<std::stringstream> ss(new std::stringstream);
    *ss << "Class print";

    STReader reader(ss);

    std::cout << "Reading test input: " << std::endl;
    std::cout << "'";

    while(reader.peek() != EOF)
    {
        std::cout << (char) reader.read();
    }

    std::cout << "'" << std::endl;

    *ss << "Class print";
    Tokenizer tokenizer(std::shared_ptr<STReader>(new STReader(ss)));

    try
    {
        std::cout << "test token: " << tokenizer.readNextToken().getType() << std::endl;
    }
    catch(TokenizerException &e)
    {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }

    std::cout << ">> TESTS Done." << std::endl;
    return 0;
}
