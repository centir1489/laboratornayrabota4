#pragma once
#ifndef OUR_PRIMITIV
#define OUR_PRIMITIV
#include<random>
#include"barrier.h"
#include"monitor.h"
#include"simaphore.h"
#include"simaphoreslim.h"
#include"spinlock.h"
#include"spinwait.h"

std::mutex mtx;
Semaphore sem(8);
SemaphoreSlim semslim(8);
SpinLock splock;
Monitor mn;

size_t random_val(size_t LeftBorder, size_t RightBorder){
    std::ranlux24 generator(std::random_device{}());
    std::uniform_int_distribution<size_t> distribution(LeftBorder, RightBorder);
    size_t RdValue = distribution(generator);
    return RdValue;
}

std::string genrndStr(size_t amtsymb){
    std::string rtrStr;
    for(size_t q = 0; q < amtsymb; q++){
        rtrStr += random_val(32, 125);
    }
    std::cout << rtrStr << '\n';
    return rtrStr;
}

void await(std::vector<std::thread>& threads){
    size_t length = threads.size();
    for(size_t q = 0; q < length; q++){
        threads[q].join();
    }
}

static void genrndStrMtx(const size_t& amtSymb){
    std::lock_guard<std::mutex> lock(mtx);
    genrndStr(amtSymb);
}

void puskMutex(const size_t& amtThreads, const size_t& amtSymb){
    std::vector<std::thread> threads;
    for(size_t q = 0; q < amtThreads; q++){
        threads.emplace_back(genrndStrMtx, amtSymb);
    }
    await(threads);
}

static void genrndStrSemaphore(const size_t& amtSymb){
    sem.acquire();
    genrndStr(amtSymb);
    sem.release();
}

void puskSemaphore(const size_t& amtThreads, const size_t& amtSymb){
    std::vector<std::thread> threads;
    for(size_t q = 0; q < amtThreads; q++){
        threads.emplace_back(genrndStrSemaphore, amtSymb);
    }
    await(threads);
}


static void genrndStrSimaphoreSlim(const size_t& amtSymb){
    semslim.wait();
    genrndStr(amtSymb);
    semslim.release();
}

void puskSimaphoreSlim(const size_t& amtThreads, const size_t& amtSymb){
    std::vector<std::thread> threads;
    for(size_t q = 0; q < amtThreads; q++){
        threads.emplace_back(genrndStrSimaphoreSlim, amtSymb);
    }
    await(threads);
}

static void genrndStrSpinLock(const size_t& amtSymb){
    splock.lock();
    genrndStr(amtSymb);
    splock.unlock();
}

void puskSpinLock(const size_t& amtThreads, const size_t& amtSymb){
    std::vector<std::thread> threads;
    for(size_t q = 0; q < amtThreads; q++){
        threads.emplace_back(genrndStrSpinLock, amtSymb);
    }
    await(threads);
}

static void genrndStrWithSpinWait(SpinWait& spinWait, const size_t& amtSymb){
    spinWait.wait();
    genrndStr(amtSymb);
}

void puskSpinWait(const size_t& amtThreads, const size_t& amtSymb){
    std::vector<std::thread> threads;
    SpinWait spinWait;
    spinWait.notify();

    for(size_t q = 0; q < amtThreads; q++){
        threads.emplace_back(genrndStrWithSpinWait, std::ref(spinWait), amtSymb);
    }
    await(threads);
}

static void genrndStrBarrier(Barrier& primitiv, const size_t& amtSymb){
    primitiv.wait();
    genrndStr(amtSymb);
    primitiv.break_barrier();
}

void puskBarrier(const size_t& amtThreads, const size_t& amtSymb){
    std::vector<std::thread> threads;
    Barrier primitiv(amtThreads - 1);
    for(size_t q = 0; q < amtThreads; q++){
        threads.emplace_back(genrndStrBarrier, std::ref(primitiv), amtSymb);
    }
    await(threads);
}

static void genrndStrMonitor(const size_t& amtSymb){
    mn.lock();
    genrndStr(amtSymb);
    mn.unlock();
}

void puskMonitor(const size_t& amtThreads, const size_t& amtSymb){
    std::vector<std::thread> threads;
    for(size_t q = 0; q < amtThreads; q++){
        threads.emplace_back(genrndStrMonitor, amtSymb);
    }
    await(threads);
}


#endif