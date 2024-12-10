#pragma once
#ifndef PRIMITIV_SPINLOCK
#define PRIMITIV_SPINLOCK
#include <atomic>
#include <thread>
#include <iostream>

class SpinLock{
public:
    SpinLock() : spinlock(false) {}

    void lock(){
        while(spinlock.exchange(true, std::memory_order_acquire));
    }

    void unlock(){
        spinlock.store(false, std::memory_order_release);
    }

private:
    std::atomic<bool> spinlock;
};


#endif