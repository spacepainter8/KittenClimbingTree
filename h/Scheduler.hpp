//
// Created by os on 8/25/22.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP


//#include "list.hpp"
#include "../h/BlockedList.hpp"

class TCB;

class Scheduler {
private:
    static BlockedList readyThreads;
    //static Scheduler *instance;
    //Scheduler(){};

public:
    static TCB *get();
    static void put (TCB *th);
    //static Scheduler* getInstance();
    //void* operator new (size_t n);
    //void operator delete (void* p);
    //Scheduler(Scheduler &s) = delete;
    //void operator=(const Scheduler&) = delete;

};


#endif //PROJECT_BASE_SCHEDULER_HPP
