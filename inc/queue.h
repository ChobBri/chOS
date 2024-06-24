/* # of queue entries */
#ifndef NQENT
#define NQENT (NPROC + 4 + NSEM + NSEM)
#endif

#define EMPTY (-1)
#define MAXKEY 0x7FFFFFFF
#define MINKEY 0x80000000

struct qentry
{
  int32 qkey;
  qid16 qnext;
  qid16 prev;
}