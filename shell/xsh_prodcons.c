#include <xinu.h>
#include <prodcons.h>

int32 n;                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

sid32 produced, consumed;

future *f1, *f2, *f3;

shellcmd xsh_prodcons(int32 nargs, char *args[])
{
	//Argument verifications and validations
	int32 count=2000;             //local varible to hold count
	int32 flag_future = 0; // initialise flag to 0	
	
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
/*
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
*/
	//create the process producer and consumer and put them in ready queue.
	//Look at the definations of function create and resume in the system folder for reference.      

	//if (nargs > 2){
	//		printf("Too many arguments\n");
	//		return 0;
	//	}
		 if (nargs == 1){
			flag_future =0;
			count = 2000;
		}
		else if (nargs == 2 ){
			if ( strncmp(args[1],"-f", 10) == 0){
				flag_future = 1;
			}
			else if (atoi(args[1]) <= 0)
			{
				printf("Please enter positive value greater that 0\n");
				return 0;
			}
			else{
				flag_future = 0;
				count = atoi(args[1]);
			}
		}
		else if(nargs > 3) {
		
			printf("Too many arguements \n");
			return 0;
		}
		else{
			printf("Invalid arguments\n Use --help \n");
			return 0;
		}
	if (flag_future == 0)
	{
	consumed = semcreate(1);
	produced = semcreate(0);
	//resume( create(producer, 1024, 20, "consumer", 3, consumed, produced, count));
	resume( create(producer, 1024, 20, "producer", 3, consumed, produced, count));
	resume( create(consumer, 1024, 20, "consumer", 3, consumed, produced, count));
	}

	else{
          f1 = future_alloc(FUTURE_EXCLUSIVE);
	  //f2 = future_alloc(FUTURE_EXCLUSIVE);
	  //f3 = future_alloc(FUTURE_EXCLUSIVE);
 
	  resume( create(future_cons, 1024, 20, "fcons1", 1, f1) );
	  resume( create(future_prod, 1024, 20, "fprod1", 1, f1) );
	  //resume( create(future_cons, 1024, 20, "fcons2", 1, f2) );
	  //resume( create(future_prod, 1024, 20, "fprod2", 1, f2) );
	  //resume( create(future_cons, 1024, 20, "fcons3", 1, f3) );
	  //resume( create(future_prod, 1024, 20, "fprod3", 1, f3) );	
	}

return (0);
}
