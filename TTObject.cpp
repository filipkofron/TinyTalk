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

bool TTObject::addField(const char *name, TTObject *object)
{
    // TODO: implement
    throw std::exception();
}

bool TTObject::hasField(const char *name)
{
    // TODO: implement
    throw std::exception();
}

TTObject *TTObject::getField(const char *name)
{
    // TODO: implement
    throw std::exception();
}

bool TTObject::setField(const char *name, TTObject *object)
{
    // TODO: implement
    throw std::exception();
}
