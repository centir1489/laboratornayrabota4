#pragma once
#ifndef PRIMITIV_BARRIER
#define PRIMITIV_BARRIER
#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<atomic>

class Barrier{
public:
    Barrier(size_t cnt) : cnt(cnt), waiting(0), barrier_broken(false) {}

    void wait(){
        std::unique_lock<std::mutex> lock(mtx);
        if(barrier_broken){
            return;
        }
        ++waiting;

        if(waiting == cnt){
            waiting = 0;
            cv.notify_all();
        } 
        else{
            cv.wait(lock);
        }
    }

    void break_barrier(){
        std::unique_lock<std::mutex> lock(mtx);
        barrier_broken = true;
        cv.notify_all();
    }

    bool isBroken(){
        std::unique_lock<std::mutex> lock(mtx);
        return barrier_broken;
    }

private:
    size_t cnt;
    size_t waiting;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> barrier_broken;
};

#endif