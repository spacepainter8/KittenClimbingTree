
#include "../h/syscall_c.h"
//#include "../h/TCB.hpp"



void transfer (uint64 code, void* arg1, void* arg2, void* arg3, void* arg4);

void* mem_alloc (size_t size){

    size_t realSz = size;
    realSz/=MEM_BLOCK_SIZE;
    realSz+=(size%MEM_BLOCK_SIZE==0)?0:1;
    transfer(0x01, &realSz, 0, 0, 0);
    void* allocated = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (allocated));
    return allocated;
}

int mem_free (void* addr){
    transfer (0x02, addr, 0, 0, 0);
    int retVal = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;

}

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg){
    uint64* stack_space = 0;
    // da li je ovo pravi nacin alociranja niza
    // da li je u redu da ovde imam mem_alloc ili treba da se poziva iz MemoryAllocator
    if (start_routine) stack_space = (uint64*)mem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));
    transfer(0x11, handle, start_routine, arg, stack_space);
    int retVal = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
   //
    void* h = 0;
    __asm__ volatile ("mv %0, a1" : "=r" (h));
    thread_t tcb = (thread_t) h;
    *handle=tcb;
   //
    return retVal;

};

int thread_exit (){

    transfer(0x12, 0, 0, 0, 0);
    // nisam sigurna sta bi mogla da bude povratna vrednost
    // nisam sigurna kako bih je vratila nazad tj.
    int retVal = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));

    return retVal;
};

void thread_dispatch (){
    transfer(0x13, 0, 0, 0, 0);
};

void thread_tcb (thread_t* handle, void(*start_routine)(void*), void* arg){
    uint64* stack_space = 0;
    // da li je ovo pravi nacin alociranja niza
    if (start_routine) stack_space = (uint64*)mem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));
    transfer(0x14, handle, start_routine, arg, stack_space);
    void* h = 0;
    __asm__ volatile ("mv %0, a1" : "=r" (h));
    thread_t tcb = (thread_t) h;
    *handle=tcb;
}

int thread_start (TCB* tcb){
    transfer (0x15, tcb, 0, 0, 0);
    int retVal = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

int sem_open (sem_t* handle, unsigned init){
    // nesto lose
  //  sem_t test = *handle;
    transfer(0x21, handle, &init, 0,0);
    int retVal = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    void* h = 0;
    __asm__ volatile ("mv %0, a1" : "=r" (h));
    sem_t scb = (sem_t) h;
    *handle=scb;
    return retVal;

}

int sem_close (sem_t handle){
    transfer (0x22, handle, 0, 0, 0);
    int retVal = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

int sem_wait (sem_t id){
    transfer (0x23, id, 0, 0, 0);
    int retVal = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

int sem_signal (sem_t id){
    transfer (0x24, id, 0, 0, 0);
    int retVal = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}



char getc(){
    transfer(0x41, 0, 0, 0, 0);
    char retVal = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (retVal));
    return retVal;
}

void putc(char c){
    transfer(0x42, &c, 0, 0, 0);
}

