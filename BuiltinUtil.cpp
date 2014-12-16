#include "TTLiteral.h"
#include "BuiltinUtil.h"
#include "Expression.h"
#include "Common.h"

RefPtr<TTObject> BuiltinUtil::createSimpleMethod(const std::string &msgName, const std::string &buitlinName)
{
    std::vector<RefPtr<TTObject> > names;
    RefPtr<TTObject> objectLit = TTLiteral::createStringLiteral(TO_TT_STR(msgName.c_str()));
    names.push_back(objectLit);
    RefPtr<TTLiteral> objArray = TTLiteral::createObjectArray(names)->getLiteral();
    RefPtr<TTLiteral> msgNameLit = TTLiteral::createStringLiteral(TO_TT_STR(msgName.c_str()))->getLiteral();
    RefPtr<TTLiteral> builtinNameLit = TTLiteral::createStringLiteral(TO_TT_STR(buitlinName.c_str()))->getLiteral();
#ifdef DEBUG
    std::cout << "createSimpleMethod" << std::endl;
#endif
    RefPtr<TTObject> res = Expression::createBlock(objArray, msgNameLit, NULL, builtinNameLit);
    return res;
}

RefPtr<TTObject> BuiltinUtil::createMultipleMethod(const std::string &msgName, const std::vector <std::string> &msgArgs, const std::string &buitlinName)
{
    std::vector<RefPtr<TTObject> > names;
    for(auto arg : msgArgs)
    {
        RefPtr<TTObject> objectLit = TTLiteral::createStringLiteral(TO_TT_STR(arg.c_str()));
        names.push_back(objectLit);
    }
    RefPtr<TTLiteral> objArray = TTLiteral::createObjectArray(names)->getLiteral();
    RefPtr<TTLiteral> msgNameLit = TTLiteral::createStringLiteral(TO_TT_STR(msgName.c_str()))->getLiteral();
    RefPtr<TTLiteral> builtinNameLit = TTLiteral::createStringLiteral(TO_TT_STR(buitlinName.c_str()))->getLiteral();
    RefPtr<TTObject> res = Expression::createBlock(objArray, msgNameLit, NULL, builtinNameLit);
    return res;
}

void BuiltinUtil::addSimpleMethod(RefPtr<TTObject> dest, const std::string &msgName, const std::string &buitlinName)
{
    RefPtr<TTObject> method = createSimpleMethod(msgName, buitlinName);

    dest->addField(TO_TT_STR(msgName.c_str()), method);
}

void BuiltinUtil::addMultipleMethod(RefPtr<TTObject> dest, const std::string &msgName, const std::vector<std::string> &msgArgs, const std::string &buitlinName)
{
    RefPtr<TTObject> method = createMultipleMethod(msgName, msgArgs, buitlinName);

    dest->addField(TO_TT_STR(msgName.c_str()), method);
}
