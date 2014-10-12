#ifndef SLOTMANAGER_CPP
#define SLOTMANAGER_CPP

#include "MemAllocator.h"
#include "TTObject.h"

void SlotManager::init(size_t slotCount)
{
    this->slotCount = slotCount;
    slots = (NamePair *) MemAllocator::getDefault()->allocate(sizeof(NamePair) * slotCount);
}

void SlotManager::deinit()
{
    // No need, the GC should clean up
}

bool SlotManager::addSlotName(const uint8_t *name, TTObject *object)
{
    if(slotsUsed < slotCount)
    {
        uint8_t *newName = MemAllocator::getDefault()->allocateString(name);
        NamePair namePair;
        namePair.name = newName;
        namePair.object = object;
        slots[slotsUsed++] = namePair;
        return true;
    }

    return false;
}

bool SlotManager::setSlotName(const uint8_t *name, TTObject *object)
{
    for(size_t s = 0; s < slotsUsed; s++)
    {
        if(cmpName(slots[s].name, name) == 0)
        {
            uint8_t *newName = MemAllocator::getDefault()->allocateString(name);
            slots[s].name = newName;
            return true;
        }
    }
    return false;
}

TTObject *SlotManager::getSlotName(const uint8_t *name)
{
    for(size_t s = 0; s < slotsUsed; s++)
    {
        if(cmpName(slots[s].name, name) == 0)
        {
            uint8_t *newName = MemAllocator::getDefault()->allocateString(name);
            slots[s].name = newName;
            return slots[s].object;
        }
    }
    return NULL;
}

#endif
