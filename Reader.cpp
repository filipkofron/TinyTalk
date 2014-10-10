#include "STReader.h"

STReader::STReader(std::shared_ptr<std::istream> is)
    : is(is)
{

}

int STReader::read()
{
    int ret;

    if(!buffer.empty())
    {
        ret = buffer.top();
        buffer.pop();
        return ret;
    }

    ret = is->get();

    if(is->fail())
    {
        ret = EOF;
    }

    return ret;
}

int STReader::peek()
{
    int ret;
    if(!buffer.empty())
    {
        ret = buffer.top();
        return ret;
    }

    ret = (int) is->get();

    if(is->fail())
    {
        return EOF;
    }

    buffer.push(ret);
    return ret;
}

void STReader::putBack(int c)
{
    buffer.push(c);
}
