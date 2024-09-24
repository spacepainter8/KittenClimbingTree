//
// Created by os on 8/7/22.
//

#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/TCB.hpp"
#include "../lib/console.h"
#include "../h/SCB.hpp"

// previous privilege, previous interrupt enable
void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap() {
    // NE KORISTITI GOTOVE FUNKCIJE ZBOG REGISTARA, MOGU BITI PREPISANI
    // MOZDA JE GENERALNO BOLJE SKIDATI REGISTRE SA STEKA NEGO DIREKTNO UZIMATI VREDNOSTI IZ NJIH
    //uint64 scause = r_scause();
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {

        uint64 code = 0;
        __asm__ volatile("mv %0, a0" : "=r" (code));

        switch(code){
            case 0x01: {
                //uint64 volatile sstatus = r_sstatus();
               // uint64 volatile sstatus;
               // __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
                void* arg1Temp = 0;
                __asm__ volatile("mv %0, a1" : "=r" (arg1Temp));
                size_t* arg1 = (size_t*) arg1Temp;
                void* allocated = MemoryAllocator::mem_alloc(*arg1);
                __asm__ volatile ("sd %0, 80(s0)" : : "r" (allocated));

                uint64 temp = 0;
                __asm__ volatile("csrr %0, sepc" : "=r" (temp));
                temp+=4;
                __asm__ volatile ("csrw sepc, %0" : : "r" (temp));
                //w_sstatus(sstatus);
                break;

            }
            case 0x02: {
                void* arg1 = 0;
                __asm__ volatile ("mv %0, a1" : "=r" (arg1));
                int retVal = MemoryAllocator::mem_free(arg1);
                __asm__ volatile ("sd %0, 80(s0)" : : "r" (retVal));
                uint64 volatile sepc = r_sepc() + 4;
                w_sepc(sepc);
                break;
            }
            case 0x11:{
                // thread_create()
                typedef void(*body)(void*) ;

                void* arg1 = 0, *arg2 = 0, *arg = 0, *arg4 = 0;
                __asm__ volatile ("mv %0, a1" : "=r" (arg1));
                __asm__ volatile ("mv %0, a2" : "=r" (arg2));
                __asm__ volatile ("mv %0, a3" : "=r" (arg));
                __asm__ volatile ("ld %0, 112(s0)" : "=r" (arg4));


                body start_routine = (body) arg2;

                uint64* stack = (uint64*) arg4;
                TCB* handle = TCB::createThread(start_routine, arg, stack);
                if (start_routine) {
                    Scheduler::put(handle);

                }
                // samo test privremeni
                // obrisati liniju ispod posle
                //handle->setFinished(true);

                handle->setStarted(true);


                //__asm__ volatile ("mv a1, %0" : : "r" (arg1));
                //__asm__ volatile ("sd %0, 0(a1)" : : "r" (&handle));

                int ret = 0;

                // ovaj ret se mozdad lose radi jer ovo lose kompajlira
                if (handle!=0){
                    __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));
                }
                // fali else grana

                else{
                    ret = -1;
                    __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));

                }
                __asm__ volatile ("sd %0, 88(s0)" : : "r" (handle));


                uint64 volatile sepc = r_sepc() + 4;
                w_sepc(sepc);

                //  w_sstatus(sstatus);
                break;


            }
            case 0x12:{
                // thread_exit()
                if (!TCB::running->isStarted() || TCB::running->retExit()) {
                    int ret = -1;
                    __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));
                    break;
                }
                TCB::running->setFinished(true);
                TCB::running->setExit(true);

               // mozda u dispatchu treba pozvati delete za old ukoliko je finished = true;
               // treba obrisati stek i dealocirati TCB

                TCB::dispatch();

                break;
            }
            case 0x13:{
                // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
                // thread_dispatch();
                uint64 volatile sepc = r_sepc() + 4;
                uint64 volatile sstatus = r_sstatus();
                TCB::dispatch();
                w_sstatus(sstatus);
                w_sepc(sepc);
                break;
            }
            case 0x14:{
                // thread_tcb()
               /* typedef void(*body)(void*) ;

                void* arg1 = 0, *arg2 = 0, *arg = 0, *arg4 = 0;
                __asm__ volatile ("mv %0, a1" : "=r" (arg1));
                __asm__ volatile ("mv %0, a2" : "=r" (arg2));
                __asm__ volatile ("mv %0, a3" : "=r" (arg));
                __asm__ volatile ("ld %0, 112(s0)" : "=r" (arg4));


                body start_routine = (body) arg2;

                uint64* stack = (uint64*) arg4;
                TCB* handle = TCB::createThread(start_routine, arg, stack);

                TCB** tcb = (TCB**) arg1;
                *tcb=handle;
               // prethodno je u a1 arg1 sacuvano iz a1,
                __asm__ volatile ("mv a1, %0" : : "r" (arg1));
                __asm__ volatile ("sd %0, 0(a1)" : : "r" (&handle));
               // __asm__ volatile ("sd %0, 0(a1)" : : "r" (&handle));

                int ret = 0;

                if (handle){
                    __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));
                }

                ret = -1;
                __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));

                uint64 volatile sepc = r_sepc() + 4;
                w_sepc(sepc);


                break;*/
                typedef void(*body)(void*) ;

                void* arg1 = 0, *arg2 = 0, *arg = 0, *arg4 = 0;
                __asm__ volatile ("mv %0, a1" : "=r" (arg1));
                __asm__ volatile ("mv %0, a2" : "=r" (arg2));
                __asm__ volatile ("mv %0, a3" : "=r" (arg));
                __asm__ volatile ("ld %0, 112(s0)" : "=r" (arg4));


                body start_routine = (body) arg2;

                uint64* stack = (uint64*) arg4;
                TCB* handle = TCB::createThread(start_routine, arg, stack);

                TCB** tcb = (TCB**) arg1;
                *tcb=handle;
                // prethodno je u a1 arg1 sacuvano iz a1,
                //__asm__ volatile ("mv a1, %0" : : "r" (arg1));
                //__asm__ volatile ("sd %0, 0(a1)" : : "r" (&handle));
                // __asm__ volatile ("sd %0, 0(a1)" : : "r" (&handle));

                int ret = 0;

                if (handle){
                    __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));
                }
                else {
                    ret = -1;
                    __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));
                }

                __asm__ volatile ("sd %0, 88(s0)" : : "r" (handle));

                uint64 volatile sepc = r_sepc() + 4;
                w_sepc(sepc);


                break;
            }
            case 0x15:{
                // thread_start()
                uint64 temp1 = 0;
                __asm__ volatile("csrr %0, sepc" : "=r" (temp1));
                temp1+=4;
                __asm__ volatile ("csrw sepc, %0" : : "r" (temp1));
                int ret = 0;
                void* temp = nullptr;
                __asm__ volatile ("ld %0, 88(s0)" : "=r" (temp));
                TCB* tcb = (TCB*) temp;
                if (tcb!=nullptr && !tcb->isStarted()){
                    Scheduler::put(tcb);
                    tcb->setStarted(true);
                }
                else ret = -1;
                __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));


                break;
            }
            case 0x21:{
                // sem_open()
                void* arg1 = 0, *arg2 = 0;
                __asm__ volatile ("mv %0, a1" : "=r" (arg1));
                __asm__ volatile ("mv %0, a2" : "=r" (arg2));
                unsigned* init = (unsigned*) arg2;
/*
                SCB* handle = SCB::createSem(*init);
                // prethodno je u a1 arg1 sacuvano iz a1,
                //__asm__ volatile ("mv a1, %0" : : "r" (arg1));
               // __asm__ volatile ("sd %0, 0(a1)" : : "r" (&handle));
               // __asm__ volatile ("sd %0, 0(a1)" : : "r" (&handle));
                SCB** temp = (SCB**) arg1;
                *temp = handle;
*/

               SCB* handle = SCB::createSem(*init);



                //__asm__ volatile ("mv a1, %0" : : "r" (arg1));
                //__asm__ volatile ("sd %0, 0(a1)" : : "r" (&handle));

                __asm__ volatile ("sd %0, 88(s0)" : : "r" (handle));
                int ret = 0;

                // ovaj ret se mozdad lose radi jer ovo lose kompajlira
                if (handle!=0){
                    __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));
                }
                else{
                    ret = -1;
                    __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));

                }


                uint64 volatile sepc = r_sepc() + 4;
                w_sepc(sepc);


                break;
            }
            case 0x22:{
                // sem_close()
                // dealocirati i postaviti flag ig
                // deblokirati sve niti pre brisanja

                // treba proci kroz celu listu TCBova
                // i ubaciti svaki u Scheduler
                // i setovati blocked na false


                void* arg1 = 0;
                __asm__ volatile ("mv %0, a1" : "=r" (arg1));
                SCB* handle = (SCB*) arg1;

                int ret = 0;
                if (handle->isClosed()) {
                    ret = -1;
                    __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));
                    break;
                }

                handle->unblockAll();
                handle->setClosed(true);
                delete handle;

                __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));


                uint64 volatile sepc = r_sepc() + 4;
                w_sepc(sepc);


                break;
            }
            case 0x23:{
                // sem_wait(sem_t id)

                void* arg1 = 0;
                __asm__ volatile ("mv %0, a1" : "=r" (arg1));
                SCB* handle = (SCB*) arg1;

                int ret = handle->wait();

                __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));

                uint64 volatile sepc = r_sepc() + 4;
                w_sepc(sepc);


                break;
            }
            case 0x24:{
                // sem_signal(sem_t id)

                void* arg1 = 0;
                __asm__ volatile ("mv %0, a1" : "=r" (arg1));
                SCB* handle = (SCB*) arg1;

                int ret = handle->signal();

                __asm__ volatile ("sd %0, 80(s0)" : : "r" (ret));

                uint64 volatile sepc = r_sepc() + 4;
                w_sepc(sepc);


                break;
            }

            case 0x41:{
                // getc();
               uint64 volatile sstatus = r_sstatus();
               uint64 volatile sepc = r_sepc();
                uint64 volatile sip = r_sip();
                char x = __getc();
               w_sstatus(sstatus);
                w_sepc(sepc);
                w_sip(sip);
               // u sstatus bit 1 upisati 0
               // mc_sstatus(1<<1);
               //  mc_sip(SIP_SSIP);
                // je l treba da moze da mi dolaze ovde prekidi lol
                // proveriti prekide pre i posle ovoga
                uint64  temp1 = 0;
                __asm__ volatile("csrr %0, sepc" : "=r" (temp1));
                temp1+=4;
                __asm__ volatile ("csrw sepc, %0" : : "r" (temp1));
                __asm__ volatile ("sd %0, 80(s0)" : : "r" (x));
                //w_sstatus(sstatus);
               // mc_sip(SIP_SSIP);
                break;

            }
            case 0x42:{
                // putc(char c)
                void* temp = nullptr;
                __asm__ volatile ("ld %0, 88(s0)" : "=r" (temp));
                char* x = (char*) temp;
                char c = *x;

                // sstatus mozda ne treba

                uint64 volatile sstatus = r_sstatus();
                uint64 volatile sepc = r_sepc();
                uint64 volatile sip = r_sip();
                __putc(c);
                w_sstatus(sstatus);
                w_sepc(sepc);
                w_sip(sip);
               // mc_sip(SIP_SSIP);

                uint64  temp1 = 0;
                __asm__ volatile("csrr %0, sepc" : "=r" (temp1));
                temp1+=4;
                __asm__ volatile ("csrw sepc, %0" : : "r" (temp1));

                break;

            }





        }



    }
    else if (scause == 0x8000000000000001UL){
        // timer interrupt; asynchronous context change
        mc_sip(SIP_SSIP);
        /*
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }*/

    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        //uint64 volatile sstatus = r_sstatus();
        uint64 volatile sstatus = r_sstatus();
        uint64 volatile sepc = r_sepc();
        uint64 volatile sip = r_sip();
        console_handler();
        w_sstatus(sstatus);
        w_sepc(sepc);
        w_sip(sip);
        //w_sstatus(sstatus);
    }
    else {
        // unexpected scause

    }
}






//}