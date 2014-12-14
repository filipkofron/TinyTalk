#include "TTLiteral.h"
#include "TTObject.h"
#include "common.h"
#include "Runtime.h"
#include <cstring>

void TTLiteral::_gc_COPY_copy(TTLiteral **ptr, MemAllocator *oldMem, MemAllocator *newMem)
{
#ifdef DEBUG
    std::cout << ">> TTLiteral::_gc_COPY_copy ptr: " << ((unsigned long) (uintptr_t) *ptr) << std::endl;
#endif
//#ifdef DEBUG
//    if(*ptr == NULL)
//    {
//        std::cout << "NULL!" << std::endl;
//        return;
//    }
//    if(oldMem->literals.find((uintptr_t) *ptr) == oldMem->literals.end() && newMem->literals.find((uintptr_t) *ptr) == newMem->literals.end())
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

    if(!oldMem->isInside((uintptr_t) *ptr))
    {
#ifdef DEBUG
        std::cout << "is also not in OLD ????????" << std::endl;
#endif
        return;
    }

    if(ptr[0]->type == LITERAL_TYPE_MOVED_LITERAL)
    {
#ifdef DEBUG
        std::cout << "is moved already" << std::endl;
#endif
        *ptr = (TTLiteral *) *(uintptr_t *) ptr[0];
        return;
    }

#ifdef DEBUG
    std::cout << "copying literal type: " << ptr[0]->getTypeInfo() << ": ";
    ptr[0]->printValue(std::cout, 1, false);
    std::cout << std::endl;
#endif

    TTLiteral *newLit = newMem->allocateLiteral();

    newLit->type = ptr[0]->type;
    newLit->length = ptr[0]->length;
    newLit->data = NULL;
    uint8_t *_gc_tempData = newMem->allocate(ptr[0]->length); // Because of GC
    newLit->data = _gc_tempData;
    newLit->collectiblePtrs = ptr[0]->collectiblePtrs;
    if(ptr[0]->data)
    {
        memcpy(newLit->data, ptr[0]->data, newLit->length);
    }

    ptr[0]->type = LITERAL_TYPE_MOVED_LITERAL;
    *(uintptr_t *) ptr[0] = (uintptr_t) newLit;

    ptr[0] = newLit;

    if(newLit->data && newLit->collectiblePtrs == 1)
    {
        for(uint32_t i = 0; i < newLit->length / sizeof(TTObject *); i ++)
        {
#ifdef DEBUG
            std::cout << "Copying collectible ptr: " << i << " from: " << newLit->length / sizeof(TTObject *) << std::endl;
#endif
            TTObject::_gc_COPY_copy(&((TTObject **) newLit->data)[i], oldMem, newMem);
        }
    }
#ifdef DEBUG
    std::cout << "<< TTLiteral::_gc_COPY_copy" << std::endl;
#endif
}

RefPtr<TTLiteral> TTLiteral::copy(MemAllocator *allocator, RefPtr<TTLiteral> lit)
{
    // TODO: simple copy, since we don't need to recurse and this value is held as a single instance
    // TODO: only, so the GC will be ok with this.
    // TODO: Also, no object references can be yet referenced from these data.

    TTLiteral *_gc_tempLit = MemAllocator::getCurrent()->allocateLiteral();
    RefPtr<TTLiteral> newLit = _gc_tempLit;

    newLit->type = lit->type;
    newLit->length = lit->length;
    newLit->data = NULL;
    newLit->collectiblePtrs = lit->collectiblePtrs;
    uint8_t *_gc_tempLit2 = MemAllocator::getCurrent()->allocate(lit->length);

    newLit->data = _gc_tempLit2;
    memcpy(newLit->data, lit->data, lit->length);

    return newLit;
}

RefPtr<TTLiteral> TTLiteral::clone(RefPtr<TTLiteral> lit)
{
    return copy(MemAllocator::getCurrent(), lit);
}

