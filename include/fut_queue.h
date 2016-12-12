#ifndef _FUT_QUEUE_H_
#define _FUT_QUEUE_H_

#include <xinu.h>

typedef struct fut_qnode 
{
  pid32	key;		/* Key on which the queue is ordered	*/
  qid16	*next;
}fut_qnode;

typedef struct fut_queue
{
  fut_qnode *front,*rear;
}fut_queue;

/*queue manipulation functions */
fut_queue *fut_qcreate();
fut_qnode* newNode(pid32 pid);
int fut_enqueue(fut_queue *q, pid32 pid);
pid32 fut_dequeue(fut_queue *q);
int fut_isempty(fut_queue *q);

#endif
