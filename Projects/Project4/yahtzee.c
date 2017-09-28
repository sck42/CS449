#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int dice[5] = {0}; 
int totalUpperScore = 0; 
int totalLowerScore = 0; 
int upperScore[6]; 
int lowerScore[7]; 
int bonus = 0; 

//Open and use the dice driver. 
int diceRoll()
{
	unsigned int roll; 
	int x = open("/dev/dice", O_RDONLY); 
	int y = read(x, &roll, sizeof(int)); 
	return roll; 
}

//helper method for qsort. 
int comp(const void * first, const void * second)
{
	return (*(int *)first - *(int *)second); 
}

//Ask user which dice reroll is required then reroll the ones they want to reroll. 
void reroll()
{
	char input[11], *userDice[11]; 
	
	//Ask user which dice to reroll and store their input in an array. 
	printf("\n"); 
	printf("\n"); 
	printf("Which dice to reroll? ");
	fgets(input, sizeof(input), stdin); 
	
	//Split input using the space character as a delimiter. 
	int i = 0; 
	userDice[0] = strtok(input, " "); 
	while(userDice[i] != NULL)
	{
		//Set the dice to reroll to 1. 
		int diceToReroll = atoi(userDice[i]) - 1; 
		dice[diceToReroll]= diceRoll(); 
		i++; 
		userDice[i] = strtok(NULL, " "); 
	}
	
	//Print out the new rerolls. 
	printf("Your Rerolls are: \n"); 
	for(i = 0; i < 5; i++)
	{
		printf("%d ", dice[i]); 
	}
	printf("\n"); 
}
int validSelection(char upper_lower){
	char input[20];
	int selection;

	//Keep asking for selection until appropriate selection is given. 
	while(1)
	{
		printf("Selection: ");
		fgets(input, sizeof(input), stdin);
		selection = atoi(input);

		if(upper_lower == 'u')
		{
			if(selection < 0 || selection > 6)
				printf("Please select a number between 1 and 6.");

			else if(upperScore[selection - 1] == -1)
				return selection;

		    printf("Invalid selection. Upper category #%d already assigned", selection);
		}
		else if(upper_lower == 'l')
		{
		    if(selection < 0 || selection > 7)
				printf("Please select a number between 1 and 7.");

		    else if(lowerScore[selection - 1] == -1)
				return selection;

		    printf("Invalid selection. Lower category #%d already assigned", selection);
		}
	}
}
void upperSelection()
{
	char input[20];
	printf("Place dice into: \n"); 
	printf("1) Ones\n"); 
	printf("2) Twos\n");
	printf("3) Threes\n"); 
	printf("4) Fours\n"); 
	printf("5) Fives\n"); 
	printf("6) Sixes\n"); 
	
	printf("\n");   
	int selection = validSelection('u'); 
	printf("\n");
	printf("\n");
	int sum = 0; 
	int i = 0; 

	//Sum up all the dice that equal selection. 
	for(i = 0; i < 5; i++)
	{
		if(dice[i] == selection)
			sum += selection; 
	}
	
	//Store the sum. 
	upperScore[selection - 1] = sum; 
	totalUpperScore += sum; 
	
	//Check if upper total is greater than 62 to check for bonus. 
	if(totalUpperScore > 62)
	{
		bonus = 35; 
	} 
}