const char *TTLiteral::getTypeInfo()
{
    const char *res = "INVALID";

    switch(type)
    {
        case LITERAL_TYPE_INTEGER:
            res = "INTEGER";
            break;
        case LITERAL_TYPE_STRING:
            res = "STRING";
            break;
        case LITERAL_TYPE_OBJECT_ARRAY:
            res = "OBJECT ARRAY";
            break;
        case LITERAL_TYPE_BYTE_ARRAY:
            res = "BYTE ARRAY";
            break;
        case LITERAL_TYPE_MOVED_LITERAL:
            res = "MOVED LITERAL";
            break;
    }

    return res;
}

void TTLiteral::printValue(std::ostream &os, const uint32_t &level, const bool &recursive)
{
    std::cout << "TTLiteral::printValue BEGIN" << std::endl;
    std::cout << "TTLiteral::printValue type: " << (unsigned long long) type << std::endl;
    if(!data)
    {
        std::cout << "data NULL!" << std::endl;
        return;
    }
    switch(type)
    {
        case LITERAL_TYPE_INTEGER:
            os << *((int32_t *) data);
            break;
        case LITERAL_TYPE_STRING:
            os << (const char *) data;
            break;
        case LITERAL_TYPE_OBJECT_ARRAY:
            os << "byte size: " << length << std::endl;
            for(uint32_t i = 0; i < length / sizeof(TTObject *); i++)
            {
                prlvl(os, level);
                os << "[" << i << "] -> ";
                if(&((TTObject **) data)[i])
                {
                    if(recursive)
                    {
                        ((TTObject **) data)[i]->print(os, level + 1, recursive);
                    }
                    else
                    {
                        os << "<non-recursive-print>";
                    }
                }
                else
                {
                    os << "NULL";
                }
                os << std::endl;
            }
            prlvl(os, level - 1);
        case LITERAL_TYPE_BYTE_ARRAY:
            os << "size: " << length << std::endl;
            for(uint32_t i = 0; i < length; i++)
            {
                prlvl(os, level);
                os << "[" << i << "] -> ";
                    if(recursive)
                    {
                        os << (unsigned) data[i];
                    }
                    else
                    {
                        os << "<non-recursive-print>";
                    }
                os << std::endl;
            }
            prlvl(os, level - 1);
            break;
    }
    std::cout << "TTLiteral::printValue END" << std::endl;
}

RefPtr<TTObject> TTLiteral::createStringLiteral(uint32_t length)
{
#ifdef DEBUG
    std::cout << "Allocating string literal of length: " << length << std::endl;
#endif
    TTLiteral *laterLit = MemAllocator::getCurrent()->allocateLiteral(); // to initialize
    laterLit->type = LITERAL_TYPE_STRING;
    laterLit->length = 0;
    laterLit->collectiblePtrs = 0;
    laterLit->data = NULL;

    RefPtr<TTLiteral> lit = laterLit;

    lit->length = length + 1;

    uint8_t *_gc_tempLit = MemAllocator::getCurrent()->allocate(lit->length);
    lit->data = _gc_tempLit;
    lit->data[length] = '\0';
#ifdef DEBUG
    std::cout << "Allocating string, ptr =  " << (uintptr_t) lit->data << " content: '" << (const char *) lit->data << "'" << std::endl;
#endif

    RefPtr<TTObject> obj = TTObject::createObject(TT_LITERAL);
    RefPtr<TTObject> parent = Runtime::globalEnvironment->getField(TO_TT_STR("String"));
    obj->setField(TO_TT_STR("parent"), parent);

    obj->setLiteral(lit);

    return obj;
}

RefPtr<TTObject> TTLiteral::createIntegerLiteral()
{
    TTLiteral *_gc_tempLit = MemAllocator::getCurrent()->allocateLiteral();
    RefPtr<TTLiteral> lit = _gc_tempLit;

    lit->type = LITERAL_TYPE_INTEGER;
    lit->length = sizeof(uint32_t);
    uint8_t *_gc_tempLit2 = MemAllocator::getCurrent()->allocate(lit->length);
    lit->data = _gc_tempLit2;
    lit->collectiblePtrs = 0;

    RefPtr<TTObject> obj = TTObject::createObject(TT_LITERAL);
    RefPtr<TTObject> parent = Runtime::globalEnvironment->getField(TO_TT_STR("Integer"));
    obj->setField(TO_TT_STR("parent"), parent);

    obj->setLiteral(lit);

    return obj;
}

