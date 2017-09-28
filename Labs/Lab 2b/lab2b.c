#include <stdio.h>
#include <stdlib.h>

int* search(int* begin, int* end, int needle); 

int main(int argc, char **argv)
{
	int num; 
	int nums[10], i;
	int *found = NULL; 
	
	if(argc != 2){
		printf("Usage: search <number> \n"); 
		return 1;
	}
	num = atoi(argv[1]); 
	for(i = 0; i < 10; i++){
		nums[i] = 1 << i; 
	}
	found = search(nums, &nums[9], num); 
	if(found){
		printf("Number %d found in index %d. \n", num, found - nums); 
	}
	else{
		printf("Number %d was not found. \n", num); 
	}
	return 0; 
}
int* search(int* begin, int* end, int needle)
{
	int middle;
	middle = (end - begin)/2; 
	
	if(begin > end){
		return NULL;
	} 
	if(begin[middle] == needle)
	{
		return &begin[middle]; 
	}
	else if(begin[middle] > needle)
	{
		return search(begin, &begin[middle - 1], needle);
	}
	else if(begin[middle] < needle)
	{
		return search(&begin[middle + 1], end, needle); 
	} 
}
