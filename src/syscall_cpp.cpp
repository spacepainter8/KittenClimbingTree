//
// Created by os on 8/28/22.
//

#include "../h/syscall_cpp.h"
#include "../h/TCB.hpp"

// zasto ovde ne radi kad includujem syscall_c.h nego moram ovako da im postavim deklaracije ???
/*
extern "C" void* mem_alloc(size_t s);
extern "C" void mem_free(void* p);
extern "C" int thread_create (thread_t* handle, void (*start_routine) (void*), void* arg);
extern "C" int thread_exit();
extern "C" void thread_dispatch();
extern "C" void thread_tcb (thread_t* handle, void(*start_routine)(void*), void* arg);
extern "C" int thread_start(TCB* tcb);
*/
void *operator new(size_t n){
    return mem_alloc(n);
}

void *operator new[] (size_t n){
    return mem_alloc(n);
}

void operator delete(void *p) noexcept{
    mem_free(p);
}

void operator delete[] (void *p) noexcept{
    mem_free(p);
}

Thread::Thread(void (*body)(void *), void *arg) {
    myHandle = nullptr;
    //thread_create(&myHandle, body, arg);
    thread_tcb(&myHandle, body, arg);
}

Thread::~Thread() {
    //thread_exit();
    myHandle->setFinished(true);
    myHandle->setExit(true);
    // ovde treba izvaditi ovu nit iz schedulera vrv i iz liste blokiranih niti ako je tu
    //delete myHandle;
}

int Thread::start() {
    if (myHandle->isStarted()) return -2;
    return thread_start(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread() {
    myHandle = nullptr;
    thread_tcb(&myHandle, &Thread::wrapper, this);
}

void Thread::wrapper(void *arg) {
    Thread* th = (Thread*) arg;
    th->run();
}




Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    // da li je ovo ok?
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}







char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}
