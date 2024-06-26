#pragma once

#include <types.h>

#define N_PROC 100
#define QUEUE_N_LISTS 2
#define QUEUE_N_ENTRIES (N_PROC + QUEUE_N_LISTS + QUEUE_N_LISTS)

#define QUEUE_EMPTY (-1)
#define QUEUE_MAXKEY (0x7FFFFFFF)
#define QUEUE_MINKEY (0x80000000)

typedef uint16 qid16;
typedef uint32 pid32;

typedef struct q_entry {
    int32 qkey;
    qid16 qnext;
    qid16 qprev;
} q_entry;

bool isBadPId(pid32 pid);

qid16 queue_head(qid16);
qid16 queue_tail(qid16);
qid16 queue_firstId(qid16);
qid16 queue_lastId(qid16);
qid16 queue_firstKey(qid16);
qid16 queue_lastKey(qid16);
bool queue_isEmpty(qid16);
bool queue_isNonEmpty(qid16);
bool queue_isBadQId(qid16);

pid32 queue_getItem(pid32);
pid32 queue_getFirst(qid16);
pid32 queue_getLast(qid16);

pid32 queue_enqueue(qid16, pid32);
pid32 queue_dequeue(qid16);

int32 queue_insert(qid16, pid32, int32);

qid16 queue_getNew();
