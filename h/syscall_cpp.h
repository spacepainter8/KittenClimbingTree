//
// Created by os on 8/28/22.
//

#ifndef PROJECT_BASE_SYSCALL_CPP_H
#define PROJECT_BASE_SYSCALL_CPP_H

#include "../lib/hw.h"
#include "../h/syscall_c.h"

void* operator new (size_t);
void* operator new[](size_t);
void operator delete (void*) noexcept;
void operator delete[](void*) noexcept;

class TCB;
typedef TCB* thread_t;

class Thread {
public:
    Thread (void (*body) (void*), void* arg);
    virtual ~Thread();

    int start();

    static void dispatch();
    static void wrapper (void* arg);


protected:
    Thread();
    virtual void run (){};

private:
    thread_t myHandle;
};



class SCB;
typedef SCB* sem_t;

class Semaphore {
public:

    Semaphore (unsigned init = 1);
    virtual ~Semaphore();

    int wait();
    int signal();



private:
    sem_t myHandle;
};


class Console {
public:
    // Console treba da bude singleton
    static char getc ();
    static void putc (char);
};



#endif //PROJECT_BASE_SYSCALL_CPP_H
