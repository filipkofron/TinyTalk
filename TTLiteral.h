#ifndef TTLITERAL_H
#define TTLITERAL_H

struct TTLiteral;

#include <vector>
#include "TTObject.h"
#include "MemAllocator.h"

#define LITERAL_TYPE_STRING 0x00
#define LITERAL_TYPE_INTEGER 0x01
#define LITERAL_TYPE_OBJECT_ARRAY 0x02
#define LITERAL_TYPE_BYTE_ARRAY 0x03
// TODO: #define LITERAL_TYPE_BIG_INTEGER 0x03
// TODO: #define LITERAL_TYPE_BYTE_CODE 0x04
// TODO: #define LITERAL_TYPE_MACHINE_CODE 0x05

struct TTLiteral
{
    uint8_t type;
    uint32_t length;
    uint8_t *data;

    static TTLiteral *copy(MemAllocator *allocator, TTLiteral *lit);

    static TTLiteral *clone(TTLiteral *lit);

    const char *getTypeInfo();

    void printValue(std::ostream &os, const uint32_t &level, const bool &recursive);

    static TTObject *createStringLiteral(uint32_t length);
    static TTObject *createStringLiteral(const uint8_t *str);

    static TTObject *createIntegerLiteral();
    static TTObject *createIntegerLiteral(const int32_t &value);

    static TTObject *createByteArray(uint32_t size);
    static TTObject *createByteArray(const std::vector<uint8_t> &objects);

    static TTObject *createObjectArray(uint32_t size);
    static TTObject *createObjectArray(const std::vector<TTObject *> &objects);
};

#endif
