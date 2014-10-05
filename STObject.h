#ifndef STOBJECT_HPP
#define STOBJECT_HPP

class STObject;

#include <map>
#include <string>
#include <memory>

class STObject
{
private:
    std::shared_ptr<STObject> clazz;
    std::map<std::string, std::shared_ptr<STObject> > data;

public:
    STObject();
    ~STObject();
    STObject(STObject &object);

    std::shared_ptr<STObject> const &getClazz() const;

    void setClazz(std::shared_ptr<STObject> const &clazz);
};

#endif
