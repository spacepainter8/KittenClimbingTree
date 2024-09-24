//
// Created by os on 8/30/22.
//

#ifndef PROJECT_BASE_OLD_SYSCALL_C_H
#define PROJECT_BASE_OLD_SYSCALL_C_H

#ifdef __cplusplus
extern "C" {
#endif



#include "../lib/hw.h"
//#include "../h/TCB.hpp"

typedef struct TCB TCB;
typedef TCB* thread_t;
typedef struct SCB SCB;
typedef SCB* sem_t;





void* mem_alloc (size_t size);
int mem_free (void*);


int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit (); // setovanje finished i dispatch??
void thread_dispatch ();
void thread_tcb (thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_start (TCB* tcb);

int sem_open (sem_t* handle, unsigned init);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);




const int EOF = -1;
char getc();
void putc(char);

#ifdef __cplusplus
}
#endif



#endif //PROJECT_BASE_OLD_SYSCALL_C_H
