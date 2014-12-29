#ifndef GCBARRIER_H
#define GCBARRIER_H

class GCBarrier;

#include <set>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "SpinLock.h"

class GCBarrier
{
private:
    std::set<std::thread::id> threads;
    std::set<std::thread::id> waiting;
    std::thread::id gcRunner;
    std::mutex mutex;
    std::condition_variable allocatingCV;
    std::condition_variable waitingCV;
    std::condition_variable gcCV;

    bool allocating;
    bool gcing;

public:
    GCBarrier();

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
