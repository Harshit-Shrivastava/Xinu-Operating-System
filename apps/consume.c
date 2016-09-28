#include <xinu.h>
#include <prodcons.h>

void consumer(sid32 consumed, sid32 produced, int32 count) {
	int32 i;
	for(i=1; i<=count; i++)
	{
		wait(produced);
		printf("\tConsumed value is %d\n", n);
		signal(consumed);
	}
}

