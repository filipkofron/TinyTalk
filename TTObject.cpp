#include "TTObject.h"
#include "Expression.h"
#include "Common.h"
#include "cstdint"
#include "BuiltinUtil.h"
#include "Runtime.h"
#include <cstring>

void TTObject::_gc_COPY_copy(TTObject **ptr, MemAllocator *oldMem, MemAllocator *newMem)
{
#ifdef DEBUG
    std::cout << ">> TTObject::_gc_COPY_copy ptr: " << ((unsigned long) (uintptr_t) *ptr) << std::endl;
#endif
//#ifdef DEBUG
//    if(*ptr == NULL)
//    {
//        std::cout << "NULL!" << std::endl;
//        return;
//    }
//    if(oldMem->objects.find((uintptr_t) *ptr) == oldMem->objects.end() && newMem->objects.find((uintptr_t) *ptr) == newMem->objects.end())
//    {
//        std::cerr << "Address not known!!" << std::endl;
//        KILL;
//    }
//#endif

    if(newMem->isInside((uintptr_t) *ptr))
    {
#ifdef DEBUG
        std::cout << "is new already" << std::endl;
#endif
        return;
    }
    else
    {
        {
#ifdef DEBUG
            std::cout << "not new" << std::endl;
#endif
        }
    }

    if(!oldMem->isInside((uintptr_t) *ptr))
    {
#ifdef DEBUG
        std::cout << "is also not in OLD ????????" << std::endl;
#endif
        return;
    }else
    {
        {
#ifdef DEBUG
            std::cout << "is old" << std::endl;
#endif
        }
    }

    if(IS_MOVED_OBJECT(ptr[0]))
    {
#ifdef DEBUG
        std::cout << "is moved already new addr: " << ((unsigned long) (uintptr_t) *ptr) << std::endl;
#endif
        *ptr = (TTObject *) *(uintptr_t *) ptr[0];
        return;
    }

#ifdef DEBUG
    std::cout << "Making copy of:";
    ptr[0]->print(std::cout, 1, false);
    std::cout << std::endl;
#endif

    TTObject *newObj = newMem->allocateObject();
    memcpy(newObj, *ptr, sizeof(**ptr));
#ifdef DEBUG
    std::cout << "Allocating capacity: newObj->fieldCapacity: " << newObj->fieldCapacity << " newObj->fieldCount: " << newObj->fieldCount << std::endl;
#endif
    newMem->ensure((sizeof(uint8_t *) + sizeof(TTObject *)) * newObj->size);
    uint8_t **_temp_GCnames = (uint8_t **) newMem->allocate(sizeof(uint8_t *) * newObj->size);
    TTObject **_temp_GCobjects = (TTObject **) newMem->allocate(sizeof(TTObject *) * newObj->size);
    newObj->names = _temp_GCnames;
    newObj->objects = _temp_GCobjects;
    memcpy(newObj->names, ptr[0]->names, sizeof(uint8_t *) * newObj->size);
    memcpy(newObj->objects, ptr[0]->objects, sizeof(TTObject *) * newObj->size);
#ifdef DEBUG
    std::cout << "old obj bytes before: " << std::endl;
    print_bytes(sizeof(TTObject), ptr[0]);

    std::cout << "new obj bytes before: " << std::endl;
    print_bytes(sizeof(TTObject), newObj);
#endif
    ptr[0]->type = TT_MOVED_OBJECT;

    *((uintptr_t *) *ptr) = (uintptr_t) newObj;
#ifdef DEBUG
    std::cout << "old obj bytes after: " << std::endl;
    print_bytes(sizeof(TTObject), ptr[0]);

    std::cout << "new obj bytes after: " << std::endl;
    print_bytes(sizeof(TTObject), newObj);

    std::cout << "old addr: " << (unsigned long) ptr[0] << std::endl;
#endif
    ptr[0] = newObj;
#ifdef DEBUG
    std::cout << "new addr: " << (unsigned long) ptr[0] << std::endl;

    std::cout << "DEST obj bytes after: " << std::endl;
    print_bytes(sizeof(TTObject), ptr[0]);


    std::cout << "Traversing rest, fieldcount = " << newObj->fieldCount << std::endl;
#endif

    for(uint32_t i = 0; i < newObj->size; i++)
    {
        if (newObj->names[i])
        {
#ifdef DEBUG
        std::cout << "Will copy '" << newObj->fields[i].name << "'" << std::endl;
#endif
            uint8_t *_temp_GCStr = newMem->cloneString(newObj->names[i]);
            newObj->names[i] = _temp_GCStr;
#ifdef DEBUG
        std::cout << "Will copy (allocated): '" << newObj->fields[i].name << "'" << std::endl;
        std::cout << " -->> " << std::endl;
#endif
            if (*newObj->names[i])
            {
                _gc_COPY_copy(&newObj->objects[i], oldMem, newMem);
            }
            else
            {
                TTLiteral::_gc_COPY_copy((TTLiteral **) &newObj->objects[i], oldMem, newMem);
            }
#ifdef DEBUG
        std::cout << " <<-- " << std::endl;
#endif
        }
#ifdef DEBUG
    std::cout << "<< TTObject::_gc_COPY_copy" << std::endl;
#endif
    }
}

