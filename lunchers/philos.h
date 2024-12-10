#pragma once
#ifndef PHILOS
#define PHILOS
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <cstdlib> 

const size_t AMOUNT_PHILOS = 8;

std::mutex mtx_1;
std::mutex forks[AMOUNT_PHILOS];
size_t philosofs[AMOUNT_PHILOS];

void outStatus(size_t numbphil, std::string action){
    mtx_1.lock();
    std::cout << "Философ " + std::to_string(numbphil + 1) << ' ' + action + '\n';
    mtx_1.unlock();
}


void philosof(size_t numbPhilos){
    while(true){
        // std::system("clear");
        outStatus(numbPhilos, " думает");
        std::this_thread::sleep_for(std::chrono::milliseconds(2500)); // задержка пока думает

        //опередление вилки
        size_t leftFork = numbPhilos;
        size_t rightFork = (numbPhilos + 1) % AMOUNT_PHILOS;
        {
            std::lock(forks[leftFork], forks[rightFork]);
            std::lock_guard<std::mutex> leftLock(forks[leftFork], std::adopt_lock);
            std::lock_guard<std::mutex> rightLock(forks[rightFork], std::adopt_lock);

            outStatus(numbPhilos, " ест");
            std::this_thread::sleep_for(std::chrono::milliseconds(2500)); // опять задержка
        }
    }
}

void philos(){
    std::vector<std::thread> philosophs;
    for(size_t q = 0; q < AMOUNT_PHILOS; ++q){
        philosophs.emplace_back(philosof, q);
    }

    for(auto &philosopher : philosophs){
        philosopher.join();
    }
}


#endif