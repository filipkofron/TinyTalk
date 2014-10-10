#ifndef STMESSAGE_H
#define STMESSAGE_H

class STMessage;

#include <string>
#include <memory>
#include <map>
#include "STObject.h"

class STMessage
{
private:
    std::map<std::string, std::shared_ptr<STObject> > params;
public:
    std::map<std::string, std::shared_ptr<STObject> > &getParams();
};

#endif
