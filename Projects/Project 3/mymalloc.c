//Sai Konduru 
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>
#define MAX_MEM 1<<30
#include <stdio.h>
#include <math.h>

struct Node{
	unsigned char header;
	struct Node* prev;
	struct Node* next; 
}; 

struct Node *freeList[26] = {NULL}; 
static void *base = NULL; 

void *my_buddy_malloc(int size)
{
	//Check for invalid size. 
	if(size > MAX_MEM)
	{
		printf("Sorry request too large.\n");
		return NULL; 
	}
	
	//First time my_buddy_malloc is executed run this. 
	if(base == NULL)
	{
		base = mmap(NULL, MAX_MEM, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, 0, 0);  
		freeList[25] = base; 
		freeList[25]->header = 30; 
		freeList[25]->next = NULL; 
		freeList[25]->prev = NULL; 
	}
	assert(base != NULL); 
	
	struct Node *result = NULL; 
	//Find the index most appropriate to the size given. 
	//Index refers to the index of freeList that the given size most likely would go to. 
	int index = power(size + 1); 
	int originalIndex = index; 
	
	//Check if freeList has the block that user asks for. 
	if(freeList[index] != NULL)
	{
		if(freeList[index]->next == NULL)
		{
			result = freeList[index]; 
			freeList[index] = NULL; 
			result->next = NULL;
			result->prev = NULL;
			result->header = index+5;
			result->header |= 128;
		}
		else
		{
			result = freeList[index]; 
			freeList[index] = freeList[index]->next; 
			freeList[index]->prev = NULL; 
			result->next = NULL;
			result->prev = NULL;
			result->header = index+5;
			result->header |= 128;
		}
	}
	else
	{
		//Find the next allocated block to which can be used by the user. 
		while(freeList[index] == NULL)
		{
			++index; 
		}
		
		//Break the allocated block down until it can best fit the size that user requests. 
		while(index != originalIndex)
		{
			struct Node *space = freeList[index]; 
			int half = (1 << (index +5))/2; 
			struct Node *buddy = (struct Node*)((char *)space + half); 
			
			if(space->next != NULL)
			{
				freeList[index] = space->next; 
				space->next = NULL; 
			}
			else{
				freeList[index] = NULL; 
			}
			space->next = buddy; 
			buddy->prev = space; 
			--index; 
			freeList[index] = space; 
			space->header = index + 5; 
			buddy->header = index + 5; 
		}
		result = freeList[index]; 
		freeList[index] = freeList[index]->next; 
		freeList[index]->prev = NULL; 
		result->next = NULL;
		result->prev = NULL;
		result->header = index+5;
		result->header |= 128;
	}

	//return a pointer pointing one byte from the header. 
	return (void *)((char *)result + 1);
}
int power(int x)
{
	int i = 0; 
	for(i = 0; i<26; i++)
	{
		if((1<<(i+5)) >= x)
		{
			return i; 
		}
	}
}

void my_free(void *ptr)
{
	//Check if pointer is valid. 
	if(ptr == NULL)
	{
		printf("Pointer already pointing to NULL.\n"); 
		return; 
	}
	
	struct Node *free = (struct Node*)((char *)ptr-1);
	//set first bit as 0, or free
	free->header&=127;
	//find location of buddy
	void *buddyLocation = (((void *)((char *)ptr-1)-base) ^ (1 << free->header)) + base;
	struct Node *buddy = (struct Node *)buddyLocation;
	//a buddy is available and buddy is the same size as the block being freed
	while(buddy->header==free->header)
	{
		//separate buddy from its location in freeList
		if(buddy->prev!=NULL)
		{
			if(buddy->next==NULL)
			{
				buddy->prev->next=NULL;
				freeList[buddy->header-5]=NULL;
			}
			else
			{
				buddy->prev->next=buddy->next;
			}
		}
		else
		{
			freeList[buddy->header-5]=buddy->next;
		}
			
		if(buddy->next!=NULL)
		{
			buddy->next->prev=buddy->prev;
		}
		buddy->next=NULL;
		buddy->prev=NULL;

		if(buddy<free)
		{
			free=buddy;
		}
		free->header++;
		//find the next buddy of the new free block
		buddyLocation=(((void *)(free)-base) ^ (1 << free->header)) + base;
		buddy=(struct Node *)buddyLocation;
	}
	
	//place the buddy in the freeList based on if the list has a head or not
	int index =free->header-5;
	if(freeList[index]!=NULL)
	{
		freeList[index]->prev = free;
		free->next = freeList[index];
		freeList[index] = free;
		free->prev = NULL;
	}
	else
	{
		freeList[index]=free;
		free->prev=NULL;
		free->next=NULL;
	}

}
void dump_heap()
{
	struct Node *cur;
	printf("head->\n");
	int i; 
	int size; 
	for(i = 0; i < 26; i++)
	{
		printf("%d->", i + 5); 
	  	if(freeList[i] == NULL)
		{
			printf("NULL \n"); 
		}
		else
		{
			cur = freeList[i]; 
			while(cur->next != NULL)
			{
				size = 1 << (cur->header); 
				printf("[%d, %d, %d]->", 0, (int)((char *)cur - (char *)base), size);
				cur = cur->next; 
			}
			size = 1 << freeList[i]->header; 
			printf("[%d, %d, %d]->NULL", 0, (int)((char *)cur - (char *)base), size); 
			printf("\n"); 
			
			if(cur->next != NULL)
			{
				assert(cur->next->prev == cur); 
			}
		}
	}
}