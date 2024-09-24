//
// Created by os on 9/8/22.
//

#ifndef PROJECT_BASE_OLD_BLOCKEDLIST_HPP
#define PROJECT_BASE_OLD_BLOCKEDLIST_HPP
class TCB;
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"

class BlockedList {
    struct Elem {
        TCB *data;
        Elem *next;

        Elem (TCB *data, Elem *next): data(data), next(next){}
        void* operator new(size_t size);
        void operator delete(void* addr);

    };

    Elem *head, *tail;
public:
    BlockedList() : head(0), tail(0){}
    // problem sa ovim destruktorom mozda samo koristiti razlicite liste
    //~BlockedList();
    //List(const List<T> &) = delete;
    //List<T> &operator=(const List<T> &) = delete;

    void addLast (TCB *data);
    TCB* removeFirst();
    void* operator new(size_t size);
    void operator delete(void* addr);
};


#endif //PROJECT_BASE_OLD_BLOCKEDLIST_HPP
