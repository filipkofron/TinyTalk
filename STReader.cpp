#include "STReader.h"

void STReader::read(std::istream &is)
{

}

void STReader::read(const std::string &str)
{

}


bool STReader::isWhitespace(const int &c)
{
    switch(c)
    {
        case '\t':
        case ' ':
        case '\n':
            return true;
    }

    return false;
}
