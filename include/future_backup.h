#include<xinu.h>
#include<kernel.h>
#include<stdbool.h>
#include <prodcons.h>
#include <myqueue.h>

#ifndef _FUTURE_H_
#define _FUTURE_H_

/* define states */
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2     



typedef struct futent
{
	int *value;		
	int flag;		
	int state;         	
	pid32 pid;
   	int set_head;
	int set_tail;
	int get_head;
	int get_tail;
	pid32 set_queue[myQSIZE];
	pid32 get_queue[myQSIZE];

} future;

extern int TEST;
//extern future *f1,*f2,*f3;
//typedef struct futent future;
extern sid32 queue_lock;
sid32 produced1;

typedef unsigned int uint;
/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1
#define FUTURE_SHARED 2	
#define FUTURE_QUEUE  3

/* Interface for system call */
extern future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);
uint future_prod(future*);
uint future_cons(future*);

uint network_cons(future*,int32,char []);
uint network_prod(future*,int32);
#endif /* _FUTURE_H_ */
