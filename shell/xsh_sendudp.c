#include <xinu.h>
#include <stdio.h>
#include <string.h>
/*------------------------------------------------------------------------
 * xsh_hello - prints the welcome message
 *------------------------------------------------------------------------
 */

#define SIZE 1024

shellcmd xsh_sendudp(int nargs, char *args[]) {
	int	retval;
	char response[SIZE];
	uint32 target_ip, locport = 3000;
	
	printf("Command found");

	/* Output info for '--help' argument */
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) 
	{
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tDisplays a welcome message for the given user name.\n");
		printf("Options (one per invocation):\n");		
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
	}

	/* Check argument count */
	/* If argument count is greater than 2, then there are too many arguments*/
	if (nargs > 2) 
	{
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		return 1;
	}

	/* If argument count is less than 2, then there are too few arguments*/
	if (nargs < 2) 
	{
		fprintf(stderr, "%s: too few arguments\n", args[0]);
		return 1;
	}
	
	udp_init();
	int32 slot =0;
	uint32 ip;
	dot2ip("192.168.1.100",&target_ip);
	
	/* register local UDP port */
	slot=udp_register(target_ip,8000,locport);
	if (slot == SYSERR) {
		fprintf(stderr, "%s: could not reserve UDP port %d\n",
				args[0], locport);
		return 1;
	}
	else
		kprintf("\nSlot obtained : %d",slot);


	retval = udp_send(slot,args[1],strlen(args[1]));
	if (retval == SYSERR) {
			fprintf(stderr, "%s: error sending UDP \n",
				args[0]);
			return 1;
		}
	// udp_sendto(slot,target_ip,8080,args[1],strlen(args[1]));
	
	retval = udp_recv(slot,response,SIZE,15000);
	if (retval == TIMEOUT) {
			fprintf(stderr, "%s: timeout...\n", args[0]);
	} else if (retval == SYSERR) {
		fprintf(stderr, "%s: error from udp_recv \n", args[0]);
		udp_release(slot);
		return 1;
	}
	else
		printf("Received %s\n",response);

	return 0;
}
