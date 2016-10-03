#include <prodcons.h>

syscall future_get(future *future, int *value){
  intmask mask;
  mask = disable();
   if(future->state==FUTURE_EMPTY){
        //set tid:
        future->pid=getpid();
        //set state:
        future->state=FUTURE_WAITING;
        //disable interrupts
        intmask im=disable();
        //sleep:
        // put_thread_to_sleep(getpid());
        suspend(future->pid); 
	restore(im);
        if(future->state==FUTURE_VALID){
            future->state=FUTURE_EMPTY;
            *value=future->value;
            //reset tid:
            future->pid=NULL;
            return OK;
        }
    }
    else if (future->state==FUTURE_VALID){
        //just get value:
        intmask im=disable();
        future->state=FUTURE_EMPTY;
        *value=future->value;
        //reset tid:
        future->pid=NULL;
        restore(im);
        return OK;
    }
  return SYSERR;
}
