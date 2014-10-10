#include "STObject.h"

std::shared_ptr<STObject> const &STObject::getClazz() const
{
    return clazz;
}

void STObject::setClazz(std::shared_ptr<STObject> const &clazz)
{
    this->clazz = clazz;
}

std::shared_ptr<STObject> STObject::sendMessage(STMessage &message)
{
    throw std::exception();
}
