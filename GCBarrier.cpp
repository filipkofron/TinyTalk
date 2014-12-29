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
    std::unique_lock<std::mutex> lock(mutex);
    //std::cout << "REG ENTRY" << std::endl;
    if(gcing)
    {
        gcCV.wait(lock);
    }
    threads.insert(std::this_thread::get_id());
    //std::cout << "REG EXIT" << std::endl;
}

void GCBarrier::unreg()
{
    std::unique_lock<std::mutex> lock(mutex);
    if(gcing)
    {
        waiting.insert(std::this_thread::get_id());
        waitingCV.notify_one();
        gcCV.wait(lock);
        waiting.erase(std::this_thread::get_id());
    }
    threads.erase(std::this_thread::get_id());
}

void GCBarrier::enteringAlloc(bool &runAgain)
{
    std::unique_lock<std::mutex> lock(mutex);
    if(allocating)
    {
        /*if(gcRunner != std::this_thread::get_id() || !gcing)
        {*/
            waiting.insert(std::this_thread::get_id());
            waitingCV.notify_one();
            allocatingCV.wait(lock);
            waiting.erase(std::this_thread::get_id());
            runAgain = true;
        /*}*/
    }
    else
    {
        allocating = true;
    }
}

void GCBarrier::leavingAlloc()
{
    std::unique_lock<std::mutex> lock(mutex);
    if(!gcing)
    {
        allocating = false;
        allocatingCV.notify_one();
    }
}

void GCBarrier::enteringGC()
{
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "RUNNING GC -================-" << std::endl;
    gcRunner = std::this_thread::get_id();
    gcing = true;
    while((threads.size() - 1) != waiting.size())
    {
        waitingCV.wait(lock);
    }
}

void GCBarrier::leavingGC()
{
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "LEAVING GC -================-" << std::endl;
    gcing = false;
    gcCV.notify_all();
}

void GCBarrier::tryCatchAnyWhere()
{
    std::unique_lock<std::mutex> lock(mutex);
    if(gcing)
    {
        waiting.insert(std::this_thread::get_id());
        waitingCV.notify_one();
        gcCV.wait(lock);
        waiting.erase(std::this_thread::get_id());
    }
}

void GCBarrier::enteringBlocking()
{
    std::unique_lock<std::mutex> lock(mutex);
    waiting.insert(std::this_thread::get_id());
    waitingCV.notify_one();
}

void GCBarrier::leavingBlocking()
{
    std::unique_lock<std::mutex> lock(mutex);
    if(gcing)
    {
        gcCV.wait(lock);
    }
    waiting.erase(std::this_thread::get_id());
}
