#pragma once

#include <boost/atomic.hpp>
#include "scope_lock.h"

namespace light
{

class spin_lock
{
private:
    typedef enum { Locked, Unlocked } LockState;
    boost::atomic<LockState> state_;

public:
    spin_lock()
        : state_(Unlocked) 
    {
    }

    void lock()
    {
        while (state_.exchange(Locked, boost::memory_order_acquire) == Locked) {
            /* busy-wait */
        }
    }
    void unlock()
    {
        state_.store(Unlocked, boost::memory_order_release);
    }
};

} // namespace light