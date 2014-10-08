#include "TokenizerException.h"

TokenizerException::TokenizerException()
    : message("No message.")
{
}

TokenizerException::TokenizerException(const std::string &str)
    : message(str)
{
}


const std::string &TokenizerException::what()
{
    return message;
}

std::ostream & operator << (std::ostream &os, const TokenizerException &exception)
{
    os << "TokenizerException: " << exception;
    return os;
}
