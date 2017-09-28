#include <stdio.h>
#include <stdlib.h>

struct Node {
	int grade;
	struct Node *next; 
};
int main()
{
	int input = 0;
	int total = 0;
	int numEntries = 0; 
	struct Node *head; 
	struct Node *tail; 
	head = (struct Node *)malloc(sizeof(struct Node));  
	tail = head;  
	head->next = NULL; 
	while(input != -1)
	{
		printf("Please enter a grade. ");
		scanf("%d", &input);
		tail->grade = input;
		tail->next = (struct Node *)malloc(sizeof(struct Node));
		tail = tail->next; 
	}
	tail->next = NULL; 
	
	struct Node *currentNode = head->next; 
	struct Node *temp = head->next; 
	total += head->grade; 
	while(currentNode != NULL)
	{ 
		total += currentNode->grade; 
		numEntries++;
		temp = currentNode; 
		currentNode = currentNode->next; 
		free(temp);
	}
	printf("Your average is %d", (total + 1)/(numEntries-1)); 
}