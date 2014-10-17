#ifndef TTLITERAL_H
#define TTLITERAL_H

struct TTLiteral;

#include "TTObject.h"
#include "MemAllocator.h"

#define LITERAL_TYPE_STRING 0x00
#define LITERAL_TYPE_INTEGER 0x01
#define LITERAL_TYPE_OBJECT_ARRAY 0x02
// TODO: #define LITERAL_TYPE_BIG_INTEGER 0x03
// TODO: #define LITERAL_TYPE_BYTE_CODE 0x04
// TODO: #define LITERAL_TYPE_MACHINE_CODE 0x05

struct TTLiteral
{
    uint8_t type;
    uint32_t length;
    uint8_t *data;

    TTLiteral *copy(MemAllocator *allocator);

    TTLiteral *clone();

    TTObject *onMessage(TTObject *object);

    TTLiteral *createStringLiteral(uint32_t length);
    TTLiteral *createIntegerLiteral();
    TTLiteral *createObjectArray(uint32_t size);
};

#endif
