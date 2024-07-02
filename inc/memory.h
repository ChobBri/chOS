#pragma once

#include <types.h>
#include <peripherals/base.h>
#include <status.h>

extern void restore(int32);
extern int32 disable(void);

#define MEM_BLOCK_SIZE 16 /* bytes */

uint32 mem_ceilMemBlock(uint32);
uint32 mem_floorMemBlock(uint32);

typedef struct memblk {
    struct memblk *next;
    uint32 length;
} memblk;

void *mem_allocMem(uint32 nbytes);
void *mem_allocStack(uint32 nbytes);
status mem_freeMem(void *blkaddr, uint32 nbytes);
status mem_freeStack(void *blkaddr, uint32 nbytes);

memblk *mem_getFreeList();

void *mem_getMemMin();
void *mem_getMemMax();

void mem_init();
