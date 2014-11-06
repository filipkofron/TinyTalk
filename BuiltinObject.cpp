#include "BuiltinObject.h"
#include "common.h"
#include "Runtime.h"

TTObject *BuiltinObjectAlloc::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
#ifdef DEBUG
    std::cout << "[Builtin]: Allocating a new object." << std::endl;
#endif

    return TTObject::createObject(TT_OBJECT);
}

TTObject *BuiltinObjectAddField::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 2 || values.size() != 2)
    {
        std::cerr << "[Builtin]: Adding field accepts two arguments only." << std::endl;
        throw std::exception();
    }
    std::string name = (char *) values[1]->getLiteral()->data;
#ifdef DEBUG
    std::cout << "[Builtin]: Adding field '" << name << "'." << std::endl;
#endif

    TTObject *object = values[0];
#ifdef DEBUG
    std::cout << "[Builtin]: Before add: " << object << std::endl;
#endif

    object->addField(TO_TT_STR(name.c_str()), Runtime::globalEnvironment->getField(TO_TT_STR("nil")));

#ifdef DEBUG
    std::cout << "[Builtin]: After add: " << object << std::endl;
#endif

    return dest;
}

TTObject *BuiltinObjectGetter::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 2 || values.size() != 2)
    {
        std::cerr << "[Builtin]: Getting field accepts two arguments only." << std::endl;
        throw std::exception();
    }
    else
    {
        if (values[0]->type != TT_LITERAL)
        {
            std::cerr << "[Builtin]: Getting field accepts literal value only!" << std::endl;
            throw std::exception();
        }
    }
    std::string name = (char *) values[1]->getLiteral()->data;
#ifdef DEBUG
    std::cout << "[Builtin]: Getting field '" << name << "'." << std::endl;
#endif

    TTObject *object = values[0];
#ifdef DEBUG
    std::cout << "[Builtin]: Checking field in dest = " << object << std::endl;
#endif

    if (!object->hasField(TO_TT_STR(name.c_str())))
    {
        std::cerr << "[Builtin]: Field not found!" << std::endl;
        throw std::exception();
    }
#ifdef DEBUG
    std::cout << "[Builtin]: Getting field from dest = " << object << std::endl;
#endif

    return object->getField(TO_TT_STR(name.c_str()));
}

TTObject *BuiltinObjectSetter::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 3 || values.size() != 3)
    {
        std::cerr << "[Builtin]: Setting field accepts 3 arguments only." << std::endl;
        throw std::exception();
    }
    else
    {
        if (values[1]->type != TT_LITERAL)
        {
            std::cerr << "[Builtin]: Setting field accepts literal value only!" << std::endl;
            throw std::exception();
        }
    }
    if (argNames[2] != "value")
    {
        std::cerr << "[Builtin]: Setting field's third argument must be 'value', '"
                << argNames[2] << "' given!" << std::endl;
        throw std::exception();
    }
    std::string name = (char *) values[1]->getLiteral()->data;
#ifdef DEBUG
    std::cout << "[Builtin]: Setting field '" << name << "'." << std::endl;
#endif

    TTObject *object = values[0];
#ifdef DEBUG
    std::cout << "[Builtin]: Checking field in dest = " << object << std::endl;
#endif

    if (!object->hasField(TO_TT_STR(name.c_str())))
    {
        std::cerr << "[Builtin]: Field not found!" << std::endl;
        throw std::exception();
    }
#ifdef DEBUG
    std::cout << "[Builtin]: Setting field in dest = " << object << std::endl;
#endif

    object->setField(TO_TT_STR(name.c_str()), values[2]);

    return dest;
}

TTObject *BuiltinObjectDebugPrint::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: Print object accepts one argument only." << std::endl;
        throw std::exception();
    }

    std::cout << "DBG PRINT ***************************** BEGIN ********************************" << std::endl;
    values[0]->print(std::cout, 1, false);
    std::cout << std::endl << "DBG PRINT ****************************** END *********************************" << std::endl;

    return dest;
}

TTObject *BuiltinObjectDebugPrintRec::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: Print object accepts one argument only." << std::endl;
        throw std::exception();
    }

    std::cout << "DBG PRINT REC ***************************** BEGIN ********************************" << std::endl;
    values[0]->print(std::cout, 1, true);
    std::cout << "DBG PRINT REC ****************************** END *********************************" << std::endl;

    return dest;
}

TTObject *BuiltinObjectClone::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 1)
    {
        std::cerr << "[Builtin]: Clone builting function accepts one argument only." << std::endl;
        throw std::exception();
    }

    TTObject *cloned = values[0];
    TTObject *res = TTObject::clone(cloned);

    return  res;
}

TTObject *BuiltinObjectNew::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 0)
    {
        std::cerr << "[Builtin]: New builtin function accepts no arguments." << std::endl;
        throw std::exception();
    }

    TTObject *res = TTObject::createObject(TT_OBJECT);
    res->setField(TO_TT_STR("parent"), dest);

    return res;
}

TTObject *BuiltinObjectToString::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 || values.size() != 0)
    {
        std::cerr << "[Builtin]: ToString builtin function accepts no arguments." << std::endl;
        throw std::exception();
    }

    const char *resStr = "Object";

    if(!dest)
    {
        resStr = "NULL";
    }

    if(dest->type == TT_LITERAL)
    {
       return dest->getLiteral()->onMessage(dest, argNames[0], argNames, values);
    }

    TTObject *res = TTObject::createObject(TT_LITERAL);
    TTLiteral *string = TTLiteral::createStringLiteral(TO_TT_STR(resStr));

    res->setLiteral(string);

    return res;
}
