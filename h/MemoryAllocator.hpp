//
// Created by os on 8/7/22.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct BlockHeader{
    BlockHeader* next;
    size_t size;
  //  BlockHeader(BlockHeader* n, size_t s):next(n),size(s){};
};

class MemoryAllocator{
public:


    static BlockHeader* freeMemHead;
    static void CreateInstance();


    static void* mem_alloc (size_t size);
    static int mem_free (void* addr);
    static int join (BlockHeader* curr);

private:
    MemoryAllocator();
    static bool flag;

};




#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
