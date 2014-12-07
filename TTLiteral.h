#ifndef TTLITERAL_H
#define TTLITERAL_H

struct TTLiteral;

#include <vector>
#include "TTObject.h"
#include "MemAllocator.h"
#include "RefPtr.h"

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

    static RefPtr<TTLiteral> copy(MemAllocator *allocator, RefPtr<TTLiteral> lit);

    static RefPtr<TTLiteral> clone(RefPtr<TTLiteral> lit);

    const char *getTypeInfo();

    void printValue(std::ostream &os, const uint32_t &level, const bool &recursive);

    static RefPtr<TTObject> createStringLiteral(uint32_t length);
    static RefPtr<TTObject> createStringLiteral(const uint8_t *str);

    static RefPtr<TTObject> createIntegerLiteral();
    static RefPtr<TTObject> createIntegerLiteral(const int32_t &value);

    static RefPtr<TTObject> createByteArray(uint32_t size);
    static RefPtr<TTObject> createByteArray(const std::vector<uint8_t> &objects);

    static RefPtr<TTObject> createObjectArray(uint32_t size);
    static RefPtr<TTObject> createObjectArray(const std::vector<RefPtr<TTObject> > &objects);

    static void setLiteralParent(RefPtr<TTObject> obj, RefPtr<TTLiteral> lit);
};

#endif