RefPtr<TTObject> TTObject::createObject(uint8_t type, uint32_t fieldsPreallocated)
{
    TTObject *tempHax = MemAllocator::getCurrent()->allocateObject();
    tempHax->size = 0;
    tempHax->count = 0;
    tempHax->type = type;
    tempHax->names = NULL;
    tempHax->objects = NULL;
    RefPtr<TTObject> newObject = tempHax;

    // ensure we don't get the GC call here!
    MemAllocator::getCurrent()->ensure((sizeof(uint8_t *) + sizeof(TTObject *)) * fieldsPreallocated);
    uint8_t **allocedNames = (uint8_t **) MemAllocator::getCurrent()->allocate(sizeof(uint8_t *) * fieldsPreallocated);
    TTObject **allocedObjects = (TTObject **) MemAllocator::getCurrent()->allocate(sizeof(TTObject *) * fieldsPreallocated);
    newObject->names = allocedNames;
    newObject->objects = allocedObjects;
    newObject->size = fieldsPreallocated; // hax for GC, it must not know, because of the above

    // the gc can occure now
    if(type != TT_ENV)
    {
        for (auto pairVal : laterFields)
        {
            newObject->addField(TO_TT_STR(pairVal.first.c_str()), &(pairVal.second));
        }
    }

    return newObject;
}

RefPtr<TTObject> TTObject::createObject(uint8_t type)
{
    return createObject(type, DEFAULT_FIELD_COUNT);
}

RefPtr<TTObject> TTObject::clone(RefPtr<TTObject> cloned)
{
    TTObject *temp_GC_Object = MemAllocator::getCurrent()->allocateObject();
    RefPtr<TTObject> newObject = temp_GC_Object;
    memcpy(&newObject, &cloned, sizeof(*cloned));

    // ensure we don't get the GC call here!
    MemAllocator::getCurrent()->ensure((sizeof(uint8_t *) + sizeof(TTObject *)) * cloned->size);
    uint8_t **_temp_GC_Names = (uint8_t **) MemAllocator::getCurrent()->allocate(sizeof(uint8_t *) * cloned->size);
    TTObject **_temp_GC_Objects = (TTObject **) MemAllocator::getCurrent()->allocate(sizeof(TTObject *) * cloned->size);
    newObject->names = _temp_GC_Names;
    newObject->objects = _temp_GC_Objects;

    memcpy(newObject->names, cloned->names, sizeof(uint8_t *) * cloned->size);
    memcpy(newObject->objects, cloned->objects, sizeof(TTObject *) * cloned->size);

    return newObject;
}

