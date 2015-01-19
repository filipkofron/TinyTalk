#include <iostream>
#include "GCBarrier.h"
#include "Common.h"

GCBarrier::GCBarrier()
    : gcRunner(std::this_thread::get_id())
{
    allocating = false;
    gcing = false;
}

void GCBarrier::reg()
{
    gcMutex.lock();
    threads.insert(std::this_thread::get_id());
    gcMutex.unlock();
}

void GCBarrier::unreg()
{
    gcMutex.lock();
    threads.erase(std::this_thread::get_id());
    gcMutex.unlock();
}

void GCBarrier::enteringAlloc(bool &runAgain)
{
    runAgain = false;
    if(!allocMutex.tryLock())
    {
        runAgain = true;
        if(gcing)
        {
            if(gcRunner == std::this_thread::get_id())
            {
                runAgain = false;
                return;
            }
        }
        while (!allocMutex.tryLock())
        {
            if(gcing)
            {
                if(gcRunner == std::this_thread::get_id())
                {
                    runAgain = false;
                    return;
                }
            }
            waitingMutex.lock();
            waiting.insert(std::this_thread::get_id());
            waitingMutex.unlock();
        }
        waitingMutex.lock();
        waiting.erase(std::this_thread::get_id());
        waitingMutex.unlock();
    }
}

void GCBarrier::leavingAlloc()
{
    if(gcing && gcRunner == std::this_thread::get_id())
    {
        return;
    }
    allocMutex.unlock();
}

void GCBarrier::enteringGC()
{
    std::cout << " >> Entering GC" << std::endl;
    gcMutex.lock();
    gcRunner = std::this_thread::get_id();
    gcing = true;
    while(true)
    {
        std::cout << " >> Waiting for all threads (current: " << waiting.size() << " all: " << threads.size() << ")." << std::endl;
        waitingMutex.lock();
        if(waiting.size() == (threads.size() - 1)) // not counting this thread
        {
            waitingMutex.unlock();
            break;
        }
        waitingMutex.unlock();
    }
}

void GCBarrier::leavingGC()
{
    gcing = false;
    gcMutex.unlock();
}

void GCBarrier::tryCatchAnyWhere()
{
    if(!gcMutex.tryLock())
    {
        while (!gcMutex.tryLock())
        {
            waitingMutex.lock();
            waiting.insert(std::this_thread::get_id());
            waitingMutex.unlock();
        }
        waitingMutex.lock();
        waiting.erase(std::this_thread::get_id());
        waitingMutex.unlock();
    }
    gcMutex.unlock();
}

void GCBarrier::enteringBlocking()
{
    waitingMutex.lock();
    waiting.insert(std::this_thread::get_id());
    waitingMutex.unlock();
}

void GCBarrier::leavingBlocking()
{
    gcMutex.lock();
    waitingMutex.lock();
    waiting.erase(std::this_thread::get_id());
    waitingMutex.unlock();
    gcMutex.unlock();
}
