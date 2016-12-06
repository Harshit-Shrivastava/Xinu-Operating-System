#include <prodcons.h>

 void producer(int32 count, sid32 consumed, sid32 produced)
 {
 	int32 i =0;
 	for (i = 0; i < count; i++) //Code to produce values less than equal to count
 	{
 		wait(consumed);
 		n = i; //produced value should get assigned to global variable 'n'
 		kprintf("produced : %d\n", n); //print produced value e.g. produced : 8
 		signal(produced);
 	}
 	exit(0);
}
