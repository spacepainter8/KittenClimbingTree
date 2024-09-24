//
// Created by os on 8/7/22.
//

#include "../h/MemoryAllocator.hpp"

BlockHeader* MemoryAllocator::freeMemHead = (BlockHeader*) HEAP_START_ADDR;
bool MemoryAllocator::flag = false;

void MemoryAllocator::CreateInstance() {

    if (!flag){
        freeMemHead = (BlockHeader*) HEAP_START_ADDR;
        freeMemHead->next=nullptr;
        freeMemHead->size=(size_t)HEAP_END_ADDR-(size_t)HEAP_START_ADDR-sizeof(BlockHeader);
        //freeMemHead->size=(((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - sizeof(BlockHeader))/MEM_BLOCK_SIZE - 1)*MEM_BLOCK_SIZE;
       flag=true;
    }}


void* MemoryAllocator::mem_alloc (size_t size){
    if (size<=0) return nullptr;


    // size is in blocks
    size_t realSize = size * MEM_BLOCK_SIZE;

    // find free block
    BlockHeader* blk = MemoryAllocator::freeMemHead, *prev = nullptr;
    for(;blk!=nullptr;prev=blk, blk=blk->next){
        if (blk->size>=realSize) break;
    }
    if (blk==nullptr) return nullptr;

    // allocate
    size_t remSize = blk->size - realSize;
    if (remSize>= MEM_BLOCK_SIZE + sizeof(BlockHeader)){
        // allocate realSize
        blk->size=realSize;
        size_t vel = sizeof(BlockHeader);
        BlockHeader* newBlock = (BlockHeader*)((char*)blk + realSize + vel);
        if (prev) prev->next=newBlock;
        else MemoryAllocator::freeMemHead = newBlock;
        newBlock->next=blk->next;
        newBlock->size = remSize - sizeof(BlockHeader);
    }
    else {
        // allocate the whole block
        if (prev) prev->next=blk->next;
        else MemoryAllocator::freeMemHead = blk->next;
    }

    blk->next=nullptr;
    return (char*)blk+sizeof(BlockHeader);

}
int MemoryAllocator::mem_free (void* addr){
    if (!addr) return -1;
    if ((char*)addr<(char*)HEAP_START_ADDR || (char*)addr>=(char*)HEAP_END_ADDR) return -1;


    BlockHeader* curr = nullptr;
    // find the position in the list for the newly freed memory
    if (!(MemoryAllocator::freeMemHead) || (char*)addr<(char*)(MemoryAllocator::freeMemHead)){
        curr = nullptr;
    }
    else{
        for(curr=MemoryAllocator::freeMemHead; curr->next!=0 && (char*)addr>(char*)(curr->next);curr=curr->next);
    }

    BlockHeader* newFree = (BlockHeader*)((char*)addr-sizeof(BlockHeader));
    if (curr) newFree->next=curr->next;
    else newFree->next=MemoryAllocator::freeMemHead;
    if (curr) curr->next = newFree;
    else MemoryAllocator::freeMemHead=newFree;

    join(newFree);
    join(curr);

    return 0;
}

int MemoryAllocator::join (BlockHeader* curr){
    if (!curr) return 0;
    if (curr->next && (char*)curr+curr->size+sizeof(BlockHeader)==(char*)(curr->next)){
        curr->size += curr->next->size+sizeof(BlockHeader);
        curr->next = curr->next->next;
        return 1;
    }
    else return 0;
}
