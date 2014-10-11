#ifndef STREADER_H
#define STREADER_H

#include <istream>
#include <memory>
#include <stack>

class Reader;

class Reader
{
private:
    std::stack<int> buffer;
    std::shared_ptr<std::istream> is;
public:
    Reader(std::shared_ptr<std::istream> is);

    int read();
    int peek();
    void putBack(int c);
};

#endif
