#include <iostream>
#include "GCBarrier.h"
#include "Common.h"
#include "Runtime.h"

GCBarrier::GCBarrier()
    : gcRunner(std::this_thread::get_id())
{
    allocating = false;
    gcing = false;
    freeHelp = true;
}

void GCBarrier::lockBeforeNewThread()
{
    waitingMutex.lock();
    waiting.insert(std::this_thread::get_id());
    waitingMutex.unlock();
    gcMutex.lock();
    waitingMutex.lock();
    waiting.erase(std::this_thread::get_id());
    waitingMutex.unlock();
}

void GCBarrier::unlockBeforeNewThread()
{
    gcMutex.unlock();
}

void GCBarrier::lockGC()
{
    gcMutex.lock();
}

void GCBarrier::unlockGC()
{
    gcMutex.unlock();
}

void GCBarrier::reg()
{
    gcMutex.lock();
    threads.insert(std::this_thread::get_id());
    gcMutex.unlock();
}

void GCBarrier::unreg()
{
    waitingMutex.lock();
    waiting.insert(std::this_thread::get_id());
    waitingMutex.unlock();
    gcMutex.lock();
    waitingMutex.lock();
    waiting.erase(std::this_thread::get_id());
    waitingMutex.unlock();
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
            //allocMutex.lock();
            //break;
            //std::chrono::nanoseconds sleepTime((rand() % 2000) * Runtime::interpretersAlive.size());
            //std::this_thread::sleep_for(sleepTime);

            int max = 20000;
            for(int i = 0; i < (rand() % 500) != 0 && !freeHelp && max--; i++)
            {
            }

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

int HAAAX = 0;

void GCBarrier::enteringGC()
{
    gcMutex.lock();
    gcRunner = std::this_thread::get_id();
    gcing = true;
    std::chrono::milliseconds sleepTime(1);

    while(true)
    {
#ifdef VERBOSE
        std::cout << " >> Waiting for all threads (current: " << waiting.size() << " all: " << threads.size() << ")." << std::endl;
#endif
        if(HAAAX++ > 10000) KILL;
        waitingMutex.lock();
        if(waiting.size() == (threads.size() - 1)) // not counting this thread
        {
            waitingMutex.unlock();
            break;
        }
        waitingMutex.unlock();
        std::this_thread::sleep_for(sleepTime);
    }
    HAAAX = 0;
}

void GCBarrier::leavingGC()
{
    gcing = false;
    gcMutex.unlock();
}

void GCBarrier::tryCatchAnyWhere()
{
    std::chrono::milliseconds sleepTime(1);
    if(!gcMutex.tryLock())
    {
        while (!gcMutex.tryLock())
        {
            waitingMutex.lock();
            waiting.insert(std::this_thread::get_id());
            waitingMutex.unlock();
            std::this_thread::sleep_for(sleepTime);
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
