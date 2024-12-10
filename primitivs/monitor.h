#pragma once
#ifndef PRIMITIV_MONITOR
#define PRIMITIV_MONITOR
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class Monitor{
public:
    void lock(){
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [this](){ 
            return !islocked;
            });
        islocked = true;
    }

    void unlock(){
        {
            std::lock_guard<std::mutex> lk(mtx);
            islocked = false;
        }
        cv.notify_one();
    }

private:
    std::mutex mtx;
    std::condition_variable cv;
    bool islocked = false;
};

#endif