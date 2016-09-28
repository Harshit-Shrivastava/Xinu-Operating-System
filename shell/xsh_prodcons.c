#include <xinu.h>
#include <prodcons.h>

int32 n;                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

sid32 produced, consumed;

shellcmd xsh_prodcons(int32 nargs, char *args[])
{
	//Argument verifications and validations
	int32 count=2000;             //local varible to hold count
	
	//Implementation for help
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {

		printf("Use:\n");
		printf("\t%s [command]\n", args[0]);
		printf("Description:\n");
		printf("\tProvides a list of shell commands or\n");
		printf("\thelp information for a specific command\n");
		printf("Options:\n");
		printf("\tcommand\tspecific command for which to\n");
		printf("\t\tdisplay help information\n");
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
	}
	//If the count is provided in the command
	else if(nargs == 2)
	{	
		count = atoi(args[1]);
	}
	//More than two arguments are provided
	else if(nargs > 2)
	{
		printf("More than two parameters provided. Invalid arguments\n");
		return 0;
	}
	//No argument provided
	else if (nargs == 1)
	{
		count = 2000;
	}
	//All other cases
	else
	{
		printf("Invalid arguments\n");
		return 0;
	}

	//create the process producer and consumer and put them in ready queue.
	//Look at the definations of function create and resume in the system folder for reference.      
	consumed = semcreate(1);
	produced = semcreate(0);
	//resume( create(producer, 1024, 20, "consumer", 3, consumed, produced, count));
	resume( create(producer, 1024, 20, "producer", 3, consumed, produced, count));
	resume( create(consumer, 1024, 20, "consumer", 3, consumed, produced, count));
	return (0);
}
