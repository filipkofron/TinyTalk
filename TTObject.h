#ifndef TTOBJECT_H
#define TTOBJECT_H

struct TTObject;

#include <map>
#include <string>
#include <iostream>
#include "MemAllocator.h"
#include "TTLiteral.h"

#define DEFAULT_FIELD_COUNT 8

#define COMPARE_NAME(a, b) strcmp((const char *) a, (const char *) b)

#define TT_NIL 0x00
#define TT_OBJECT 0x01
#define TT_METHOD 0x02
#define TT_EXPR 0x04
#define TT_LITERAL 0x08
#define TT_SYMBOL 0x10
#define TT_ENV 0x20
#define TT_MESSAGE 0x20

#define IS_SOME_TYPE(a, b) (!(!((a)->type == b)))

#define IS_NIL(a) IS_SOME_TYPE(a, TT_NIL)
#define IS_OBJECT(a) IS_SOME_TYPE(a, TT_OBJECT)
#define IS_METHOD(a) IS_SOME_TYPE(a, TT_METHOD)
#define IS_EXPR(a) IS_SOME_TYPE(a, TT_EXPR)
#define IS_LITERAL(a) IS_SOME_TYPE(a, TT_LITERAL)
#define IS_SYMBOL(a) IS_SOME_TYPE(a, TT_SYMBOL)
#define IS_ENV(a) IS_SOME_TYPE(a, TT_ENV)
#define IS_MESSAGE(a) IS_SOME_TYPE(a, TT_MESSAGE)

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
    * Types
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
    uint8_t type;

    /**
    * Type specific flags
    *
    * for EXPRESSION - see Expression.h
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
    static TTObject *createObject(uint8_t type, uint32_t fieldsPreallocated);
    static TTObject *createObject(uint8_t type);

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
    bool addField(const uint8_t *name, TTObject *object);

    /**
    * Returns true if the field name exists.
    */
    bool hasField(const uint8_t *name);

    /**
    * Returns an object from field by given name or NULL if not found.
    */
    TTObject *getField(const uint8_t *name);

    /**
    * Sets an object overwriting the old one, returns false and does nothing if
    * an old one is not found.
    */
    bool setField(const uint8_t *name, TTObject *object);

    /**
    * Returns an object from field by given name or NULL if not found.
    */
    TTLiteral *getLiteral();

    /**
    * Sets an object overwriting the old one, returns false and does nothing if
    * an old one is not found.
    */
    bool setLiteral(TTLiteral *object);

    /**
    * Prints the object to a human readable format.
    */
    void print(std::ostream &os, uint32_t level, bool recursive);
};

/**
* Outputs rich object info.
*/
std::ostream &operator << (std::ostream &os, TTObject *object);

#endif
