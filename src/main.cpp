#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.h"
#include "../h/TCB.hpp"


extern void userMain();

void idle (void* arg){
    while(1){
        thread_dispatch();
    }
}


int main(){
    // priprema kernela
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    MemoryAllocator::CreateInstance();
    thread_t th;
    // stvara "main" nit
    thread_create(&th, nullptr, nullptr);
    TCB::running = th;
    thread_t idleth;
    thread_tcb(&idleth, idle, nullptr);
    TCB::idleTCB=idleth;
    idleth->setIdle(true);


    userMain();

    delete th;
    delete idleth;

    return 0;
}
