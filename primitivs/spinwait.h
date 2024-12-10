#pragma once
#ifndef PRIMITIV_SPINWAIT
#define PRIMITIV_SPINWAIT
#include <iostream>
#include <thread>
#include <atomic>


class SpinWait{
public:
    SpinWait() : spinwait_brk(false) {}

    void wait(){
        while(!spinwait_brk.load(std::memory_order_acquire)){
            std::this_thread::yield();
        }
    }

    void notify(){
        spinwait_brk.store(true, std::memory_order_release);
    }

private:
    std::atomic<bool> spinwait_brk;
};

#endif