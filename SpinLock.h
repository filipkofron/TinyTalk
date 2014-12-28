#ifndef SPINLOCK_H
#define SPINLOCK_H

class SpinLock;

#include <atomic>

class SpinLock
{
public:
    void lock();

    void unlock();

private:
    std::atomic_flag lck = ATOMIC_FLAG_INIT;
};

#endif