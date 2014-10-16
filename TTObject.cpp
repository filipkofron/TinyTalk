#include "TTObject.h"
#include <cstring>

TTObject *TTObject::copy(MemAllocator *allocator)
{
    TTObject *newObject = allocator->allocateObject();
    memcpy(newObject, this, sizeof(*this));

    // TODO: implement by the GC algo

    return newObject;
}

TTObject *TTObject::createObject(uint8_t flags, uint32_t fieldsPreallocated)
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    TTObject *newObject = alloc->allocateObject();
    newObject->fields = (Field *) alloc->allocate(sizeof(Field) * fieldsPreallocated);
    newObject->fieldCapacity = fieldsPreallocated;
    newObject->fieldCount = 0;
    newObject->flags = flags;
    return newObject;
}

TTObject *TTObject::createObject(uint8_t flags)
{
    return createObject(flags, DEFAULT_FIELD_COUNT);
}

TTObject *TTObject::clone()
{
    MemAllocator *alloc = MemAllocator::getCurrent();
    TTObject *newObject = alloc->allocateObject();
    memcpy(newObject, this, sizeof(*this));
    newObject->fields = (Field *) alloc->allocate(sizeof(Field) * fieldCapacity);

    if(IS_LITERAL(newObject))
    {
        for(uint32_t i = 0; i < fieldCount; i++)
        {
            if(!*fields[i].name)
            {
                TTLiteral *lit = ((TTLiteral *) fields[i].object)->clone();

                newObject->fields[i].object = (TTObject *) lit;
                newObject->fields[i].name = fields[i].name;
            }
            else
            {
                newObject->fields[i].object = fields[i].object->clone();
                newObject->fields[i].name = fields[i].name;
            }
        }
    }
    else
    {
        for(uint32_t i = 0; i < fieldCount; i++)
        {
            newObject->fields[i].object = fields[i].object->clone();
            newObject->fields[i].name = fields[i].name;
        }
    }

    return newObject;
}

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
    fields[fieldCount].name = alloc->allocateString(name);
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
    for(uint32_t i = 0; i < fieldCount; i++)
    {
        if(!*fields[i].name)
        {
            fields[i].object = (TTObject *) lit;
        }
    }

    return false;
}

std::ostream &operator << (std::ostream &os, TTObject *object)
{
    os << "Object" << std::endl << "{" << std::endl;
    if(!object)
    {
        os << "  Address: NULL";
    }
    else
    {
        os << "  Address: " << std::hex << (unsigned long long) object << std::endl;
        const char *type = "INVALID OBJECT (Invalid flags)";
        switch(object->flags)
        {
            case TT_NIL:
                type = "NIL OBJECT";
                break;
            default:
                break;
        }
        os << "  Type: " << type << std::endl;
        os << "  Field count/capacity: " << object->fieldCount << "/" << object->fieldCapacity
                << std::endl;
        os << "  Fields:";
        for(uint32_t i = 0; i < object->fieldCount; i++)
        {
            os << " '" << object->fields[i].name << "'";
        }
        os << std::endl;
    }
    os << "}";
    return os;
}
