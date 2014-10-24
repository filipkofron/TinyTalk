#include "BuiltinObject.h"
#include "common.h"

TTObject *BuiltinObjectAlloc::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    std::cout << "[Builtin]: Allocating a new object." << std::endl;

    return TTObject::createObject(TT_OBJECT);
}

TTObject *BuiltinObjectAddField::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if(argNames.size() != 1 && values.size() != 1)
    {
        std::cerr << "[Builtin]: Adding field accepts one argument only." << std::endl;
        throw std::exception();
    }
    std::string name = (char *) values[0]->getLiteral()->data;
    std::cout << "[Builtin]: Adding field '" << name << "'." << std::endl;

    std::cout << "[Builtin]: Before add: " << dest << std::endl;

    dest->addField(TO_TT_STR(name.c_str()), TTObject::createObject(TT_NIL));

    std::cout << "[Builtin]: After add: " << dest << std::endl;

    return dest;
}

TTObject *BuiltinObjectGetter::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if(argNames.size() != 1 && values.size() != 1)
    {
        std::cerr << "[Builtin]: Getting field accepts one argument only." << std::endl;
        throw std::exception();
    }
    else
    {
        if(values[0]->type != TT_LITERAL)
        {
            std::cerr << "[Builtin]: Getting field accepts literal value only!" << std::endl;
            throw std::exception();
        }
    }
    std::string name = (char *) values[0]->getLiteral()->data;
    std::cout << "[Builtin]: Getting field '" << name << "'." << std::endl;

    std::cout << "[Builtin]: Checking field in dest = " << dest << std::endl;

    if(!dest->hasField(TO_TT_STR(name.c_str())))
    {
        std::cerr << "[Builtin]: Field not found!" << std::endl;
        throw std::exception();
    }

    std::cout << "[Builtin]: Getting field from dest = " << dest << std::endl;

    return dest->getField(TO_TT_STR(name.c_str()));
}

TTObject *BuiltinObjectSetter::invoke(TTObject *dest, std::vector<std::string> &argNames, std::vector<TTObject *> values)
{
    if(argNames.size() != 2 || values.size() != 2)
    {
        std::cerr << "[Builtin]: Setting field accepts 2 arguments only." << std::endl;
        throw std::exception();
    }
    else
    {
        if(values[0]->type != TT_LITERAL)
        {
            std::cerr << "[Builtin]: Setting field accepts literal value only!" << std::endl;
            throw std::exception();
        }
    }
    if(argNames[1] != "value")
    {
        std::cerr << "[Builtin]: Setting field's second argument must be 'value', '"
                << argNames[1] << "' given!" << std::endl;
        throw std::exception();
    }
    std::string name = (char *) values[0]->getLiteral()->data;
    std::cout << "[Builtin]: Setting field '" << name << "'." << std::endl;

    std::cout << "[Builtin]: Checking field in dest = " << dest << std::endl;

    if(!dest->hasField(TO_TT_STR(name.c_str())))
    {
        std::cerr << "[Builtin]: Field not found!" << std::endl;
        throw std::exception();
    }

    std::cout << "[Builtin]: Setting field in dest = " << dest << std::endl;

    dest->setField(TO_TT_STR(name.c_str()), values[1]);

    return dest;
}