std::vector<std::pair<std::string, RefPtr<TTObject> > > TTObject::laterFields;

bool TTObject::addField(const uint8_t *name, RefPtr<TTObject> object)
{
#ifdef DEBUG
    std::cout << "======================= Adding field of name: " << ((const char *) name) << std::endl;
#endif

    uint32_t hash;

    if(hasField(name))
        return false;

    RefPtr<TTObject> thiz = this;

    if(thiz->count == thiz->size)
    {
        MemAllocator::getCurrent()->ensure((sizeof(uint8_t *) + sizeof(TTObject *)) * ((uint32_t) ((thiz->size + 1) * 2)));
        uint8_t **oldNames = thiz->names;
        TTObject **oldObjects = thiz->objects;

        uint32_t oldSize = thiz->size;

        thiz->size = (uint32_t) ((thiz->size + 1) * 2);

        thiz->names = (uint8_t **) MemAllocator::getCurrent()->allocate(sizeof(uint8_t *) * thiz->size);
        thiz->objects = (TTObject **) MemAllocator::getCurrent()->allocate(sizeof(TTObject *) * thiz->size);

        thiz->count = 0;

        for(uint32_t i = 0; i < oldSize; i++)
        {
            if(oldNames[i])
            {
                thiz->addField(oldNames[i], oldObjects[i]);
            }
        }
    }

    hash = strHash32(name) % thiz->size;

    uint32_t i = hash;
    while(thiz->names[i]) // we have space for sure, find first empty slot
    {
        i++;
        if(i == thiz->size)
        {
            i = 0;
        }
    }
    uint8_t *_gcTEMP_allocedName = MemAllocator::getCurrent()->cloneString(name);
    thiz->names[i] = _gcTEMP_allocedName;
    thiz->objects[i] = &object;

    thiz->count++;

    return true;
}

bool TTObject::hasField(const uint8_t *name)
{
#ifdef DEBUG
    std::cout << "======================= hasField: " << ((const char *) name) << std::endl;
#endif
    uint32_t hash = strHash32(name) % size;
    uint32_t i = hash;
    bool cycle = false;
    while(names[i])
    {
#ifdef DEBUG
        std::cout << "======================= hasField: i: " << i << " hash: " << hash << std::endl;
#endif
        if(COMPARE_NAME(name, names[i]) == 0)
            return true;
        if(cycle && i == hash)
            return false;
        i++;
        if(i == size)
        {
            i = 0;
            cycle = true;
        }
    }

    return false;
}

TTObject *TTObject::getField(const uint8_t *name)
{
#ifdef DEBUG
    std::cout << "======================= getField: " << ((const char *) name) << std::endl;
#endif
    uint32_t hash = strHash32(name) % size;
    uint32_t i = hash;
    bool cycle = false;
    while(names[i])
    {
        if(COMPARE_NAME(name, names[i]) == 0)
            return objects[i];
        if(cycle && i == hash)
            return NULL;
        i++;
        if(i == size)
        {
            i = 0;
            cycle = true;
        }
    }

    return NULL;
}

bool TTObject::setField(const uint8_t *name, RefPtr<TTObject> object)
{
#ifdef DEBUG
    std::cout << "======================= setField: " << ((const char *) name) << std::endl;
#endif
    uint32_t hash = strHash32(name) % size;
    uint32_t i = hash;
    bool cycle = false;
    while(names[i])
    {
        if(COMPARE_NAME(name, names[i]) == 0)
        {
            objects[i] = &object;
            return true;
        }
        if(cycle && i == hash)
            return false;
        i++;
        if(i == size)
        {
            i = 0;
            cycle = true;
        }
    }

    return false;
}


