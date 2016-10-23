#include <prodcons.h>
#include <stdlib.h>
#include <xinu.h>
#include <future.h>
/*
*Description
*Get the value of a future set by an operation and change the state of future from FUTURE_VALID to FUTURE_EMPTY.
*Parameters
*future *f: future on which to wait for and obtain value
*int *value: variable in which to obtain the value from a future
*Return
*syscall: SYSERR or OK
*/

int get_exclusive(future *f, int *value){
    if(f->state==FUTURE_EMPTY){
        //set pid:
        f->pid=getpid();
        //set state:
        f->state=FUTURE_WAITING;
        //disable interrupts
        intmask im=disable();
        //sleep:
       // put_thread_to_sleep(getpid())
	suspend(f->pid);
        restore(im);
        if(f->state==FUTURE_VALID){
            f->state=FUTURE_EMPTY;
            *value=f->value;
            //reset pid:
            f->pid=NULL;
            return OK;
        }
    }
    else if (f->state==FUTURE_VALID){
        //just get value:
        intmask im=disable();
        f->state=FUTURE_EMPTY;
        *value=f->value;
        //reset pid:
        f->pid = NULL;
        restore(im);
        return OK;

    }
}

int get_shared(future *f, int * value){
    while(1){
        if(f->state==FUTURE_VALID){
            //get value:
            *value=f->value;
            //last in queue?
            if(front != 4 && rear != 4){
                //wake up next consumer:
                intmask im=disable();
                // pid32 first = Dequeue();
                // resume(first);
                restore(im);
                return OK;
            }
            else{
                //last one: 
                //get value:
                *value=(f->value);
                (f->value)=0xddddffff;
                f->state=FUTURE_EMPTY;
                return OK;
            }
        }
        else if(f->state==FUTURE_WAITING){
            //add yourself to queue:
            intmask im=disable();
            Enqueue(getpid());
            //print_queue(f->get_queue);
            //  put_thread_to_sleep(getpid())
	        suspend(getpid());
            restore(im);
        }
        else if(f->state==FUTURE_EMPTY){
            intmask im=disable();
            //make queue:
            //build head node:
            f->pid=getpid();
            f->state=FUTURE_WAITING;
            Enqueue(getpid());
            //  put_thread_to_sleep(getpid());
	        suspend(f->pid);
            restore(im);
        }
    }
}

int get_queue(future *f, int * value){
    if (set_head != set_tail)
   {
       intmask im = disable();
       future *temp = dq_set();
       printf("%d\n", temp->pid);
       temp->state = FUTURE_VALID;
       *value = temp->value;
       resume(temp->pid);
       restore(im);
   }
   else{
        intmask im = disable();
        if (f->state == FUTURE_VALID)
        {
           *value = f->value;
           return OK; 
        }
        else if (f->state == FUTURE_EMPTY || f-> state == FUTURE_WAITING)
        {
            f->state = FUTURE_WAITING;
            f-> value = *value;
            f->pid = getpid();
            nq_get(f);
         //   put_thread_to_sleep(f->pid);
	        suspend(f->pid);
        }
        restore(im);
   }
}

syscall future_get(future *f, int *value){
    //if pid id not set and state is empty
    if(f->flag==FUTURE_EXCLUSIVE){
        return get_exclusive(f,value);
    }
    else if (f->flag==FUTURE_SHARED){
        return get_shared(f,value);
    }
    else if (f->flag==FUTURE_QUEUE){
        return get_queue(f,value);
    }
    return SYSERR;
}
