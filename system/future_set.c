#include <prodcons.h>

syscall future_set(future *future, int *value){
  intmask mask;
  mask = disable();
  if(future->state == FUTURE_WAITING || future->state == FUTURE_EMPTY){
    future->value = *value;
    future->state = FUTURE_VALID;
    restore(mask);
    return OK;
  }
  restore(mask);
  return SYSERR;
}
