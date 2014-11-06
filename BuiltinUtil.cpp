#include "BuiltinUtil.h"
#include "Expression.h"
#include "common.h"

TTObject *BuiltinUtil::createSimpleMethod(const std::string &msgName, const std::string &buitlinName)
{
    std::vector<TTObject *> names;
    TTObject *objectLit = TTObject::createObject(TT_LITERAL);
    objectLit->setLiteral(TTLiteral::createStringLiteral(TO_TT_STR(msgName.c_str())));
    names.push_back(objectLit);
    TTLiteral *objArray = TTLiteral::createObjectArray(names);
    return  Expression::createBlock(objArray, TTLiteral::createStringLiteral(TO_TT_STR(msgName.c_str())), NULL, TTLiteral::createStringLiteral(TO_TT_STR(buitlinName.c_str())));
}

TTObject *BuiltinUtil::createMultipleMethod(const std::string &msgName, const std::vector <std::string> &msgArgs, const std::string &buitlinName)
{
    std::vector<TTObject *> names;
    for(auto arg : msgArgs)
    {
        TTObject *objectLit = TTObject::createObject(TT_LITERAL);
        objectLit->setLiteral(TTLiteral::createStringLiteral(TO_TT_STR(arg.c_str())));
        names.push_back(objectLit);
    }
    TTLiteral *objArray = TTLiteral::createObjectArray(names);
    return Expression::createBlock(objArray, TTLiteral::createStringLiteral(TO_TT_STR(msgName.c_str())), NULL, TTLiteral::createStringLiteral(TO_TT_STR(buitlinName.c_str())));
}

void BuiltinUtil::addSimpleMethod(TTObject *dest, const std::string &msgName, const std::string &buitlinName)
{
    TTObject *method = createSimpleMethod(msgName, buitlinName);

    dest->addField(TO_TT_STR(msgName.c_str()), method);
}

void BuiltinUtil::addMultipleMethod(TTObject *dest, const std::string &msgName, const std::vector<std::string> &msgArgs, const std::string &buitlinName)
{
    TTObject *method = createMultipleMethod(msgName, msgArgs, buitlinName);

    dest->addField(TO_TT_STR(msgName.c_str()), method);
}
