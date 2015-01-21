#ifndef SPINLOCK_H
#define SPINLOCK_H

#define SPINLOCK_SIMPLE
//#define SPINLOCK_MUTEX

class Lock;

#include <atomic>
#include <thread>

#if defined(__i386__) || defined(__x86_64__) ||  \
    defined(__amd64__) || defined(__ia64__)
#define SPINWAIT()  do { __asm __volatile("pause"); } while (0)
#else
#define SPINWAIT()  do { /* nothing */ } while (0)
#endif

#ifdef SPINLOCK_SIMPLE
class Lock
{
public:

    inline Lock() :
            m_lock(ATOMIC_FLAG_INIT)
    {
    }

    inline Lock(const Lock &) :
            m_lock(ATOMIC_FLAG_INIT)
    {
    }

    inline Lock &operator=(const Lock &)
    {
        return *this;
    }

    inline void lock()
    {
        int counter = 0;
        while (true)
        {
            if (!m_lock.test_and_set(std::memory_order_acquire))
            {
                return;
            }

            counter++;
            if(counter > 500)
                sched_yield();
            //sched_yield();
            //SPINWAIT();

            for(int i = 0; i < 100; i++)
                SPINWAIT();
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
#endif

#ifdef SPINLOCK_MUTEX
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
};
#endif

#include <atomic>
#include <cstdlib>
#include <cstdio>
//#include <pthread.h>
//#include <sched.h>
#include <cerrno>

class TicketLock
{
private:

    std::atomic<long> nextTicket;
    char pad[64];
    std::atomic<long> currentTicket;

public:
    TicketLock()
    {
        std::atomic_store(&nextTicket, 0L);
        std::atomic_store(&currentTicket, 0L);
    }

    ~TicketLock()
    {
        std::atomic_store(&nextTicket, 0L);
        std::atomic_store(&currentTicket, 0L);
    }

    inline void lock()
    {
        long ticket = std::atomic_fetch_add(&nextTicket, 1L);
        while (ticket != std::atomic_load(&currentTicket))
        {
            //sched_yield();
            SPINWAIT();
        }
    }

    inline void unlock()
    {
        long ticket = std::atomic_load_explicit(&currentTicket, std::memory_order_relaxed);
        std::atomic_store(&currentTicket, ticket + 1L);
    }

    inline long getTicket()
    {
        return std::atomic_fetch_add(&nextTicket, 1L);
    }

    inline bool tryTicket(long ticket)
    {
        return ticket == std::atomic_load(&currentTicket);
    }
};

#endif
