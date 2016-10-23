#include <xinu.h>
#include <future.h>
#include <prodcons.h>

int32 n;                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/
sid32 produced, consumed;
future  *f_exclusive,  *f_shared,  *f_queue;

pid32 queue[5];
int rear = -1;
int front = -1;





int set_head = 0;
int set_tail = 0;

int get_head = 0;
int get_tail = 0;


future set_q[5];
future get_q[5];

shellcmd xsh_prodcons(int32 nargs, char *args[]){
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

  //create the process producer and consumer and put them in ready queue.
  //Look at the definations of function create and resume in the system folder 
  //for reference.      
  if (nargs == 1){
    flag_future =0;
    count = 2000;
  }
  else if (nargs == 2 ){
    if ( strncmp(args[1],"-f", 10) == 0){
      flag_future = 1;
    }
    else if (atoi(args[1]) <= 0){
      printf("Please enter positive value greater that 0\n");
      return 0;
    }
    else{
      flag_future = 0;
      count = atoi(args[1]);
    }
  }
  else if(nargs > 2) {		
    printf("Too many arguements \n");
    return 0;
  }
  else{
    printf("Invalid arguments\n Use --help \n");
    return 0;
  }
  if (flag_future == 0){
    consumed = semcreate(1);
    produced = semcreate(0);
    resume( create(producer, 1024, 20, "producer", 3, consumed, produced, count));
    resume( create(consumer, 1024, 20, "consumer", 3, consumed, produced, count));
  }
  else{
    //f1 = future_alloc(FUTURE_EXCLUSIVE);
    //f2 = future_alloc(FUTURE_EXCLUSIVE);
    //f3 = future_alloc(FUTURE_EXCLUSIVE);
    f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
    f_shared = future_alloc(FUTURE_SHARED);
    f_queue = future_alloc(FUTURE_QUEUE);

  
//Test FUTURE_EXCLUSIVE
 	 //  resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
  	// resume( create(future_prod, 1024, 20, "fprod1", 1, f_exclusive) );
 
 		//  Test  FUTURE_QUEUE
         // resume(  create(future_cons,  1024,  20,  "fcons6",  1,  f_queue)  );
         // resume(  create(future_cons,  1024,  20,  "fcons7",  1,  f_queue)  );
         // resume(  create(future_cons,  1024,  20,  "fcons7",  1,  f_queue)  );  
         // resume(  create(future_cons,  1024,  20,  "fcons7",  1,  f_queue)  );
         // resume(  create(future_prod,  1024,  20,  "fprod3",  1,  f_queue)  );
         // resume(  create(future_prod,  1024,  20,  "fprod4",  1,  f_queue)  );
         // resume(  create(future_prod,  1024,  20,  "fprod5",  1,  f_queue)  );
         // resume(  create(future_prod,  1024,  20,  "fprod6",  1,  f_queue)  ); 

    // Test FUTURE_SHARED
        resume(  create(future_cons,  1024,  20,  "fcons2",  1,  f_shared)  );
        resume(  create(future_cons,  1024,  20,  "fcons3",  1,  f_shared)  );
        // resume(  create(future_cons,  1024,  20,  "fcons4",  1,  f_shared)  );  
        // resume(  create(future_cons,  1024,  20,  "fcons5",  1,  f_shared)  );  
        resume(  create(future_prod,  1024,  20,  "fprod2",  1,  f_shared)  );
        
        
		// sleep(1);     

        syscall exclusive = future_freemem(f_exclusive);
    	syscall shared = future_freemem(f_shared); 
	syscall queue = future_freemem(f_queue);
		if (!(exclusive == (uint32 *)OK && shared == (uint32 *)OK && queue == (uint32 *)OK))
		{
			printf("There is an error, please check\n");
		}
		else{
			printf("Success\n");
		}	
  }
  return (0);
}
