#include "BuiltinObject.h"
#include "common.h"

TTObject *BuiltinObjectAlloc::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    std::cout << "[Builtin]: Allocating a new object." << std::endl;

    return TTObject::createObject(TT_OBJECT);
}

TTObject *BuiltinObjectAddField::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 2 && values.size() != 2)
    {
        std::cerr << "[Builtin]: Adding field accepts two arguments only." << std::endl;
        throw std::exception();
    }
    std::string name = (char *) values[1]->getLiteral()->data;
    std::cout << "[Builtin]: Adding field '" << name << "'." << std::endl;

    TTObject *object = values[0];
    std::cout << "[Builtin]: Before add: " << object << std::endl;

    object->addField(TO_TT_STR(name.c_str()), TTObject::createObject(TT_NIL));

    std::cout << "[Builtin]: After add: " << object << std::endl;

    return dest;
}

TTObject *BuiltinObjectGetter::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 2 && values.size() != 2)
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
    std::cout << "[Builtin]: Getting field '" << name << "'." << std::endl;

    TTObject *object = values[0];

    std::cout << "[Builtin]: Checking field in dest = " << object << std::endl;

    if (!object->hasField(TO_TT_STR(name.c_str())))
    {
        std::cerr << "[Builtin]: Field not found!" << std::endl;
        throw std::exception();
    }

    std::cout << "[Builtin]: Getting field from dest = " << object << std::endl;

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
    std::cout << "[Builtin]: Setting field '" << name << "'." << std::endl;

    TTObject *object = values[0];
    std::cout << "[Builtin]: Checking field in dest = " << object << std::endl;

    if (!object->hasField(TO_TT_STR(name.c_str())))
    {
        std::cerr << "[Builtin]: Field not found!" << std::endl;
        throw std::exception();
    }

    std::cout << "[Builtin]: Setting field in dest = " << object << std::endl;

    object->setField(TO_TT_STR(name.c_str()), values[2]);

    return dest;
}

TTObject *BuiltinObjectDebugPrint::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 && values.size() != 1)
    {
        std::cerr << "[Builtin]: Print object accepts one argument only." << std::endl;
        throw std::exception();
    }

    std::cout << "DBG PRINT ***************************** BEGIN ********************************" << std::endl;
    values[0]->print(std::cout, 1, false);
    std::cout << "DBG PRINT ****************************** END *********************************" << std::endl;

    return dest;
}

TTObject *BuiltinObjectDebugPrintRec::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if (argNames.size() != 1 && values.size() != 1)
    {
        std::cerr << "[Builtin]: Print object accepts one argument only." << std::endl;
        throw std::exception();
    }

    std::cout << "DBG PRINT REC ***************************** BEGIN ********************************" << std::endl;
    values[0]->print(std::cout, 1, true);
    std::cout << "DBG PRINT REC ****************************** END *********************************" << std::endl;

    return dest;
}
