#include "mymalloc.h"

node *freeList[26] = {NULL}; 
static void *base = NULL; 


void *my_buddy_malloc(int size)
{
	if(base == NULL)
	{
		base = mmap(NULL, MAX_MEM, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, 0, 0); 
		freeList[25] = base; 
		freeList[25]->header = 30; 
		freeList[25]->next = NULL; 
		freeList[25]->previous = NULL; 
	}
	assert(base != NULL); 
	int pwr = power(size); 
	void *result = findFreeSpace(pwr);
//	dump_heap();
	return result; 
}
void my_free(void *ptr)
{
	node *free = (struct Node*)ptr; 
	if(free->header == 30)
	{
		return; 
	}
	free->header &= 127; 
	
	void *val = ((ptr-base) ^ (1<< free->header)) + base;
	node *findBuddy = (struct Node*)val; 
	unsigned char myBits = findBuddy->header; 
	myBits = myBits >> 7; 
	
	if(myBits == 0)
	{
		if(findBuddy->next == NULL && findBuddy->previous == NULL)
		{
			freeList[findBuddy->header - 5] = NULL; 
		}
		else if(findBuddy->next == NULL && findBuddy->previous != NULL)
		{
			node *temp = findBuddy->previous; 
			temp->next = NULL;
			freeList[findBuddy->header - 5] = NULL; 
		}
		else if(findBuddy->next != NULL && findBuddy->previous == NULL)
		{
			freeList[findBuddy->header - 5] = findBuddy->next;
			freeList[findBuddy->header - 5]->previous = NULL; 
			findBuddy->next = NULL; 
		}
		int greater = val - ptr; 
		if(greater > 0)
		{
			free->header += 1; 
			if(freeList[free->header - 5] != NULL)
			{
				freeList[free->header - 5]->previous = free; 
				free->next = freeList[free->header - 5]; 
				freeList[free->header-5] = free; 
				free->previous = NULL; 
				my_free(ptr); 
			}
			else
			{
				freeList[free->header - 5] =free; 
				free->previous = NULL; 
				free->next = NULL; 
			}
		}
		else
		{
			findBuddy->header += 1; 
			if(freeList[findBuddy->header-5] != NULL)
			{
				freeList[findBuddy->header - 5]->previous = findBuddy; 
				findBuddy->next = freeList[findBuddy->header-5]; 
				freeList[findBuddy->header-5] = findBuddy; 
				findBuddy->previous = NULL; 
				my_free(val); 
			}
			else
			{
				freeList[findBuddy->header-5] = findBuddy; 
				findBuddy->previous = NULL; 
				findBuddy->next = NULL; 
			}
		}
	}
	else
	{
		if(freeList[free->header-5] != NULL)
		{
			free->next = freeList[free->header - 5]; 
			freeList[free->header-5]->previous = free; 
			freeList[free->header-5] = free; 
			free->previous = NULL; 
		}
		else
		{
			free->next = NULL; 
			free->previous = NULL; 
			freeList[free->header - 5] = free; 
		}
	}
}
int power(int x)
{
	int i; 
	for(i = 5; i <= 30; i++)
	{
		if((1 << i) > x) //Left shift 1 = moving by powers of 2. 
		{
			return i; 
		}
	}
}
void *findFreeSpace(int size)
{
	int i; 
	for(i = size - 5; i < 26; i++)
	{
		if(freeList[i] != NULL)
		{
			if(freeList[i]->header == size)
			{
				freeList[i]->header |= 128; 
				void *temp = freeList[i]; 
				freeList[i] = freeList[i]->next;
				if(freeList[i] != NULL)
				{
					freeList[i]->previous = NULL; 
				}
				return temp; 
			}
			else{
				void *result = recursiveSplitter(i - 1, size - 5); 
				return result; 
			}
		}
	}
}
void * recursiveSplitter (int index, int baseCase)
{
	node *temp = freeList[index + 1]; 
	
	int size = ((1 << (index + 6)) / 2); 
	node *toSplit = temp + size; 
	
	if(temp->next != NULL)
	{
		freeList[index + 1] = temp->next; 
		temp->next = NULL; 
		freeList[index + 1]->previous = NULL; 
	}
	else{
		freeList[index + 1] = NULL; 
	}
	temp->next = toSplit;
	toSplit->previous = temp; 
	if(freeList[index] != NULL)
	{
		toSplit->next = freeList[index]; 
		freeList[index]->previous = toSplit; 
	}
	else{
		toSplit->next = NULL; 
	}
	freeList[index] = temp; 
	temp->previous = NULL;
	temp->header = index + 5; 
	toSplit->header = index + 5; 
	if(freeList[index]->header == baseCase+5)
	{
		freeList[index]->header |= 128;
		void *tmp = freeList[index]; 
		freeList[index] = freeList[index]->next; 
		if(freeList[index] != NULL)
		{
			freeList[index]->previous = NULL;
		}
		return tmp;
	}
	else{
		recursiveSplitter(index - 1, baseCase); 
	}
}
void dump_heap()
{
  node *cur;
  printf("head->");
  for(cur = base; cur != NULL; cur = cur->next) {
	printf("%s-> ", cur->header); 
    //printf("[%d:%d:%d]->", cur->occ, (char*)cur - (char*)base, cur->size);
    assert((char*)cur >= (char*)base && (char*)cur + cur->header <= (char*)sbrk(0)); // check that block is within bounds of the heap
    if(cur->next != NULL) assert(cur->next->previous == cur); // if not last block, check that forward/backward links are consistent
  }
  printf("NULL\n");
}












