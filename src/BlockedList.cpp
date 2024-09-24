//
// Created by os on 9/8/22.
//

#include "../h/BlockedList.hpp"

void *BlockedList::Elem::operator new(size_t size) {

        size_t realSz = size;
        realSz/=MEM_BLOCK_SIZE;
        realSz+=(size%MEM_BLOCK_SIZE==0)?0:1;
        return MemoryAllocator::mem_alloc(realSz);

}

void BlockedList::Elem::operator delete(void *addr) {

        MemoryAllocator::mem_free(addr);

}

void BlockedList::addLast(TCB *data) {

        Elem *elem = (Elem*) new Elem(data, 0);
        if (tail){
            tail->next=elem;
            tail=elem;
        } else{
            head=tail=elem;
        }

}

TCB *BlockedList::removeFirst() {
    if (!head) return 0;

    Elem *temp = head;
    head = head->next;
    if (!head) tail=0;

    TCB *ret = temp->data;
    delete temp;
    return ret;
}

void *BlockedList::operator new(size_t size) {
    size_t realSz = size;
    realSz/=MEM_BLOCK_SIZE;
    realSz+=(size%MEM_BLOCK_SIZE==0)?0:1;
    return MemoryAllocator::mem_alloc(realSz);
}

void BlockedList::operator delete(void *addr) {
    MemoryAllocator::mem_free(addr);
}

/*
BlockedList::~BlockedList() {
    Elem* temp = head;
    while (head){
        head=head->next;
        delete temp;
        temp=head;
    }
}
*/