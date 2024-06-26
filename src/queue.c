#include <queue.h>

struct q_entry queuetab[QUEUE_N_ENTRIES];

qid16 queue_head(qid16 qid) { return qid; }
qid16 queue_tail(qid16 qid) { return qid + 1; }
qid16 queue_firstId(qid16 qid) { return queuetab[queue_head(qid)].qnext; }
qid16 queue_lastId(qid16 qid) { return queuetab[queue_tail(qid)].qprev; }
qid16 queue_firstKey(qid16 qid) { return queuetab[queue_firstId(qid)].qkey; }
qid16 queue_lastKey(qid16 qid) { return queuetab[queue_lastId(qid)].qkey; }
bool queue_isEmpty(qid16 qid) { return queue_firstId(qid) >= N_PROC; }
bool queue_isNonEmpty(qid16 qid) { return queue_firstId(qid) < N_PROC; }
bool queue_isBadQId(qid16 qid) { return (int64) qid < 0 || (int64) qid >= QUEUE_N_ENTRIES - 1; }

bool isBadPId(pid32 pid) {
  return false;
}
pid32 queue_getItem(pid32 pid) {
    if (isBadPId(pid))
    {
        return -1;
    }

    pid32 next = queuetab[pid].qnext;
    pid32 prev = queuetab[pid].qprev;
    queuetab[prev].qnext = next;
    queuetab[next].qprev = prev;
    return pid;
}

pid32 queue_getFirst(qid16 qid) {
    if (queue_isBadQId(qid))
    {
        return -1;
    }

    if (queue_isEmpty(qid)) {
        return QUEUE_EMPTY;
    }

    pid32 head = queue_head(qid);
    return queue_getItem(queuetab[head].qnext);
}

pid32 queue_getLast(qid16 qid) {
    if (queue_isBadQId(qid))
    {
        return -1;
    }

    if (queue_isEmpty(qid)) {
        return QUEUE_EMPTY;
    }

    pid32 tail = queue_tail(qid);
    return queue_getItem(queuetab[tail].qprev);
}

pid32 queue_enqueue(qid16 qid, pid32 pid) {
    if (queue_isBadQId(qid) || isBadPId(pid))
    {
        return -1;
    }

    qid16 tail = queue_tail(qid);
    qid16 prev = queuetab[tail].qprev;

    queuetab[pid].qnext = tail;
    queuetab[pid].qprev = prev;
    queuetab[prev].qnext = pid;
    queuetab[tail].qprev = pid;
    return pid;
}

pid32 queue_dequeue(qid16 qid) {
    if (queue_isBadQId(qid))
    {
        return -1;
    }

    if (queue_isEmpty(qid))
    {
        return QUEUE_EMPTY;
    }

    pid32 pid = queue_getFirst(qid);
    queuetab[pid].qnext = QUEUE_EMPTY;
    queuetab[pid].qprev = QUEUE_EMPTY;
    return pid;
}

int32 queue_insert(qid16 qid, pid32 pid, int32 key) {
    if (queue_isBadQId(qid) || isBadPId(pid)) {
        return -1;
    }

    if (key == QUEUE_MINKEY) {
        return -2;
    }

    qid16 curr = queue_firstId(qid);

    while (queuetab[curr].qkey >= key) {
        curr = queuetab[curr].qnext;
    }

    qid16 prev = queuetab[curr].qprev;
    queuetab[pid].qnext = curr;
    queuetab[pid].qprev = prev;
    queuetab[pid].qkey = key;
    queuetab[prev].qnext = pid;
    queuetab[curr].qprev = pid;
    return 0;
}

qid16 queue_getNew() {
    static qid16 nextQId = N_PROC;

    qid16 newQId = nextQId;
    if (newQId > QUEUE_N_ENTRIES) {
        return -1;
    }

    nextQId += 2; /* Increment by 2 (head + tail entry) */

    queuetab[queue_head(newQId)].qnext = queue_tail(newQId);
    queuetab[queue_head(newQId)].qprev = QUEUE_EMPTY;
    queuetab[queue_head(newQId)].qkey = QUEUE_MAXKEY;

    queuetab[queue_tail(newQId)].qprev = queue_head(newQId);
    queuetab[queue_tail(newQId)].qnext = QUEUE_EMPTY;
    queuetab[queue_tail(newQId)].qkey = QUEUE_MINKEY;

    return newQId;
}