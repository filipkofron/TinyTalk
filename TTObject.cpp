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
    Field * _temp_GCfields = (Field *) newMem->allocate(sizeof(Field) * newObj->fieldCapacity);
    newObj->fields = _temp_GCfields;
    memcpy(newObj->fields, ptr[0]->fields, sizeof(Field) * newObj->fieldCapacity);
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

    for(uint32_t i = 0; i < newObj->fieldCount; i++)
    {
#ifdef DEBUG
        std::cout << "Will copy '" << newObj->fields[i].name << "'" << std::endl;
#endif
        uint8_t *_temp_GCStr = newMem->cloneString(newObj->fields[i].name);
        newObj->fields[i].name = _temp_GCStr;
#ifdef DEBUG
        std::cout << "Will copy (allocated): '" << newObj->fields[i].name << "'" << std::endl;
        std::cout << " -->> " << std::endl;
#endif
        if(*newObj->fields[i].name)
        {
            _gc_COPY_copy(&newObj->fields[i].object, oldMem, newMem);
        }
        else
        {
            TTLiteral::_gc_COPY_copy((TTLiteral **) &newObj->fields[i].object, oldMem, newMem);
        }
#ifdef DEBUG
        std::cout << " <<-- " << std::endl;
#endif
    }
#ifdef DEBUG
    std::cout << "<< TTObject::_gc_COPY_copy" << std::endl;
#endif
}

RefPtr<TTObject> TTObject::createObject(uint8_t type, uint32_t fieldsPreallocated)
{
    TTObject *tempHax = MemAllocator::getCurrent()->allocateObject();
    tempHax->fieldCapacity = 0;
    tempHax->fieldCount = 0;
    tempHax->type = type;
    tempHax->fields = NULL;
    RefPtr<TTObject> newObject = tempHax;
    Field *fields = (Field *) MemAllocator::getCurrent()->allocate(sizeof(Field) * fieldsPreallocated);
    newObject->fields = fields;
    newObject->fieldCapacity = fieldsPreallocated; // hax for GC, it must not know, because of the above

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
    Field *_temp_GC_Fields = (Field *) MemAllocator::getCurrent()->allocate(sizeof(*newObject->fields) * cloned->fieldCapacity);
    newObject->fields = _temp_GC_Fields;

    memcpy(newObject->fields, cloned->fields, sizeof(*cloned->fields) * cloned->fieldCapacity);

    return newObject;
}

std::vector<std::pair<std::string, RefPtr<TTObject> > > TTObject::laterFields;

bool TTObject::addField(const uint8_t *name, RefPtr<TTObject> object)
{
#ifdef DEBUG
    std::cout << "======================= Adding field of name: " << ((const char *) name) << std::endl;
#endif
    if(!*name)
    {
        return false;
    }
    if(hasField(name))
    {
#ifdef DEBUG
        std::cerr << __FUNCTION__ << ": ERROR: THE FIELD: '" << (const char *) name << "' already exists!!!!!" << std::endl;
        std::cerr << __FUNCTION__ << ": ERROR: THE FIELD: '" << (const char *) name << "' already exists!!!!!" << std::endl;
        std::cerr << __FUNCTION__ << ": ERROR: THE FIELD: '" << (const char *) name << "' already exists!!!!!" << std::endl;
        std::cerr << __FUNCTION__ << ": ERROR: THE FIELD: '" << (const char *) name << "' already exists!!!!!" << std::endl;
        std::cerr << __FUNCTION__ << ": ERROR: THE FIELD: '" << (const char *) name << "' already exists!!!!!" << std::endl;
        std::cerr << __FUNCTION__ << ": ERROR: THE FIELD: '" << (const char *) name << "' already exists!!!!!" << std::endl;
        std::cerr << __FUNCTION__ << ": ERROR: THE FIELD: '" << (const char *) name << "' already exists!!!!!" << std::endl;
        std::cerr << __FUNCTION__ << ": ERROR: THE FIELD: '" << (const char *) name << "' already exists!!!!!" << std::endl;

#endif
        return false;
    }

    RefPtr<TTObject> thiz = this;

    if(thiz->fieldCount == thiz->fieldCapacity)
    {
        thiz->fieldCapacity *= 2;
        Field *newFields = (Field *) MemAllocator::getCurrent()->allocate(sizeof(Field) * thiz->fieldCapacity);
        memcpy(newFields, thiz->fields, sizeof(Field) * thiz->fieldCount);
        thiz->fields = newFields;
    }
    uint8_t *newStr = MemAllocator::getCurrent()->cloneString(name); // Because of GC
#ifdef DEBUG
    std::cout << "cloned str: '" << (const char *) name << "' to '" << (const char *) newStr << "'" << std::endl;
#endif
    thiz->fields[thiz->fieldCount].name = newStr;
    thiz->fields[thiz->fieldCount].object = &object;
    thiz->fieldCount++;

#ifdef DEBUG
    std::cout << "======================= Added field of name: " << ((const char *) name) << std::endl;
#endif

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
#ifdef DEBUG
    std::cout << "TTObject::getField: " << (unsigned long) name << ": '" << (const char *) name << "'"<< std::endl;
    std::cout << "TTObject::getField: THIS: " << (unsigned long) this << std::endl;
#endif
    if(!*name)
    {
        return NULL;
    }

#ifdef DEBUG
    std::cout << "Object contents: " << std::endl;
    print_bytes(sizeof(TTObject), this);
#endif

    for(uint32_t i = 0; i < fieldCount; i++)
    {
#ifdef DEBUG
        std::cout << "TTObject::getField: [" << i << "] " << (unsigned long) fields[i].name << std::endl;
#endif
        if(COMPARE_NAME(name, fields[i].name) == 0)
        {
            return fields[i].object;
        }
    }
    return NULL;
}

bool TTObject::setField(const uint8_t *name, RefPtr<TTObject> object)
{
    if(!*name)
    {
        return false;
    }
    for(uint32_t i = 0; i < fieldCount; i++)
    {
        if(COMPARE_NAME(name, fields[i].name) == 0)
        {
            fields[i].object = &object;
            return true;
        }
    }
    return addField(name, object);
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

bool TTObject::setLiteral(RefPtr<TTLiteral> lit)
{
    if(type != TT_LITERAL)
    {
        std::cerr << "Object is not literal: Cannot set literal value!" << std::endl;
        throw  std::exception();
    }

    TTLiteral::setLiteralParent(this, lit);

    for(uint32_t i = 0; i < fieldCount; i++)
    {
        if(!*fields[i].name)
        {
            fields[i].object = (TTObject *) &lit;
            return true;
        }
    }

    if(fieldCount == fieldCapacity)
    {
        fieldCapacity *= 2;
        Field *newFields = (Field *) MemAllocator::getCurrent()->allocate(sizeof(Field) * fieldCapacity);
        memcpy(newFields, fields, sizeof(Field) * fieldCount);
        fields = newFields;
    }
    uint8_t *_temp_GC_Str = MemAllocator::getCurrent()->cloneString(TO_TT_STR(""));
    fields[fieldCount].name = _temp_GC_Str;
    fields[fieldCount].object = (TTObject *) &lit;
    fieldCount++;

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
            if(&this->fields[i].object)
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