RefPtr<TTObject> TTLiteral::createObjectArray(uint32_t size)
{
    TTLiteral *_gc_tempLit = MemAllocator::getCurrent()->allocateLiteral();
    RefPtr<TTLiteral> lit = _gc_tempLit;

    lit->type = LITERAL_TYPE_OBJECT_ARRAY;
    lit->length = (uint32_t) sizeof(TTObject *) * size;
    lit->data = NULL;
    uint8_t *_gc_tempLit2 = MemAllocator::getCurrent()->allocate(lit->length);
    lit->data = _gc_tempLit2;
    lit->collectiblePtrs = 1;

    RefPtr<TTObject> obj = TTObject::createObject(TT_LITERAL);
    RefPtr<TTObject> parent = Runtime::globalEnvironment->getField(TO_TT_STR("Array"));
    obj->setField(TO_TT_STR("parent"), parent);

    obj->setLiteral(lit);

    return obj;
}

RefPtr<TTObject> TTLiteral::createByteArray(uint32_t size)
{
    TTLiteral *_gc_tempLit = MemAllocator::getCurrent()->allocateLiteral();
    RefPtr<TTLiteral> lit = _gc_tempLit;

    lit->type = LITERAL_TYPE_BYTE_ARRAY;
    lit->length = size;
    lit->data = NULL;
    uint8_t *_gc_tempLit2 = MemAllocator::getCurrent()->allocate(lit->length);
    lit->data = _gc_tempLit2;
    lit->collectiblePtrs = 0;

    RefPtr<TTObject> obj = TTObject::createObject(TT_LITERAL);

    obj->setLiteral(&lit);

    return obj;
}

RefPtr<TTObject> TTLiteral::createStringLiteral(const uint8_t *str)
{
    RefPtr<TTObject> obj = createStringLiteral(strlen((const char *) str));
    strcpy((char *)obj->getLiteral()->data, (const char *)str);
    return obj;
}

RefPtr<TTObject> TTLiteral::createIntegerLiteral(const int32_t &value)
{
    RefPtr<TTObject> obj = createIntegerLiteral();

    *((int32_t *) obj->getLiteral()->data) = value;

    return obj;
}

RefPtr<TTObject> TTLiteral::createObjectArray(const std::vector<RefPtr<TTObject> > &objects)
{
    RefPtr<TTObject> obj = createObjectArray(objects.size());

    TTObject **dataObs = ((TTObject **) obj->getLiteral()->data);

    for(auto object : objects)
    {
        *dataObs = &object;
        dataObs++;
    }

    return obj;
}

RefPtr<TTObject> TTLiteral::createByteArray(const std::vector<uint8_t> &bytes)
{
    RefPtr<TTObject> obj = createByteArray(bytes.size());

    uint8_t *dataBytes = obj->getLiteral()->data;

    for(auto &byt : bytes)
    {
        *dataBytes = byt;
        dataBytes++;
    }

    return obj;
}

void TTLiteral::setLiteralParent(RefPtr<TTObject> obj, RefPtr<TTLiteral> lit)
{
    RefPtr<TTObject> ptr;
    if(lit->type == LITERAL_TYPE_STRING)
    {
        ptr = Runtime::globalEnvironment->getField(TO_TT_STR("String"));
        obj->setField(TO_TT_STR("parent"), ptr);
        return;
    }
    if(lit->type == LITERAL_TYPE_INTEGER)
    {
        ptr = Runtime::globalEnvironment->getField(TO_TT_STR("Integer"));
        obj->setField(TO_TT_STR("parent"), ptr);
        return;
    }
    if(lit->type == LITERAL_TYPE_OBJECT_ARRAY)
    {
        ptr = Runtime::globalEnvironment->getField(TO_TT_STR("Array"));
        obj->setField(TO_TT_STR("parent"), ptr);
        return;
    }
    if(lit->type == LITERAL_TYPE_BYTE_ARRAY)
    {
        // TODO: add ByteArray
        /*
        ptr = Runtime::globalEnvironment->getField(TO_TT_STR("Array"));
        obj->setField(TO_TT_STR("parent"), ptr);*/
        return;
    }
    std::cerr << "Unsupported literal type: " << lit->getTypeInfo() << std::endl;
    throw std::exception();
}
