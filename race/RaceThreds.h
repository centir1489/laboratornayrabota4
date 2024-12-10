#pragma once
#ifndef RACE_THREADS
#define RACE_THREADS
#include<iostream>
#include<vector>
#include<cstdlib> 
#include<ostream>
#include<random>
#include<thread>
#include<mutex>
#include<utility>
#include"timer.h"

const size_t SIZE_COMPANY = 10000;
const size_t AMOUNT_THREADED = 12;


const std::vector<std::string> SURNAME = {
    "Petrov", "Ivanov", "Sidorov", "Smirnov", "Kuznetsov", "Popov", "Vasiliev", "Mikhailov", "Fedorov", "Novikov", 
    "Morozov", "Lebedev", "Semenov", "Golubev", "Vinogradov", "Borisov", "Dmitriev", "Kovalev", "Zaitsev", "Karpov", 
    "Tikhonov", "Gusev", "Sokolov", "Pavlov", "Frolov", "Belyakov", "Kolesnikov", "Makarov", "Grigorev", "Nikolaev", 
    "Alekseev", "Shestakov", "Zhuravlev", "Kuzmin", "Semyonov", "Cherepanov", "Krylov", "Bashirov", "Deryabin", "Kovtun", 
    "Svetlov", "Trofimov", "Yakovlev", "Fomin", "Savin", "Kuznetsova", "Petrova"
    };

const std::vector<std::string> POST = {
    "Analyst", "Designer", "Developer", "Consultant", "Coordinator", "Specialist", "Technician", "Administrator", 
    "Salesperson", "Accountant", "Auditor", "Scientist", "Writer", "Editor", "Artist", "Photographer", "Marketer", "Strategist", "Trainer", 
    "Instructor", "Operator", "Supervisor", "Planner", "Broker", "Agent", "Director of Operations", "Project Manager", "Quality Assurance", 
    "Data Scientist", "Web Developer", "Graphic Designer", "Social Media Manager", "Content Creator", "Financial Analyst", "Human Resources Manager", 
    "IT Support", "Legal Advisor", "Public Relations Officer", "Customer Service Representative", "Logistics Coordinator", "Business Analyst", 
     
}; 

size_t RandomValues(size_t LeftBorder, size_t RightBorder){
    std::ranlux24 generator(std::random_device{}());
    std::uniform_int_distribution<size_t> distribution(LeftBorder, RightBorder - 1); 
    return distribution(generator);
}


struct Collaborator{
public:    
    std::string surename;
    std::string post;
    uint16_t wage;
    uint16_t age;
    Collaborator() : surename(gen_surname()), post(gen_post()), wage(gne_wage(5, 96)), age(gen_age(20, 71)) {}

    friend std::ostream& operator<<(std::ostream& os, Collaborator clltr){
        os << "==================\n" << "Surname: " << clltr.surename << "\nPost: " << clltr.post 
           << "\nWage: " << clltr.wage << "\nAge: " << clltr.age 
           << "\n==================\n"; 
        return os;
    }

private:
    std::string gen_surname(){
        return SURNAME[RandomValues(0, SURNAME.size())];
    }

    std::string gen_post(){
        return POST[RandomValues(0, POST.size())];
    }

    uint16_t gne_wage(size_t LeftBorder, size_t RightBorder){
        return RandomValues(LeftBorder, RightBorder) * 100;
    }

    uint8_t gen_age(size_t LeftBorder, size_t RightBorder){
        return RandomValues(LeftBorder, RightBorder);
    }
};

struct Company{
public:    
    std::vector<Collaborator> company;
    std::mutex mtx;

    Company(size_t amt){
        for(size_t q = 0; q < amt; q++){
            Collaborator addclltr;
            company.push_back(addclltr);
        }
    }

    void get(size_t border){
        std::cout << "Company\n";
        for(size_t q = 0; q < company.size(); q++){
            if(border < q){
                break;
            }
            std::cout << q + 1 << '\n' << company[q];
        }
    }


