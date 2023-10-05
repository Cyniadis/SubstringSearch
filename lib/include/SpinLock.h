#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <atomic>

/**
 * @brief SpinLock Simple spin lock to replace std mutex
 */
class SpinLock
{
    std::atomic_flag locked = ATOMIC_FLAG_INIT ;
public:
    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    void unlock() {
        locked.clear(std::memory_order_release);
    }
};

#endif // SPINLOCK_H
