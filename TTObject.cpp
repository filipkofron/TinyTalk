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

    for(uint32_t i = 0; i < fieldCount; i++)
    {
        newObject->fields[i].object = fields[i].object->clone();
        newObject->fields[i].name = fields[i].name;
    }

    return newObject;
}

bool TTObject::addField(const uint8_t *name, TTObject *object)
{
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
}

bool TTObject::hasField(const uint8_t *name)
{
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
