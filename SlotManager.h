#ifndef SLOTMANAGER_H
#define SLOTMANAGER_H

struct SlotManager;

#include "TTObject.h"
#include <cstring>

#define cmpName(a, b) strcmp((const char *) a, (const char *) b)

struct SlotManager
{
    struct NamePair
    {
        uint8_t *name;
        TTObject *object;
    };

    // TODO: GC must find these slots
    NamePair *slots;

    size_t slotCount;
    size_t slotsUsed;

    void init(size_t slotCount);
    void deinit();

    bool addSlotName(const uint8_t *name, TTObject *object);
    bool setSlotName(const uint8_t *name, TTObject *object);
    TTObject *getSlotName(const uint8_t *name);
};

#endif
