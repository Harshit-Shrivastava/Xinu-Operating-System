#include <prodcons.h>
//Code to consume values of global variable 'n' until the value of n is less than or equal to count
void consumer(int32 count, sid32 consumed, sid32 produced)
{
	int32 i;
    for (i = 0; i <= count; i++)
    {
    	wait(produced);
    	kprintf("consumed :%d\n", n);//print consumed value e.g. consumed : 8
    	signal(consumed);
    }
    exit(0);
}
