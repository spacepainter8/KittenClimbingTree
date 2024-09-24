//
// Created by os on 8/25/22.
//


#include "../h/Scheduler.hpp"

class TCB;


BlockedList Scheduler::readyThreads;

TCB* Scheduler::get() {
    return readyThreads.removeFirst();
}

void Scheduler::put(TCB *th){
    readyThreads.addLast(th);
}

