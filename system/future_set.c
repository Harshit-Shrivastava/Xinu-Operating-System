#include <prodcons.h>
#include <stdlib.h>
#include <xinu.h>
#include <future.h>
/*
*Description
*Set a value in a future and changes state from FUTURE_EMPTY or FUTURE_WAITING to FUTURE_VALID.
*Parameters
*future *f: future in which to set the value int *value: result of an operation to be set as value in a future
*Return
*syscall: SYSERR or OK
*/

int set_exclusive(future *f, int * value){
    if(f->state==FUTURE_WAITING || f->state==FUTURE_EMPTY){
        f->value=*value;
        f->state=FUTURE_VALID;
        //disable interrupts
        intmask im=disable();
        //put thread into ready queue
        ready(f->pid);
        //restore interrupts.
        restore(im);
        return OK;
    }
    return SYSERR;
}

int set_shared(future *f, int * value){
  printf("In producer\n");
    // if(f->state==FUTURE_WAITING){
        //print_queuee(f->get_queue);
        intmask im=disable();
        //set val
        f->value=*value;
        printf("value = %d\n",f->value);
        //set state
        int oldstate=f->state;
        //printf("oldstate=%d\n\r",oldstate);
        f->state=FUTURE_VALID;
        //if no consumer waiting
        // if(oldstate!=FUTURE_WAITING){
        //     restore(im);
        //     return OK;
        // }
        //wake up first consumer:
       printf("prod state:%d\n\r",oldstate);
       while (front < 4){
        //Dequeue first in queue:
        pid32 first = Dequeue();
       printf("first = %d\n", first);
        resume(first);
        front++;
        }
        restore(im);
        return OK;
    // }
    // printf("In SYSERR\n");
    // return SYSERR;
}
/*
 * consumer has to check if both queues are empty before setting the state. 
 * AND set the states accordingly.
 */
int set_queue(future *f, int * value){
   if (get_head != get_tail)
   {
       f->state = FUTURE_VALID;
       f->pid = getpid();
       f->value = *value;
       // printf("hello %d\n", *value);
       future *temp = dq_get();
       // printf("tid: %d\n", temp->tid);
       // printf("value: %d\n", temp->value);
       temp->state = FUTURE_VALID;
       temp->value = f->value;
       intmask im = disable();
       resume(temp->pid);
       restore(im);
   }
}

syscall future_set(future *f, int *value){
    if(f->flag==FUTURE_EXCLUSIVE){
        return set_exclusive(f,value);
    }
    else
    if (f->flag==FUTURE_SHARED){
        return set_shared(f,value);
    }
    else
    if (f->flag==FUTURE_QUEUE){
        return set_queue(f,value);
    }
    return SYSERR;
}
