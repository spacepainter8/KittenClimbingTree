//
// Created by os on 9/5/22.
//

#ifndef PROJECT_BASE_OLD_SCB_HPP
#define PROJECT_BASE_OLD_SCB_HPP

#include "../h/BlockedList.hpp"
#include "../lib/hw.h"
class TCB;

class SCB {
public:
    static SCB *createSem(unsigned init);

    ~SCB();


    int wait();
    int signal();
    void unblockAll();


    int value () const {return val;}
    void* operator new(size_t);
    void operator delete(void*);

    void setClosed (bool val){
        closed=val;
    }
    bool isClosed(){
        return closed;
    }

private:
    SCB (unsigned init);
    int val;
    BlockedList* blocked;
    int block();
    void unblock();
    bool closed;
};



#endif //PROJECT_BASE_OLD_SCB_HPP
