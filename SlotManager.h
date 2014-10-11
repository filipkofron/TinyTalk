#ifndef SLOTMANAGER_H
#define SLOTMANAGER_H

struct SlotManager;

#include "TTObject.h"

struct SlotManager
{
    struct NamePair
    {
        uint8_t *name;
        TTObject *object;
    };

    TTObject *slots;

    void init(size_t slotCount);
    void deinit();

    void setSlotIndex(uint16_t idx, TTObject *object);
    TTObject *getSlotIndex(uint16_t idx);

    void setSlotName(const uint8_t *name, TTObject *object);
    TTObject *getSlotName(const uint8_t *name);
};

#endif
