#include "TTObject.h"
#include "Expression.h"
#include "common.h"
#include "BuiltinUtil.h"
#include <cstring>

TTObject *TTObject::gccopy(MemAllocator *allocator)
{
    TTObject *newObject = allocator->allocateObject();
    memcpy(newObject, this, sizeof(*this));

    // TODO: implement by the GC algo

    return newObject;
}

TTObject *TTObject::createObject(uint8_t type, uint32_t fieldsPreallocated)
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    TTObject *newObject = alloc->allocateObject();
    newObject->fields = (Field *) alloc->allocate(sizeof(Field) * fieldsPreallocated);
    newObject->fieldCapacity = fieldsPreallocated;
    newObject->fieldCount = 0;
    newObject->type = type;

    for(auto pairVal : laterFields)
    {
        newObject->addField(TO_TT_STR(pairVal.first.c_str()), &(pairVal.second));
    }

    return newObject;
}

TTObject *TTObject::createObject(uint8_t type)
{
    return createObject(type, DEFAULT_FIELD_COUNT);
}

TTObject *TTObject::clone(TTObject *cloned)
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    TTObject *newObject = alloc->allocateObject();
    memcpy(newObject, cloned, sizeof(*cloned));
    newObject->fields = (Field *) alloc->allocate(sizeof(newObject->fields) * cloned->fieldCapacity);

    memcpy(newObject->fields, cloned->fields, sizeof(*cloned->fields) * cloned->fieldCapacity);

    return newObject;
}

std::vector<std::pair<std::string, RefPtr<TTObject> > > TTObject::laterFields;

bool TTObject::addField(const uint8_t *name, TTObject *object)
{
    if(!*name)
    {
        return false;
    }
    if(TTObject::hasField(name))
    {
        return false;
    }
    MemAllocator *alloc = MemAllocator::getCurrent();
    if(fieldCount == fieldCapacity)
    {
        fieldCapacity *= 2;
        Field *newFields = (Field *) alloc->allocate(sizeof(Field) * fieldCapacity);
        memcpy(newFields, fields, sizeof(Field) * fieldCount);
        fields = newFields;
    }
    fields[fieldCount].name = alloc->cloneString(name);
    fields[fieldCount].object = object;
    fieldCount++;

    return true;
}

bool TTObject::hasField(const uint8_t *name)
{
    if(!*name)
    {
        return false;
    }
    for(uint32_t i = 0; i < fieldCount; i++)
    {
        if(COMPARE_NAME(name, fields[i].name) == 0)
        {
            return true;
        }
    }
    return false;
}

TTObject *TTObject::getField(const uint8_t *name)
{
    if(!*name)
    {
        return NULL;
    }
    for(uint32_t i = 0; i < fieldCount; i++)
    {
        if(COMPARE_NAME(name, fields[i].name) == 0)
        {
            return fields[i].object;
        }
    }
    return NULL;
}

bool TTObject::setField(const uint8_t *name, TTObject *object)
{
    if(!*name)
    {
        return false;
    }
    for(uint32_t i = 0; i < fieldCount; i++)
    {
        if(COMPARE_NAME(name, fields[i].name) == 0)
        {
            fields[i].object = object;
            return true;
        }
    }
    return false;
}

TTLiteral *TTObject::getLiteral()
{
    if(type != TT_LITERAL)
    {
        std::cerr << "Object is not literal: Cannot get literal value!" << std::endl;
        throw  std::exception();
    }
    for(uint32_t i = 0; i < fieldCount; i++)
    {
        if(!*fields[i].name)
        {
            return (TTLiteral *) fields[i].object;
        }
    }
    return NULL;
}

