#ifndef STREADER_H
#define STREADER_H

#include <istream>
#include <memory>
#include <stack>

class STReader;

class STReader
{
private:
    std::stack<int> buffer;
    std::shared_ptr<std::istream> is;
public:
    STReader(std::shared_ptr<std::istream> is);

    int read();
    int peek();
    void putBack(int c);
};

#endif
