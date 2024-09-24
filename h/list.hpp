//
// Created by os on 8/25/22.
//

#ifndef PROJECT_BASE_LIST_H
#define PROJECT_BASE_LIST_H

#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"

template<typename T>
class List
{
private:
    struct Elem {
        T *data;
        Elem *next;

        Elem (T *data, Elem *next): data(data), next(next){}
        void* operator new(size_t size){
            size_t realSz = size;
            realSz/=MEM_BLOCK_SIZE;
            realSz+=(size%MEM_BLOCK_SIZE==0)?0:1;
            return MemoryAllocator::mem_alloc(realSz);
        }
        void operator delete(void* addr){
            MemoryAllocator::mem_free(addr);
        }

    };

    Elem *head, *tail;
public:
    List() : head(0), tail(0){}
    //List(const List<T> &) = delete;
    //List<T> &operator=(const List<T> &) = delete;

    void addLast (T *data){
        Elem *elem = new Elem(data, 0);
        if (tail){
            tail->next=elem;
            tail=elem;
        } else{
            head=tail=elem;
        }
    }
    T* removeFirst(){
        if (!head) return 0;

        Elem *temp = head;
        head = head->next;
        if (!head) tail=0;

        T *ret = temp->data;
        delete temp;
        return ret;
    }
    void* operator new(size_t size){
        size_t realSz = size;
        realSz/=MEM_BLOCK_SIZE;
        realSz+=(size%MEM_BLOCK_SIZE==0)?0:1;
        return MemoryAllocator::mem_alloc(realSz);
    }
    void operator delete(void* addr){
        MemoryAllocator::mem_free(addr);
    }
};


#endif //PROJECT_BASE_LIST_H
