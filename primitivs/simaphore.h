#pragma once
#ifndef PRIMITIV_SIMAPHORE
#define PRIMITIV_SIMAPHORE
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


class Semaphore{
public:
    explicit Semaphore(size_t cnt = 0) : cnt(cnt) {}

    void acquire(){
        std::unique_lock<std::mutex> lock(mtx);
        while(cnt <= 0){
            cv.wait(lock);
        }
        --cnt;
    }

    void release(){
        std::unique_lock<std::mutex> lock(mtx);
        ++cnt;
        cv.notify_one();
    }

private:
    std::mutex mtx;
    std::condition_variable cv;
    size_t cnt;
};

#endif