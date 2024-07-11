#include <memory.h>
#include <interrupts.h>

/* .length keeps track of number of free bytes */
memblk m_memFreeList;

uint32 mem_ceilMemBlock(uint32 bytes) {
    return ((MEM_BLOCK_SIZE - 1) + bytes) & ~(MEM_BLOCK_SIZE - 1);
}

uint32 mem_floorMemBlock(uint32 bytes) {
    return (bytes) & ~(MEM_BLOCK_SIZE - 1);
}

void *mem_allocMem(uint32 nbytes) {
    imask intrMask;
    intrMask = intr_disable();
    if (nbytes == 0) {
        intr_restore(intrMask);
        return nullptr;
    }

    nbytes = mem_ceilMemBlock(nbytes);

    memblk* prev = &m_memFreeList;
    memblk* curr = m_memFreeList.next;

    while (curr != nullptr) {
        if (curr->length == nbytes) {
            prev->next = curr->next;
            m_memFreeList.length -= nbytes;
            intr_restore(intrMask);
            return (void *) curr;
        } else if (curr->length > nbytes) {
            memblk *newMemBlock = (memblk *)((uint64) curr + nbytes);
            prev->next = newMemBlock;
            newMemBlock->next = curr->next;
            newMemBlock->length = curr->length - nbytes;
            m_memFreeList.length -= nbytes;
            intr_restore(intrMask);
            return (void *)curr;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }

    intr_restore(intrMask);
    return nullptr;
}

void *mem_allocStack(uint32 nbytes) {
    imask intrMask;
    intrMask = intr_disable();
    if (nbytes == 0) {
        intr_restore(intrMask);
        return nullptr;
    }

    nbytes = mem_ceilMemBlock(nbytes);

    memblk* prev = &m_memFreeList;
    memblk* curr = m_memFreeList.next;

    memblk* stkprev = nullptr;
    memblk* stkcurr = nullptr;

    while (curr != nullptr) {
        if (curr->length >= nbytes) {
            stkprev = prev;
            stkcurr = curr;
        }
        prev = curr;
        curr = curr->next;
    }

    if (stkcurr == nullptr) {
        intr_restore(intrMask);
        return nullptr;
    }

    if (stkcurr->length == nbytes) {
        stkprev->next = stkcurr->next;
    } else if (stkcurr->length > nbytes) {
        stkcurr->length -= nbytes;
        stkcurr = (memblk *)((uint64) stkcurr + stkcurr->length);
    }

    stkcurr = (memblk *)((uint64) stkcurr + nbytes - sizeof(uint64));  // Stack grows downwards

    m_memFreeList.length -= nbytes;
    intr_restore(intrMask);
    return stkcurr;
}

status mem_freeMem(void *blkaddr, uint32 nbytes) {
    int64 intrMask;
    intrMask = intr_disable();

    if (nbytes == 0 || blkaddr < mem_getMemMin() || blkaddr >= mem_getMemMax()) {
        intr_restore(intrMask);
        return SYSERR;
    }

    nbytes = mem_ceilMemBlock(nbytes);

    memblk *newblk = (memblk *) blkaddr;
    memblk *prev = &m_memFreeList;
    memblk *curr = m_memFreeList.next;

    while (curr != nullptr && curr < newblk) {
        prev = curr;
        curr = curr->next;
    }

    memblk *prevEnd = (memblk *)((uint64) prev + prev->length);
    memblk *newblkEnd = (memblk *)((uint64) newblk + nbytes);

    if ((prev != &m_memFreeList && prevEnd > newblk) || (curr != nullptr && newblkEnd > curr)) {
        intr_restore(intrMask);
        return SYSERR;
    }

    m_memFreeList.length += nbytes;

    prev->next = newblk;
    newblk->length = nbytes;
    newblk->next = curr;

    /* Merge if needed */
    if (newblkEnd == curr) {
        newblk->length += curr->length;
        newblk->next = curr->next;
    }

    if (prevEnd == newblk) {
        prev->length += newblk->length;
        prev->next = newblk->next;
    }

    intr_restore(intrMask);
    return OK;
}

status mem_freeStack(void *blkaddr, uint32 nbytes) {
    return mem_freeMem((void *)((uint64) blkaddr - mem_ceilMemBlock(nbytes) + sizeof(uint64)), nbytes);
}

void *mem_getMemMin() {
    return (void *) VIDEOCORE_BASE;
}

void *mem_getMemMax() {
    return (void *) PERIPHERAL_REAL_BASE;
}

memblk *mem_getFreeList() {
    return &m_memFreeList;
}

void mem_init() {
    m_memFreeList.length = PERIPHERAL_REAL_BASE - VIDEOCORE_BASE;
    memblk *heapBase = (memblk *) VIDEOCORE_BASE;

    m_memFreeList.next = heapBase;
    heapBase->length = m_memFreeList.length;
    heapBase->next = nullptr;
}