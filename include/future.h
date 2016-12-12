#ifndef _FUTURE_H_
#define _FUTURE_H_
 
#include <fut_queue.h>

/* define states */
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1	
#define FUTURE_SHARED     2	    
#define FUTURE_QUEUE      3     

int front;
int rear;

int set_head;
int set_tail;

int get_head;
int get_tail;

typedef struct futent
{
   int *value;		
   int flag;		
   int state;         	
   pid32 pid;
} future;

typedef struct netfutent
{
   int *value;		
   int flag;		
   int state;         	
   pid32 pid;
   fut_queue *set_queue;     
   fut_queue *get_queue;
}netfuture;

pid32 queue[];

future set_q[];
future get_q[];

/* Interface for system call */
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);

sid32 produced1;

void Enqueue(pid32 pid);
pid32 Dequeue();
future* dq_set();
future* dq_get();
void nq_get(future *f);
void nq_set(future *f);
 
int fprod_udp_request(netfuture *fut,int serverip,int port,int locport);
int fcons_udp_print(netfuture *fut);
#endif /* _FUTURE_H_ */
