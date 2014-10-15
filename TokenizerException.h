#ifndef TOKENIZEREXCEPTION_H
#define TOKENIZEREXCEPTION_H

class TokenizerException;

#include <string>
#include <ostream>

class TokenizerException
{
private:
    std::string message;

public:
    TokenizerException();
    TokenizerException(const std::string &str);

    const std::string &what();
};

std::ostream & operator << (std::ostream &os, const TokenizerException &exception);

#endif
