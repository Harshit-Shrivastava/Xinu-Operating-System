#include <prodcons.h>
#include <stdlib.h>
/*
*Description
*freemem the allocated future. This function will simply freemem the space assigned to Future. Use memfreemem(). call to freemem the space.
*Parameters
*future* f: future to freemem
*Return
*syscall: SYSERR or OK
*/

syscall future_freemem(future* f){
  intmask im=disable();
  syscall returnval = freemem(f, sizeof(future));
  restore(im);
  return returnval;
}