TTLiteral *TTObject::getLiteral()
{
#ifdef DEBUG
    std::cout << "======================= getLiteral " << std::endl;
#endif
    if(type != TT_LITERAL)
    {
        std::cerr << "Object is not literal: Cannot get literal value!" << std::endl;
        throw std::exception();
    }

    uint32_t hash = strHash32(TO_TT_STR("")) % size;
    uint32_t i = hash;
    bool cycle = false;
    while(names[i])
    {
        if(!*names[i])
            return (TTLiteral *) objects[i];
        if(cycle && i == hash)
            return NULL;
        i++;
        if(i == size)
        {
            i = 0;
            cycle = true;
        }
    }

    return NULL;
}

bool TTObject::setLiteral(RefPtr<TTLiteral> lit)
{
#ifdef DEBUG
    std::cout << "======================= setLiteral " << std::endl;
#endif
    if(type != TT_LITERAL)
    {
        std::cerr << "Object is not literal: Cannot set literal value!" << std::endl;
        throw  std::exception();
    }
    RefPtr<TTObject> thiz = this;

    TTLiteral::setLiteralParent(&thiz, lit);

    if(thiz->count == thiz->size)
    {
        MemAllocator::getCurrent()->ensure((sizeof(uint8_t *) + sizeof(TTObject *)) * ((uint32_t) ((thiz->size + 1) * 2)));
        uint8_t **oldNames = thiz->names;
        TTObject **oldObjects = thiz->objects;

        uint32_t oldSize = thiz->size;

        thiz->size = (uint32_t) ((thiz->size + 1) * 2);

        thiz->names = (uint8_t **) MemAllocator::getCurrent()->allocate(sizeof(uint8_t *) * thiz->size);
        thiz->objects = (TTObject **) MemAllocator::getCurrent()->allocate(sizeof(TTObject *) * thiz->size);

        thiz->count = 0;

        for(uint32_t i = 0; i < oldSize; i++)
        {
            if(oldNames[i])
            {
                thiz->addField(oldNames[i], oldObjects[i]);
            }
        }
    }

    uint32_t hash = strHash32(TO_TT_STR("")) % thiz->size;

    uint32_t i = hash;
    while(thiz->names[i]) // we have space for sure, find first empty slot
    {
        i++;
        if(i == thiz->size)
        {
            i = 0;
        }
    }
    uint8_t *_gcTEMP_allocedName = MemAllocator::getCurrent()->cloneString(TO_TT_STR(""));
    thiz->names[i] = _gcTEMP_allocedName;
    thiz->objects[i] = (TTObject *) &lit;

    thiz->count++;

    return true;
}

void TTObject::print(std::ostream &os, uint32_t level, bool recursive)
{
    std::cout << "TTObject::print" << std::endl;
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
        case TT_STACK_FRAME:
            type = "STACT FRAME";
            break;
        case TT_MOVED_OBJECT:
            type = "MOVED OBJECT";
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
            }
            break;
        default:
            break;
    }
    prlvl(os, level);
    os << "Field count/capacity: " << this->count << "/" << this->size
            << std::endl;
    prlvl(os, level);
    os << "Fields:";
    for(uint32_t i = 0; i < this->size; i++)
    {
        if(this->names[i])
        {
            os << " '" << this->names[i] << "'";
        }
    }
    os << std::endl;

    for(uint32_t i = 0; i < this->size; i++)
    {
        if(!this->names[i]) continue;
        if(*this->names[i])
        {
            if(&this->objects[i])
            {
                prlvl(os, level);
                os << "[" << this->names[i] << "]" << " -> ";
                if(recursive)
                {
                    this->objects[i]->print(os, level + 1, recursive);
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
                    os << "[" << this->names[i] << "]" << " -> "
                            << this->objects[i] << std::endl;
                }
                else
                {
                    os << "[" << this->names[i] << "]" << " -> "
                            << "<non-recursive-print>" << std::endl;
                }
            }
        }
        else
        {
            prlvl(os, level);
            os << "[" << this->names[i] << "]" << " -> " << "LITERAL <";
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

std::ostream &operator << (std::ostream &os, RefPtr<TTObject> &object)
{
    if(!&object)
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
