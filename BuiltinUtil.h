#ifndef BUILTINUTIL_H
#define BUILTINUTIL_H

struct BuiltinUtil;

#include "RefPtr.h"
#include "TTObject.h"

struct BuiltinUtil
{
    static RefPtr<TTObject> createSimpleMethod(const std::string &msgName, const std::string &buitlinName);
    static RefPtr<TTObject> createMultipleMethod(const std::string &msgName, const std::vector <std::string> &msgArgs, const std::string &buitlinName);
    static void addSimpleMethod(RefPtr<TTObject> dest, const std::string &msgName, const std::string &buitlinName);
    static void addMultipleMethod(RefPtr<TTObject> dest, const std::string &msgName, const std::vector <std::string> &msgArgs, const std::string &buitlinName);
};

#endif