bool TTObject::setLiteral(TTLiteral *lit)
{
    if(type != TT_LITERAL)
    {
        std::cerr << "Object is not literal: Cannot set literal value!" << std::endl;
        throw  std::exception();
    }
    for(uint32_t i = 0; i < fieldCount; i++)
    {
        if(!*fields[i].name)
        {
            fields[i].object = (TTObject *) lit;
            return true;
        }
    }

    MemAllocator *alloc = MemAllocator::getCurrent();
    if(fieldCount == fieldCapacity)
    {
        fieldCapacity *= 2;
        Field *newFields = (Field *) alloc->allocate(sizeof(Field) * fieldCapacity);
        memcpy(newFields, fields, sizeof(Field) * fieldCount);
        fields = newFields;
    }
    fields[fieldCount].name = alloc->cloneString(TO_TT_STR(""));
    fields[fieldCount].object = (TTObject *) lit;
    fieldCount++;

    return true;
}

void TTObject::print(std::ostream &os, uint32_t level, bool recursive)
{
    if(level < 1)
    {
        level = 1;
    }


    os << "Object" << std::endl;
    prlvl(os, level - 1);
    os << "{" << std::endl;

    if(!this)
    {
        prlvl(os, level);
        os << "NULL" << std::endl;
        prlvl(os, level - 1);
        os << "}";
        return;
    }

    prlvl(os, level);
    os << "Address: " << (unsigned long long) this << std::endl;
    const char *type = "INVALID OBJECT";
    switch(this->type)
    {
        case TT_NIL:
            type = "NIL OBJECT";
            break;
        case TT_EXPR:
            type = "EXPRESSION";
            break;
        case TT_LITERAL:
            type = "LITERAL";
            break;
        case TT_ENV:
            type = "ENV";
            break;
        case TT_OBJECT:
            type = "OBJECT";
            break;
        default:
            break;
    }
    prlvl(os, level);
    os << "Type: " << type << std::endl;
    switch(this->type)
    {
        case TT_EXPR:
            prlvl(os, level);
            os << "Expression type: " << Expression::getTypeInfo(this) << std::endl;
            break;
        case TT_LITERAL:
            if(this->getLiteral())
            {
                prlvl(os, level);
                os << "Literal type: " << this->getLiteral()->getTypeInfo() << std::endl;
            }
            else
            {
                prlvl(os, level);
                os << "ERROR: literal is NULL!" << std::endl;
                throw std::exception();
            }
            break;
        default:
            break;
    }
    prlvl(os, level);
    os << "Field count/capacity: " << this->fieldCount << "/" << this->fieldCapacity
            << std::endl;
    prlvl(os, level);
    os << "Fields:";
    for(uint32_t i = 0; i < this->fieldCount; i++)
    {
        os << " '" << this->fields[i].name << "'";
    }
    os << std::endl;

    for(uint32_t i = 0; i < this->fieldCount; i++)
    {
        if(*this->fields[i].name)
        {
            if(this->fields[i].object)
            {
                prlvl(os, level);
                os << "[" << this->fields[i].name << "]" << " -> ";
                if(recursive)
                {
                    this->fields[i].object->print(os, level + 1, recursive);
                }
                else
                {
                    os << "<non-recursive-print>";
                }
                os << std::endl;
            }
            else
            {
                prlvl(os, level);
                if(recursive)
                {
                    os << "[" << this->fields[i].name << "]" << " -> "
                            << this->fields[i].object << std::endl;
                }
                else
                {
                    os << "[" << this->fields[i].name << "]" << " -> "
                            << "<non-recursive-print>" << std::endl;
                }
            }
        }
        else
        {
            prlvl(os, level);
            os << "[" << this->fields[i].name << "]" << " -> " << "LITERAL <";
            if(this->getLiteral())
            {
                if(recursive)
                {
                    this->getLiteral()->printValue(os, level + 1, recursive);
                }
                else
                {
                    os << "<non-recursive-print>";
                }
            }
            else
            {
                os << "ERROR NULL !!!" << std::endl;
            }
            os << "> " << std::endl;
        }
    }
    prlvl(os, level - 1);
    os << "}";
}

std::ostream &operator << (std::ostream &os, TTObject *object)
{
    if(!object)
    {
        os << "Object" << std::endl << "{" << std::endl;
        os << "  Address: NULL" << std::endl;
        os << "}";
    }
    else
    {
        object->print(os, 1, false);
    }

    return os;
}