    uint16_t clac_average_age(const std::string& post){
        int ourSumm = 0, amt = 0;
        for(Collaborator clltr : company){
            if(post == clltr.post){
                amt++;
                ourSumm += clltr.age;
            }
        }
        return amt > 0 ? static_cast<uint16_t>(ourSumm / amt) : 0;
    }

    std::pair<uint16_t, uint16_t> calc_result(const std::string& post){
        std::pair<uint8_t, uint16_t> result;
        result.first = clac_average_age(post);
        uint16_t maxWage = 0;
        for(Collaborator clltr : company){
            if(post == clltr.post && (clltr.age >= result.first - 2 && clltr.age <= result.first + 2) && maxWage < clltr.wage){
                maxWage = clltr.wage;
            }
        }
        result.second = maxWage;
        return result;
    }


    uint16_t clac_average_age(const std::string& post, const size_t& amtthreads){
        uint ourSumm = 0, amt = 0;
        std::vector<std::thread> threads;
        std::vector<std::pair<uint, uint>> results;

        // разделяем рбаоту на потоки
        size_t numThreads = std::min(amtthreads, company.size()); 
        size_t chunkSize = company.size() / numThreads;

        for(size_t q = 0; q < numThreads; ++q){
            threads.emplace_back([&, q](){
                uint8_t localSumm = 0, localAmt = 0;
                size_t begin = q * chunkSize;
                size_t end = (q == numThreads - 1) ? company.size() : begin + chunkSize;

                for(size_t p = begin; p < end; ++p){
                    if (post == company[p].post) {
                        localAmt++;
                        localSumm += company[p].age;
                    }
                }

                std::lock_guard<std::mutex> lock(mtx);
                results.emplace_back(localSumm, localAmt);
            });
        }

        for(auto& thread : threads){
            thread.join();
        }

        for(const auto& result : results){
            ourSumm += result.first;
            amt += result.second;
        }

        return amt > 0 ? static_cast<uint16_t>(ourSumm / amt) : 0;
    }

    std::pair<uint16_t, uint32_t> calc_result(const std::string& post, const size_t& amtthreads){
        std::pair<uint16_t, uint32_t> result;
        result.first = clac_average_age(post);
        uint32_t maxWage = 0;

        std::vector<std::thread> threads;
        std::vector<uint32_t> maxWages; // хранение для разных потоков

        size_t numThreads = std::min(amtthreads, company.size()); 
        size_t chunkSize = company.size() / numThreads;

        for(size_t q = 0; q < numThreads; ++q){
            threads.emplace_back([&, q](){
                uint32_t localMaxWage = 0;
                size_t begin = q * chunkSize;
                size_t end = (q == numThreads - 1) ? company.size() : begin + chunkSize;

                for(size_t p = begin; p < end; ++p){
                    if (post == company[p].post && 
                        (company[p].age >= result.first - 2 && company[p].age <= result.first + 2) && 
                        localMaxWage < company[p].wage) {
                        localMaxWage = company[p].wage;
                    }
                }

                std::lock_guard<std::mutex> lock(mtx);
                maxWages.push_back(localMaxWage);
            });
        }

        for(auto& thread : threads){
            thread.join();
        }

        for(const auto& wage : maxWages){
            if(wage > maxWage){
                maxWage = wage;
            }
        }
        result.second = maxWage;
        return result;
    }

};




void race(){
    Company company(SIZE_COMPANY);

    std::string somePost = POST[RandomValues(0, POST.size())];
    
    Timer single("Timer_Mono");
    auto resultSingle = company.calc_result(somePost);
    single.stop();
    
    
    Timer mult("Timer_multi");
    auto resultMulti = company.calc_result(somePost, AMOUNT_THREADED);
    mult.stop();

    std::cout << "POST: " << somePost << '\n';
    std::cout << "Mono result:\n";
    std::cout << "Average age: " << static_cast<uint16_t>(resultSingle.first) << ", max wage: " << resultSingle.second << '\n';

    std::cout << "Multi-pulti-threaded result:\n";
    std::cout << "Average age: " << static_cast<uint16_t>(resultMulti.first) << ", max wage: " << resultMulti.second << '\n';

    // company.get(50);
}

#endif