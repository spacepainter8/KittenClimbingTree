//
// Created by os on 9/5/22.
//

#include "../h/SCB.hpp"
#include "../h/TCB.hpp"
uint64 lock = 0;

extern "C" uint64 copy_and_swap1(uint64 &lock, uint64 expected, uint64 desired);

#define LOCK() while(copy_and_swap1(lock, 0, 1))
#define UNLOCK() while(copy_and_swap1(lock, 1, 0))



SCB::SCB(unsigned int init) {
    //LOCK();
   // if (closed) return;
    val=init;
    blocked = new BlockedList();
    closed = false;
    //UNLOCK();
}

int SCB::wait() {
    //LOCK();
    if (closed) return -1;
    int n = 0;
    if (--val<0) n=block();
    if (n<0) return n;
    return 0;
    //UNLOCK();
}

int SCB::signal() {
    //LOCK();
    if (closed) return -1;
    if (++val<=0) unblock();
    return 0;
    //UNLOCK();
}

int SCB::block() {
    // staviti running u listu blokiranih,
    // setovati blocked flag
    // uraditi yield
    // u dispatchu treba pripaziti ako je blocked
    // bez obzira da l
    blocked->addLast(TCB::running);
    TCB::running->setBlocked(true);
    TCB::yield();
    // nisam sigurna da li ovo radi dobro
    if (closed) return -1;
    return 0;
}

void SCB::unblock() {
    TCB* tcb = blocked->removeFirst();
    // if (tcb)
    if (tcb) {
        tcb->setBlocked(false);
        // ovde treba ona ista provera ko iz dispatch-a()
        if (tcb->isFinished()) {
            delete tcb;
            return;
        }
        Scheduler::put(tcb);

    }
}

SCB::~SCB() {
      //  LOCK();
        delete blocked;
        closed = true;
      //  UNLOCK();
}
void* SCB::operator new(size_t size){
    size_t realSz = size;
    realSz/=MEM_BLOCK_SIZE;
    realSz+=(size%MEM_BLOCK_SIZE==0)?0:1;
    return MemoryAllocator::mem_alloc(realSz);
}
void SCB::operator delete(void* addr){
    MemoryAllocator::mem_free(addr);
}

SCB *SCB::createSem(unsigned int init) {
    return new SCB(init);
}

void SCB::unblockAll() {
    // treba dohvatati elemente iz liste
    // sve dok se ne dohvati nullptr
    // ubacivati ih u scheduler
    // staviti blocked na false
    // kada se dohvati nullptr prosli smo
    // sve blokirane elemente

    if (closed) return;
    TCB* temp = blocked->removeFirst();
    while(temp){
        temp->setBlocked(false);
        if (temp->isFinished()) {
            delete temp;
            temp = blocked->removeFirst();
            continue;
        }
        if (!temp->isFinished() && !temp->retExit()) Scheduler::put(temp);
        temp = blocked->removeFirst();
    }

}
