#ifndef _FUTURE_H_
#define _FUTURE_H_
 
/* define states */
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1	
#define FUTURE_SHARED     2	    
#define FUTURE_QUEUE      3     

int set_head;
int set_tail;

int get_head;
int get_tail;

typedef struct{
    pid32 thread;
    struct queue* next;
}queue;

typedef struct futent
{
   int *value;		
   int flag;		
   int state;         	
   pid32 pid;
   queue *set_queue;     
   queue *get_queue;
} future;

/* Interface for system call */
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);

pid32 check_thread(queue *q);
void Enqueue(queue *q,pid32 thr);
void put_thread_to_sleep(pid32 tid);
void Dequeue(queue *q);
future* dq_set();
future* dq_get();
void nq_get(future *f);
void nq_set(future *f);
 
#endif /* _FUTURE_H_ */
