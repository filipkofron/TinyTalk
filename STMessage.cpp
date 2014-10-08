#include "STMessage.h"

std::map<std::string, std::shared_ptr<STObject> > &STMessage::getParams()
{
    return params;
}
