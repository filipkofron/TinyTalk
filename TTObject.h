#ifndef STOBJECT_HPP
#define STOBJECT_HPP

struct TTObject;

#include <map>
#include <string>
#include "MemAllocator.h"

#define DEFAULT_FIELD_COUNT 8

#define COMPARE_NAME(a, b) strcmp((const char *) a, (const char *) b)

#define OBJECT_MASK 0x01
#define METHOD_MASK 0x02
#define EXPR_MASK 0x04
#define LITERAL_MASK 0x08
#define SYMBOL_MASK 0x10
#define ENV_MASK 0x20
#define MESSAGE_MASK 0x20

#define IS_SOME_TYPE(a, b) (!(!((a)->flags & b)))

#define IS_OBJECT(a) IS_SOME_TYPE(a, OBJECT_MASK)
#define IS_METHOD(a) IS_SOME_TYPE(a, METHOD_MASK)
#define IS_EXPR(a) IS_SOME_TYPE(a, EXPR_MASK)
#define IS_LITERAL(a) IS_SOME_TYPE(a, LITERAL_MASK)
#define IS_SYMBOL(a) IS_SOME_TYPE(a, SYMBOL_MASK)
#define IS_ENV(a) IS_SOME_TYPE(a, ENV_MASK)
#define IS_MESSAGE(a) IS_SOME_TYPE(a, MESSAGE_MASK)


/**
* TTObject is any object within this VM, that can be directly interfaced from
* the language itself.
*
* This abstract object divides into multiple basic objects identified by the VM.
*
* Any other object can be described by the object flag, as specified below.
* This might cause confusion, should be understood as a custom object identified
* the same way by the VM.
*/
struct TTObject
{
    struct Field
    {
        uint8_t *name;
        TTObject *object;
    };

    /**
    * FLAGS
    *
    * Object  :  value     ~   hexa
    * ====== == ======
    * nil     :  0000 0000 ~   0x00
    * object  :  0000 0001 ~   0x01
    * method  :  0000 0010 ~   0x02
    * expr    :  0000 0100 ~   0x04
    * literal :  0000 1000 ~   0x08
    * symbol  :  0001 0000 ~   0x10
    * env     :  0010 0000 ~   0x20
    * message :  0100 0000 ~   0x40
    *
    */
    uint8_t flags;

    Field *fields;
    uint32_t fieldCount;
    uint32_t fieldCapacity;

    /**
    * For future stuff (GC) only!
    */
    TTObject *copy(MemAllocator *allocator);

    /**
    * Objects must only be created using these functions.
    */
    static TTObject *createObject(uint8_t flags, uint32_t fieldsPreallocated);
    static TTObject *createObject(uint8_t flags);

    /**
    * Create deep copy of the object. Field names will not be cloned though, but the objects will!
    */
    TTObject *clone();


    /**
    * Field operations.
    */

    /**
    * Add a new field. Will return false if it alread exists.
    */
    bool addField(const char *name, TTObject *object);

    /**
    * Returns true if the field name exists.
    */
    bool hasField(const char *name);

    /**
    * Returns an object from field by given name or NULL if not found.
    */
    TTObject *getField(const char *name);

    /**
    * Sets an object overwriting the old one, returns false and does nothing if
    * an old one is not found.
    */
    bool setField(const char *name, TTObject *object);
};

#endif
