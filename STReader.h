#ifndef STREADER_H
#define STREADER_h

#include <istream>
#include <string>

class STReader
{
private:
public:
    void read(std::istream &is);
    void read(const std::string &str);

    static bool isWhitespace(const int &c);
};

#endif
