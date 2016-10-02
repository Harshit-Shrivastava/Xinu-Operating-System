#include <prodcons.h>

syscall future_get(future *future, int *value){
  intmask mask;
  mask = disable();
  if (future->state==FUTURE_EMPTY && future->pid==NULL){
    future->pid = getpid();
    future->state = FUTURE_WAITING;
    if(future->state == FUTURE_VALID){
      future->state = FUTURE_EMPTY;
      future->pid=NULL;
      restore(mask);
      return OK;
    }
  }
  restore(mask);
  return SYSERR;
}
