//
// Created by os on 8/28/22.
//

#ifndef PROJECT_BASE_TCB_HPP
#define PROJECT_BASE_TCB_HPP

#include "../lib/hw.h"
#include "Scheduler.hpp"

static int glID = 0;


class TCB {
public:
    ~TCB() { delete[] stack; }

    bool isFinished() const {
        return finished; }

    void setFinished(bool v) { finished = v; }

    bool isStarted() const {
        return started; }

    void setStarted(bool v) { started = v; }

    bool retExit() const {
        return exit; }

    void setExit(bool v) { exit = v; }

    bool isBlocked() const {
        return blocked; }

    void setBlocked(bool v) { blocked = v; }

    bool isIdle() const {
        return idle; }

    void setIdle(bool v) { idle = v; }

    using Body = void (*)(void*);

    static TCB *createThread(Body body, void* arg, uint64* stack);

    static void yield();

    static TCB *running;
    static TCB *idleTCB;

    int getID () const {return id;}

    uint64 getTimeSlice() const { return timeSlice; }

    void* operator new (size_t);
    void operator delete(void*);


private:
    TCB(Body body, void* arg, uint64* stack) :
            body(body),
            arg(arg),
            stack(body ? stack: nullptr),
            context({(uint64) &threadWrapper,
                     stack ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
                    }),
            finished(false),
            started(false),
            exit(false),
            blocked(false),
            idle(false),
            timeSlice(DEFAULT_TIME_SLICE){
        // if (body) Scheduler::put(this);
        id = glID++;

        // ovo je potencijalno rizicno, porazmisliti
        //if (id==0) running = this;
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    uint64* stack;
    Context context;
    bool finished;
    bool started;
    bool exit;
    bool blocked;
    bool idle;
    int id;

    uint64 timeSlice;
    static uint64 timeSliceCounter;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch (Context *oldContext, Context* runningContext);
    static void contextSwitchExit (Context *oldContext, Context* runningContext);

    static void dispatch();


};


#endif //PROJECT_BASE_TCB_HPP
