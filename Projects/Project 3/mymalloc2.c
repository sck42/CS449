//Sai Konduru 

#include "mymalloc.h"

struct Node{
	int header;
	struct Node *prev;
	struct Node *next; 
}node; 


struct Node *freeList[26] = {NULL}; 
static void *base = NULL; 

void *my_buddy_malloc(int size)
{
	if(base == NULL)
	{
		base = mmap(NULL, MAX_MEM, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, 0, 0); 
		freeList[25] = base; 
		freeList[25]->header = 30; 
		freeList[25]->next = NULL; 
		freeList[25]->prev = NULL; 
	}
	assert(base != NULL); 
	
	int allocSize = power(size); 
	void *result = freeRoom(allocSize); 
	return result; 
}
int power(int x)
{
	int i;
	for(i = 5; i <= 30; i++)
	{
		if((1 << i) > x)
		{
			return i;  
		}
	}
}
void *freeRoom(int size)
{
	int i; 
	for(i = 0; i < 26; i++)
	{
		//if(freeList[i] != NULL)
		//{
			int pwr = power(freeList[i]->header); 
			if(pwr == size)
			{
				freeList[i]->header &= 255; 	//SET THE MSB TO 1 BECAUSE BLOCK NO LONGER FREE. 
				void *result = freeList[i]; 
				freeList[i] = freeList[i]->next; 
				if(freeList[i] != NULL)
				{
					freeList->prev = NULL; 
				}
				return result; 
			}
			else
			{
				void *result = recursiveSplitter(i, size); 
				return result; 
			}
		//}
	}
	printf("Sorry we don't have any more memory."); 
	return NULL; 
}

void *recursiveSplitter(int index, int baseCase)
{
	struct Node *temp = freeList[index]; 
	int size = ((1 << (index + 5))/2;			//Half of the block that needs to be split. 
	struct Node *toSplit = (temp + size); 
	if(temp->next != NULL)
	{
		freeList[index] = temp->next; 
		temp->next = NULL;
		freeList[index]->prev = NULL; 
	}
	else
	{
		freeList[index] = NULL; 
	}
	temp->next = toSplit; 
	toSplit->prev = temp; 
	
	if(freeList[index - 1] != NULL)
	{
		toSplit->next = freeList[index - 1];
		freeList[index - 1]->prev = toSplit;  
	}
	else{
		toSplit->next = NULL; 
	}
	
	freeList[index - 1] = temp; 
	temp->prev = NULL; 
	temp->header = index +4;
	toSplit->header = index + 4; 
	
	if(freeList[index - 1]->header == baseCase)
	{
		freeList[index - 1]->header &= 255; 
		void *tmp = freeList[index - 1]; 
		freeList[index - 1] = freeList[index]->next; 
		if(freeList[index - 1] != NULL)
		{
			freeList[index - 1]->prev = NULL; 
		}
		return tmp; 
	}
	else
	{
		return recursiveSplitter(index - 2, baseCase - 5); 
	}
}
void dump_heap()
{
  struct Block *cur;
  printf("head->");
  for(cur = head; cur != NULL; cur = cur->next) {
    printf("[%d:%d:%d]->", cur->occ, (char*)cur - (char*)head, cur->size);
    assert((char*)cur >= (char*)head && (char*)cur + cur->size <= (char*)sbrk(0)); // check that block is within bounds of the heap
    if(cur->next != NULL) assert(cur->next->prev == cur); // if not last block, check that forward/backward links are consistent
  }
  printf("NULL\n");
}