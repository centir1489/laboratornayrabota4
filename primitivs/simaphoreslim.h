#pragma once
#ifndef PRIMITIV_SIMAPHORESLIM
#define PRIMITIV_SIMAPHORESLIM
#include <iostream>
#include <mutex>
#include <condition_variable>

class SemaphoreSlim{
public:
    explicit SemaphoreSlim(size_t initialCount = 0) : cnt(initialCount) {}

    void wait(){
        std::unique_lock<std::mutex> lock(mutex); // ждём, пока сnt != 0
        cv.wait(lock, [this](){ 
            return cnt > 0; 
            });
        --cnt;
    }

    void release(int releaseCount = 1){
        std::lock_guard<std::mutex> lock(mutex);
        cnt += releaseCount;
        for(size_t q = 0; q < releaseCount; ++q){
            cv.notify_one(); // Уведом
        }
    }

private:
    std::mutex mutex;
    std::condition_variable cv;
    size_t cnt;
};

#endif