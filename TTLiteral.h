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

    TTObject *onMessage(TTObject *dest, std::string &name, std::vector<std::string> &argNames, std::vector<TTObject *> values);
    TTObject *stringOnMessage(TTObject *dest, std::string &name, std::vector<std::string> &argNames, std::vector<TTObject *> values);
    TTObject *integerOnMessage(TTObject *dest, std::string &name, std::vector<std::string> &argNames, std::vector<TTObject *> values);

    const char *getTypeInfo();

    void printValue(std::ostream &os, const uint32_t &level, const bool &recursive);

    static TTLiteral *createStringLiteral(uint32_t length);
    static TTLiteral *createStringLiteral(const uint8_t *str);

    static TTLiteral *createIntegerLiteral();
    static TTLiteral *createIntegerLiteral(const int32_t &value);

    static TTLiteral *createByteArray(uint32_t size);
    static TTLiteral *createByteArray(const std::vector<uint8_t> &objects);

    static TTLiteral *createObjectArray(uint32_t size);
    static TTLiteral *createObjectArray(const std::vector<TTObject *> &objects);
};

#endif
