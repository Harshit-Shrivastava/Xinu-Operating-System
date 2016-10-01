#include <prodcons.h>
#include <stdlib.h>
/*
*Description
*Set a value in a future and changes state from FUTURE_EMPTY or FUTURE_WAITING to FUTURE_VALID.
*Parameters
*future *f: future in which to set the value int *value: result of an operation to be set as value in a future
*Return
*syscall: SYSERR or OK
*/

void print_queuee(queue*q){
    while(q!=NULL){
        printf("element:%d\n\r",q->thread);
        q=q->next;
    }
}

int set_exclusive(future *f, int * value){
    if(f->state==FUTURE_WAITING || f->state==FUTURE_EMPTY){
        f->value=*value;
        f->state=FUTURE_VALID;
        //disable interrupts
        intmask im=disable();
        //put thread into ready queue
        ready(f->tid);
        //restore interrupts.
        restore(im);
        return OK;
    }
    return SYSERR;
}

int set_shared(future *f, int * value){
    if(f->state!=FUTURE_VALID){
        //print_queuee(f->get_queue);
        intmask im=disable();
        //set val
        f->value=*value;
        //set state
        int oldstate=f->state;
        //printf("oldstate=%d\n\r",oldstate);
        f->state=FUTURE_VALID;
        //if no consumer waiting
        if(oldstate!=FUTURE_WAITING){
            restore(im);
            return OK;
        }
        //wake up first consumer:
        //printf("prod state:%d\n\r",oldstate);
        //Dequeue first in queue:
        int first=check_thread(f->get_queue);
        Dequeue(f->get_queue);
        resume(first);
        restore(im);
        return OK;
    }
    return SYSERR;
}
/*
 * consumer has to check if both queues are empty before setting the state. 
 * AND set the states accordingly.
 */
int set_queue(future *f, int * value){
   if (get_head != get_tail)
   {
       f->state = FUTURE_VALID;
       f->tid = getpid();
       f->value = *value;
       // printf("hello %d\n", *value);
       future *temp = dq_get();
       // printf("tid: %d\n", temp->tid);
       // printf("value: %d\n", temp->value);
       temp->state = FUTURE_VALID;
       temp->value = f->value;
       intmask im = disable();
       resume(temp->tid);
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
