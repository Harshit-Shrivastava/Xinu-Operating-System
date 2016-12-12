#ifndef _MYQUEUE_H_
#define _MYQUEUE_H_

#include <kernel.h>
#include<stdbool.h>
/* Queue function prototypes */
#define myQSIZE 10    

pid32 enQ(pid32 queue[],pid32 pid, int * head, int * tail);
pid32 deQ(pid32 Q[],int *head, int *tail);			/* ID of process removed	*/
bool isQempty(int * head, int * tail);

#endif 
