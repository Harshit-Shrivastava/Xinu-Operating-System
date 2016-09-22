/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
int32 cpudelay;
volatile uint32	gcounter = 400000000;
process	counterproc() {

	while(gcounter > 0) {
		gcounter--;
	}
	return OK;
}

process	main(void)
{
  char bbb_ipaddr[] = "192.168.1.101";
  char bbb_router[] = "192.168.1.255";
  /*  they should be as below, but to work with existing configs, they should be
   *  as above. 
  char bbb_ipaddr[] = "172.17.2.2";
  char bbb_router[] = "172.17.2.255";
 */
  
  /* Start the network */
  netstart(bbb_ipaddr,bbb_router);
  kprintf("\n...creating a shell\n");
  recvclr();
  resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
  
  /* Wait for shell to exit and recreate it */
  
  while (TRUE) {
    receive();
    sleepms(200);
    kprintf("\n\nMain process recreating shell\n\n");
    resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
  }
  return OK;
}
