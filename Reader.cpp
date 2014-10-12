#include "Reader.h"
#include <cstdio>

Reader::Reader(std::shared_ptr<std::istream> is)
    : is(is)
{

}

int Reader::read()
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

int Reader::peek()
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

void Reader::putBack(int c)
{
    buffer.push(c);
}