void lowerSelection()
{
	printf("Place dice into:\n");
    printf("1) Three of a kind\n");
    printf("2) Four of a kind\n");
    printf("3) Full House\n");
    printf("4) Small Straight\n");
    printf("5) Large Straight\n");
    printf("6) Yahtzee\n");
    printf("7) Chance\n\n");
	int selection = validSelection('l'); 
	printf("\n");
	printf("\n");
	int frequency[7] = {0}; 
	int sum = 0; 
	
	int i = 0;
	int row = 0;
	for(i = 0; i < 5; i++)
	{
		//Calculate Frequency
		int roll = dice[i]; 
		frequency[roll] += 1;  
		
	}
	for(i = 0; i < 4; i++)
	{
		//Find straight length
		if(dice[i + 1] == (dice[i] + 1))
		{
			row++;
		}
	}
	row++; 
	//Three of a kind
	if(selection == 1)
	{
		for(i = 0; i < 7; i++)
		{
			//Look for frequency of 3 or greater, if found sum up all dice. 
			if(frequency[i] >= 3)
			{
				int j = 0; 
				for(j = 0; j < 5; j++)
				{
					sum += dice[j]; 
				} 
				break;
			}
			sum = 0; 
		}
		lowerScore[0] = sum; 
	}
	//Four of a kind 
	else if(selection == 2)
	{
		for(i = 1; i < 7; i++)
		{
			//Look for frequency of 4 or greater, if found sum up all dice. 
			if(frequency[i] >= 4)
			{
				int j = 0; 
				for(j = 0; j < 5; j++)
				{
					sum += dice[j]; 
				} 
				break;
			}
			sum = 0; 
		}

		lowerScore[1] = sum;
	}
	//Full House 
	else if(selection == 3)
	{
		//Check if the first two are the same followed by the last three are the same. 
		if(dice[0] == dice[1] && (dice[2] == dice[3]) && (dice[3] == dice[4]))
		{
			sum = 25; 
		}
		//Check if the first three are the same followed by the last two are the same. 
		else if((dice[0] == dice[1]) && (dice[1] == dice[2]) && dice[3] == dice[4])
		{
			sum = 25; 
		}
		else
		{
			sum = 0; 
		}
		lowerScore[2] = sum;
	}
	//Small Straight
	else if(selection == 4)
	{
		//Check if 4 or greater are in order. 
		if(row >= 4)
		{
			sum = 30; 
		}
		else 
		{
			sum = 0; 
		}
		lowerScore[3] = sum; 
	}
	//Large Straight
	else if(selection == 5)
	{
		//Check if all 5 are in order. 
		if(row == 5)
		{
			sum = 40; 
		}
		else
		{
			sum = 0;
		}
		lowerScore[4] = sum;
	}
	//Yahtzee
	else if(selection == 6)
	{
		//Check frequence array for 5. If found add 50 to score. 
		for(i = 0; i < 7; i++)
		{
			if(frequency[i] == 5)
			{
				sum = 50; 
				break; 
			}	
			sum = 0;
		}
		lowerScore[5] = sum;
	}
	//Chance 
	else if(selection == 7)
	{
		//Calculate the sum of all the dice for Chance. 
		for(i = 0; i < 5; i++)
		{
			sum += dice[i]; 
		}
		lowerScore[6] = sum;
	}
	totalLowerScore += sum;  
}
void displayScore()
{
	//Print Ones score 
	if(upperScore[0] == -1)
	{
		printf("Ones: \t\t\t\t"); 
	}
	else
	{
		printf("Ones: %d\t\t\t\t", upperScore[0]);
	}
	//Print Fours score
	if(upperScore[3] == -1)
	{
		printf("Fours: \n"); 
	}
	else
	{
		printf("Fours: %d \n", upperScore[3]);
	}
	
	//Print Twos score
	if(upperScore[1] == -1)
	{
		printf("Twos: \t\t\t\t"); 
	}
	else
	{
		printf("Twos: %d\t\t\t\t", upperScore[1]);
	}
	
	//Print Fives score
	if(upperScore[4] == -1)
	{
		printf("Fives: \n"); 
	}
	else
	{
		printf("Fives: %d \n", upperScore[4]);
	}
	
	//Print Threes score
	if(upperScore[2] == -1)
	{
		printf("Threes: \t\t\t"); 
	}
	else
	{
		printf("Threes: %d\t\t\t", upperScore[2]);
	}
	
	//Print Sixes score
	if(upperScore[5] == -1)
	{
		printf("Sixes: \n"); 
	}
	else
	{
		printf("Sixes: %d \n", upperScore[5]);
	}
	//Print bonus
	printf("Upper Section Bonus: %d\n", bonus);  
	
	printf("\nLower Section Score:\n");
	//Print Three of a kind
	if(lowerScore[0] == -1)
	{
		printf("Three of a kind: \t\t"); 
	}
	else
	{
		printf("Three of a kind: %d\t\t", lowerScore[0]);
	}
	
	//Print Four of a kind
	if(lowerScore[1] == -1)
	{
		printf("Four of a kind: \n"); 
	}
	else
	{
		printf("Four of a kind: %d\n", lowerScore[1]);
	}
	
	//Print Small Straight
	if(lowerScore[3] == -1)
	{
		printf("Small Straight: \t\t"); 
	}
	else
	{
		printf("Small Straight: %d\t\t", lowerScore[3]);
	}

	//Print Large Straight 
	if(lowerScore[4] == -1)
	{
		printf("Large Straight: \n"); 
	}
	else
	{
		printf("Large Straight: %d\n", lowerScore[4]);
	}
	
	//Print Full House 
	if(lowerScore[2] == -1)
	{
		printf("Full House: \t\t\t"); 
	}
	else
	{
		printf("Full House: %d\t\t\t", lowerScore[2]);
	}
	
	//Print Yahtzee 
	if(lowerScore[5] == -1)
	{
		printf("Yahtzee: \n"); 
	}
	else
	{
		printf("Yahtzee: %d\n", lowerScore[5]);	
	}
	
	//Print Chance
	if(lowerScore[6] == -1)
	{
		printf("Chance: \n"); 
	}
	else{
		printf("Chance: %d\n", lowerScore[6]); 
	}
}
int main()
{
	int turns = 13; 
	int i = 0; 
	//Set up score arrays. 
	for(i = 0; i < 6; i++)
	{
		upperScore[i] = -1;
	}
	for(i = 0; i < 7; i++)
	{
		lowerScore[i] = -1; 
	}
	//srand(time(NULL)); 
	while(turns > 0)
	{
		//Print turn count. 
		if(turns == 1)
		{
			printf("This is your last turn! \n"); 
		}
		else if(turns == 2)
		{
			printf("You have one more turn! \n"); 
		}
		else 
		{
			printf("You have %d turns left! \n", turns - 1); 
		}
		
		//Generate 5 dice rolls using dice driver. 
		int i; 
		for(i = 0; i<5; i++)
		{
			dice[i] = diceRoll(); //(rand() % 6) + 1; 
		}
		
		//Print the current rolls. 
		printf("Your rolls are: \n"); 
		for(i = 0; i < 5; i++)
		{
			printf("%d ", dice[i]); 
		}
		
		//Ask if user wants to reroll and reroll the appropriate dice. 
		reroll(); 
		
		//Ask if user wants to reroll and reroll the appropriate dice.  
		reroll(); 
		
		//Sort the array. 
		qsort(dice, 5, sizeof(int), comp); 
		
		//Ask which type of score to go with. 
		printf("Place dice into: \n"); 
		printf("1) Upper Section \n"); 
		printf("2) Lower Section \n"); 
		printf("\n"); 
		printf("\n");
		printf("Selection? "); 
		char input[20]; 
		fgets(input, sizeof(input), stdin);
		int selection = atoi(input); 
		printf("\n");
		printf("\n");
		if(selection == 1)
		{
			//Use upper Score. 
			upperSelection(); 
		}		
		else if(selection == 2)
		{
			//Use lower Score. 
			lowerSelection(); 
		}
		//Print current total score.
		printf("Your score so far is: %d\n", totalUpperScore + totalLowerScore + bonus); 
		//Display Score. 
		displayScore(); 
		
		//Next Turn
		turns--; 
	}
	
	printf("\n\nCONGRATULATIONS! YOUR SCORE IS %d\n THANK YOU FOR PLAYING YAHTZEE!!!\n", totalUpperScore + totalLowerScore + bonus);
	
}
