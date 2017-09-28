#include "mymalloc.c"
#include <stdio.h>
#include <math.h>


int main()
{
	
	dump_heap(); 
	void *pointer = my_buddy_malloc(1000); 
	void *pointer2 = my_buddy_malloc(20000); 
	
	dump_heap(); 
	my_free(pointer); 
	dump_heap(); 
	my_free(pointer2); 
	dump_heap(); 

	printf("FINISH\n");
	
	
}