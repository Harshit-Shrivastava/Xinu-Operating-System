#include <xinu.h>
#include <prodcons.h>

void producer(sid32 consumed, sid32 produced) {
	
	int32 i;
	for(i = 1; i <= 2000; i++)
	{
		wait(consumed);
		n = i;
		signal(produced);
		printf("Producer count = %d",n); 
	}
}



