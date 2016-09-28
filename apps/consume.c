#include <xinu.h>
#include <prodcons.h>

void consumer(sid32 consumed, sid32 produced) {
	int32 i;
	for(i=1; i<=2000; i++)
	{
		wait(produced);
		printf("Produced value %d\n", n);
		signal(consumed);
	}
}

