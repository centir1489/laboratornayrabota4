#pragma once
#ifndef TIMER
#define TIMER
#include<iostream>
#include<thread>
#include<mutex>

class Timer{
public:
    std::string name = "StandartTimer";
    
    Timer(std::string name) : name(name){
        begin = std::chrono::high_resolution_clock::now();
        itWorks = true;
    }

    Timer(){
        begin = std::chrono::high_resolution_clock::now();
        itWorks = true;
    }

    ~Timer(){
        if(itWorks){
            stop();
        }
    }

    void stop() {
        if(itWorks){
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - begin;
            std::cout << '\n' << name << ' ' << duration.count() << " секунд~с\n";
            itWorks = false; // Устанавливаем флаг, что таймер остановлен
        } 
        else{
            std::cout << "Таймер в данный момент не работал\n";
        }
    }

    void reset() {
        begin = std::chrono::high_resolution_clock::now();
        itWorks = true; 
    }

private:
    std::chrono::high_resolution_clock::time_point begin, end;
    bool itWorks;
};

#endif