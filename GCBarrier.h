#ifndef GCBARRIER_H
#define GCBARRIER_H

class GCBarrier;

#include <set>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "Lock.h"

// #define SPINLOCK_TICKET // for allocation

class GCBarrier
{
private:
    std::set<std::thread::id> threads;
    std::set<std::thread::id> waiting;
    std::thread::id gcRunner;

    #ifdef SPINLOCK_TICKET
    TicketLock allocMutex;
    #else
    Lock allocMutex;
    #endif

    Lock gcMutex;
    Lock waitingMutex;

    bool allocating;
    bool gcing;

public:
    GCBarrier();

    bool freeHelp;

    void lockBeforeNewThread();
    void unlockBeforeNewThread();

    void lockGC();
    void unlockGC();

    void reg();
    void unreg();

    void enteringAlloc(bool &runAgain);
    void leavingAlloc();

    void enteringGC();
    void leavingGC();

    void tryCatchAnyWhere();

    void enteringBlocking();
    void leavingBlocking();
};

#endif
