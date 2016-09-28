#include <xinu.h>
#include <prodcons.h>

void producer(sid32 consumed, sid32 produced, int32 count)
{
	
	int32 i;
	for(i = 1; i <= count; i++)
	{
		wait(consumed);
		n = i;
		signal(produced);
		printf("Produced value is %d",n); 
	}
}



