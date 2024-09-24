//
// Created by os on 8/28/22.
//

#include "../h/TCB.hpp"
#include "../h/riscv.hpp"
#include "../h/printing.hpp"

TCB *TCB::running = nullptr;
TCB *TCB::idleTCB = nullptr;
uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(TCB::Body body, void* arg, uint64* stack)
{
    // da li ovde treba preklopiti new ili se koristi globalni new
    return new TCB(body, arg, stack );
}

void TCB::yield() {
    uint64 code = 0x13;

    __asm__ volatile ("mv a0, %0" : : "r" (code));
    __asm__ volatile ("ecall");
}

void TCB::dispatch() {
    TCB *old = running;
    if (!old->isFinished() && !old->retExit() && !old->isBlocked() && !old->isIdle()) Scheduler::put(old);
    running = Scheduler::get();
    // privremeno resenje bez IDLE niti, za brisanje nakon implementacije IDLE niti
    //if (!running) running=old;
    // sve dok je running
    if (!running) running = TCB::idleTCB;
    while (running->isFinished() || running->retExit() || running->isBlocked()) {
        if (running->isFinished()) delete running;
        running = Scheduler::get();
    }
    if (old->retExit() || old->isFinished()){
        // da li ovde treba pripaziti da je izvuce sa nekog semafora
        delete old;
        TCB::contextSwitchExit(&running->context, &running->context);
    }
    else TCB::contextSwitch(&old->context, &running->context);
}


void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}
void* TCB::operator new(size_t size){
    size_t realSz = size;
    realSz/=MEM_BLOCK_SIZE;
    realSz+=(size%MEM_BLOCK_SIZE==0)?0:1;
    return MemoryAllocator::mem_alloc(realSz);
}
void TCB::operator delete(void* addr){
    MemoryAllocator::mem_free(addr);
}

