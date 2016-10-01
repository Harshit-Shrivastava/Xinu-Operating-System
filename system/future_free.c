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
	// if (f->set_queue!=NULL)
 //         freemem(f->set_queue);
 //    if (f->get_queue!=NULL)
 //         freemem(f->get_queue);
 //     freemem(f->value);
     syscall returnval = freemem(f, sizeof(future));
     restore(im);
	return returnval;
}
