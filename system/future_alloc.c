#include <prodcons.h>
#include <stdlib.h>
/*
*Description
*Allocates a new future (in the FUTURE_EMPTY state) in given mode. We will use memget() call to allocate space to new future.
*Parameters
*int future_flag: flag to be set for this future
*Return
*future: NULL or a valid future reference
*/

future* future_alloc(int future_flag){
  intmask mask;
  mask = disable();
  future *fut = getmem(sizeof(future));
  fut->flag = future_flag;
  fut->state = FUTURE_EMPTY;
  fut->pid = NULL;
  restore(mask);
  return fut;
}
