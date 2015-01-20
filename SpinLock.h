#ifndef SPINLOCK_H
#define SPINLOCK_H

class SpinLock;
/*
#include <atomic>

class SpinLock
{
public:
    void lock();

    void unlock();

private:
    std::atomic_flag lck = ATOMIC_FLAG_INIT;
};*/

#include <atomic>
#include <thread>

class SpinLock
{
public:

    inline SpinLock() :
            m_lock(ATOMIC_FLAG_INIT)
    {
    }

    inline SpinLock(const SpinLock &) :
            m_lock(ATOMIC_FLAG_INIT)
    {
    }

    inline SpinLock &operator=(const SpinLock &)
    {
        return *this;
    }

    inline void lock()
    {
        while (true)
        {
            for (int32_t i = 0; i < 10000; ++i)
            {
                if (!m_lock.test_and_set(std::memory_order_acquire))
                {
                    return;
                }
            }

            std::this_thread::yield();  // A great idea that you don't see in many spinlock examples
        }
    }

    inline bool tryLock()
    {
        return !m_lock.test_and_set(std::memory_order_acquire);
    }

    inline void unlock()
    {
        m_lock.clear(std::memory_order_release);
    }

private:

    std::atomic_flag m_lock;
};
/*
#include <mutex>

class SpinLock
{
public:
    inline void lock()
    {
        mutex.lock();
    }
    inline void unlock()
    {
        mutex.unlock();
    }
    inline bool tryLock()
    {
        return mutex.try_lock();
    }

private:

    std::mutex mutex;
};*/
#endif