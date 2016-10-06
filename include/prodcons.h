#include <xinu.h>
#include <stddef.h>
#include <stdio.h>
#include <future.h>
#include <string.h>

typedef unsigned int uint;
future *f1, *f2, *f3;
/*Global variable for producer consumer*/
extern int32 n; /*this is just declaration*/

/* Declare the required semaphores */
extern sid32 consumed, produced; 
/*function Prototype*/
void consumer(sid32 consumed, sid32 produced, int32 count);
void producer(sid32 consumed, sid32 produced, int32 count);
uint future_cons(future *fut);
uint future_prod(future *fut);
