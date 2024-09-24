#include "../lib/hw.h"


extern "C" void transfer (uint64 code, void* arg1, void* arg2, void* arg3, void* arg4){

    __asm__ volatile ("ecall");

    return;

}