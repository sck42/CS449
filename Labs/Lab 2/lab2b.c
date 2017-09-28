#include <stdio.h> 

int main(void){

	float x; 
	printf("Please enter a number: ");
	scanf("%f", &x);
	printf("%.2f\n", x);   
	printf("Here is your weight on other planents: \n");
	printf("Mercury \t %.2f lbs \n", x * .38); 
	printf("Venus \t\t %.2f lbs \n", x*.91); 
	printf("Mars \t\t %.2f lbs \n", x*.38); 
	printf("Jupiter \t %.2f lbs \n", x*2.54); 
	printf("Saturn \t\t %.2f lbs \n", x *1.08); 
	printf("Uranus \t\t %.2f lbs \n", x*.91); 
	printf("Neptune \t %.2f lbs \n", x*1.19); 	
	return(0); 
}
 
