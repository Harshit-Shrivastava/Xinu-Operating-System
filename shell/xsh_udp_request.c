#include "prodcons.h"
#include "future.h"
#include <string.h>
#include <stdlib.h>




shellcmd xsh_udp_request(int nargs, char *args[])
{

	int  portno, retval,fpid,temp;
	char buffer[512];
	char message[512];
	uint32 serverip;	
	int locport = 42000;
	int slot;		// slot of UDP table
	int msg_len,free_status;		// length of message, status of future
	int future_flag=0;
	future *f_exlusive;
	int value,status;

	if(nargs==4 && strcmp(args[3],"-f")==0)
	{
		//goto futures;
		future_flag = 1;
	}

	if(nargs > 4 || (nargs > 3 && strcmp(args[3],"-f")!=0 ))
	{
		printf("Too many arguments for udp_request command.\n");
		printf("Use --help to know about usage.\n");
		return -1;
	}	

	if(nargs==3 && strcmp(args[2],"-f")==0)
	{
		printf("\ntoo few arguments for future mode.\n");
		return -1;
	}

	if((nargs == 2) && strncmp(args[1],"--help",7)== 0)
	{
		
		printf("Use: %s  Find length of string\n\n");
		printf("Description:\n");
		printf("\tCommunicate with server using UDP (User Datagram Protocol).\n");
		printf("Options:\n");
		printf("\tServerip:\tIP address of the server\n");
		printf("\tPortno:\tPort number of the server\n");
		printf("\tServerip:\tNumber of which square needs to be computed\n");		
		printf("\t-f Use future mode \n");
		printf("\t--help\t display this help and exit\n");
		return -1;
	}

	if(nargs < 3)
	{
		printf("Too few arguments for udp_request command.\n");
		printf("Use --help to know about usage.\n");
		return -1;
	}

	if (dot2ip(args[1], &serverip) == SYSERR) {
		printf( "%s: invalid IP address argument\r\n",
			args[0]);
		return 1;
	}
	produced1 = semcreate(1);
	
	portno = atoi(args[2]);

	//strncpy(message,args[3],strlen(args[3]));
	slot = udp_register(serverip, portno, locport);		// register a slot in udp table
	if (slot == SYSERR) 	
	{
		printf(" Error while calling udp register %d ",locport);
		return -1;
	}

	if (!future_flag)	
	{
		while(1)
		{
			printf("\n>");
			fgets(message,512,CONSOLE);
			msg_len = strlen(message);
			message[msg_len-1]='\0';
			if(strcmp(message,"exit") == 0) 
				break;

			retval = udp_send(slot, message, msg_len);
			if(retval == SYSERR)
			{
				printf(" Error in udp sending datagram ");
				return -1;
	
			}
	
			retval = udp_recv(slot, buffer, sizeof(buffer),3000);
			if(retval == SYSERR)
			{ 	
				printf("Error in receiving datagram");
				return -1;

			}

			if(retval == TIMEOUT)
			{
				printf("Timeout in receiving datagram");
				return -1;
			}

			printf("Length of %s is  %s \n",message,buffer);
			memset(buffer,'\0',sizeof(buffer));
			memset(message,'\0',sizeof(message));
		}
		udp_release(slot);
	}	
	else
	{	
		f_exlusive = future_alloc(FUTURE_EXCLUSIVE);

		if (f_exlusive != NULL )
		{
			fpid = create(fcons_udp_print, 1024, 20, "fcons1", 1, f_exlusive);
			resume(fpid);
	
			while(1)
			{
				while(f_exlusive->state==FUTURE_VALID)
					printf("");

				printf("\n>");
				fgets(message,512,CONSOLE);
				msg_len = strlen(message);
				message[msg_len-1]='\0';

				if(strcmp(message,"exit") == 0) 
					break;
				retval = udp_send(slot, message, msg_len);
				if(retval == SYSERR)
				{
					printf(" Error in udp sending datagram ");
					return -1;
				}
	
				retval = udp_recv(slot, buffer, sizeof(buffer),3000);
				if(retval == SYSERR)
				{ 	
					printf("Error in receiving datagram");
					return -1;
				}
				if(retval == TIMEOUT)
				{
					printf("Timeout in receiving datagram");
					return -1;
				}
				value = atoi(buffer);
				printf("\nProducer produced %d",value);
				status = future_set(f_exlusive, &value);
  				if (status < 1)
  				{
    					printf("future_set failed\n");
    					return -1;
 				}
				if(value==-1)
					break;
				memset(buffer,'\0',sizeof(buffer));
				memset(message,'\0',sizeof(message));
		
			}
		}
		udp_release(slot);
	//	while(count)
	//		printf("");

//		if(!(future_free(f_exlusive)))
//			return SYSERR;


	}	
	return 0;

}
