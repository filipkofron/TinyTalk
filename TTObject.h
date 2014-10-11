#ifndef STOBJECT_HPP
#define STOBJECT_HPP

struct TTObject;

#include <map>
#include <string>
#include "TTMessage.h"
#include "SlotManager.h"

struct TTObject
{
    uint8_t flags;
    uint16_t slotCount;

    SlotManager slotManager;

    void init();
    void deinit();
};

#endif
