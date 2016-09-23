#include <xinu.h>
#include <prodcons.h>

int32 n;                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

shellcmd xsh_prodcons(int32 nargs, char *args[])
{
	//Argument verifications and validations
	int32 count=2000;             //local varible to hold count

	if(nargs == 2)
	{	
		count = atoi(args[1]);
	}
	

	//create the process producer and consumer and put them in ready queue.
	//Look at the definations of function create and resume in the system folder for reference.      
	resume( create(producer, 1024, 20, "producer", 1, count));
	resume( create(consumer, 1024, 20, "consumer", 1, count));
	return (0);
}
