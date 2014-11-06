#ifndef BUILTINUTIL_H
#define BUILTINUTIL_H

struct BuiltinUtil;

#include "TTObject.h"

struct BuiltinUtil
{
    static TTObject *createSimpleMethod(const std::string &msgName, const std::string &buitlinName);
    static TTObject *createMultipleMethod(const std::string &msgName, const std::vector <std::string> &msgArgs, const std::string &buitlinName);
    static void addSimpleMethod(TTObject *dest, const std::string &msgName, const std::string &buitlinName);
    static void addMultipleMethod(TTObject *dest, const std::string &msgName, const std::vector <std::string> &msgArgs, const std::string &buitlinName);
};

#endif
