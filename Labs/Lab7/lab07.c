#include <stdio.h>
#include <pthread.h> 

int x; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
void *foo(void *p)	
{
	int	i;
	pthread_mutex_lock(&mutex); 
	for(i=0; i < 100000; i++)	
	{
		x++;
	}
	pthread_mutex_unlock(&mutex); 
	return	NULL;
}

int main()
{
	pthread_t thread, thread2; 
	int id, id2; 
	int arg1, arg2; 
	id = pthread_create(&thread, NULL, foo, NULL); 
	id2 = pthread_create(&thread2, NULL, foo, NULL); 
	pthread_join(thread, NULL);
	pthread_mutex_lock(&mutex); 
	printf("x=%d\n", x);
	pthread_mutex_unlock(&mutex); 
	foo(NULL); 
}