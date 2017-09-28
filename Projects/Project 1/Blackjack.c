//Sai Konduru 

#include <stdio.h>
#include <stdlib.h>

int main() {
	srand((unsigned int)time(NULL)); 
	char *userChoice;
	int userAcePresent = 0; 
	int numUserAce = 0; 
	int numDealerAce = 0; 
	//Create a deck that has the right probablity. 
	int deck[15] = {}, i; 
	for(i = 0; i < 10; i++)
	{
		deck[i] = i; 
	}
	deck[10] = 10; deck[11] = 10; deck[12] = 10; deck[13] = 10; 
	deck[14] = 11; 
	
	//Deal the dealer's cards
	int value = rand() % 14 + 1; 
	int knownDealerNumber = deck[value]; 
	value = rand() % 14 + 1;
	int unknownDealerNumber = deck[value]; 
	
	//Check for aces in dealer's hand
	int acePresent = 0;
	if(knownDealerNumber == 11)
	{
		acePresent = 1;  
		numDealerAce++; 
	}
	if(unknownDealerNumber == 11)
	{
		acePresent = 1; 
		numDealerAce++;
	}
	
	//Print one of dealer's numbers
	int dealerTotal = knownDealerNumber + unknownDealerNumber; 
	printf("The dealer:\n ? + %d\n", knownDealerNumber);
	printf("\n");
	
	//Deal and print the user's cards. 
	value = rand() % 14 + 1; 
	int userNumber1 = deck[value];
	value = rand() % 14 + 1; 
	int userNumber2 = deck[value];
	int userTotal = userNumber1 + userNumber2;	
	
	//Change an ace to one if over 21 and ace is present. 
	//Count and save the aces present.
	if(userNumber1 == 11)
	{
		userAcePresent = 1; 
		numUserAce++; 
	}
	if(userNumber2 == 11)
	{
		userAcePresent = 1; 
		numUserAce++; 
	}
	if(userTotal > 21 && userAcePresent == 1)
	{
		printf("You are over 21 but you have an ace. Changing Ace to One...\n"); 
		userTotal = userTotal - 10; 
		if(userNumber1 == 11)
		{
			userNumber1 = 1; 
		}
		else if(userNumber2 == 11)
		{
			userNumber2 = 1; 
		}
		numUserAce--; 
		if(numUserAce == 0)
		{
			userAcePresent = 0; 
		}
		else{
			userAcePresent = 1; 
		}
	}
	printf("You:\n %d + %d = %d\n", userNumber1, userNumber2, userTotal); 
	printf("\n"); 
	
	//Check dealer for Bust. 
	if(dealerTotal > 21)
	{
		if(acePresent == 0)
		{
			if(userTotal <= 21)
			{
				printf("The Dealer's Total is %d! You Win!", dealerTotal);
			}
			else
			{
				printf("BUSTED!\n");
				printf("You busted. Dealer wins."); 
			}
		}
		else
		{
			dealerTotal = dealerTotal - 11 + 1;
			numDealerAce--; 
			if(numDealerAce == 0)
				acePresent = 0;
			else
				acePresent = 1; 
		}
		
	}
	
	//if the user got blackjack at the beganing. 
	if(userTotal == 21)
	{
		printf("BLACKJACK! YOU WIN!\n"); 
		return; 
	}
	
	//Check user for Bust.
	if(userTotal < 21)
	{
		//Ask user if they want to hit or stand. 
		printf("Would you like to hit or stand? ");  
		scanf("%s", &userChoice);
		printf("\n");
		
		//Give user another card if they choose to hit. 
		while(strcmp(&userChoice, "hit") == 0) 
		{
			printf("The dealer:\n ? + %d\n", knownDealerNumber);
			printf("\n"); 
			
			//Change an ace to one if total is over 21 and ace is present. 
			value = rand() % 14 + 1;
			userNumber1 = deck[value];
			int previousTotal = userTotal; 
			userTotal += userNumber1;
			if(userNumber1 == 11)
			{
				userAcePresent = 1;
				numUserAce++; 
			}
			if(userTotal > 21 && userAcePresent == 1)
			{
				printf("You are over 21 but you have an ace. Changing Ace to One...\n"); 
				userTotal = previousTotal + 1;  
				userNumber1 = 1; 
				numUserAce--; 
				if(numUserAce == 0)
				{
					userAcePresent = 0; 
				}
				else{
					userAcePresent = 1; 
				}
			}
			printf("You:\n %d + %d = %d ", previousTotal, userNumber1, userTotal);
			printf("\n"); 
			
			//Check user's cards for bust. 
			if(userTotal > 21)
			{
				printf("BUSTED!\n");
				printf("You busted. Dealer wins."); 
				return; 
			}
			else if(userTotal == 21)
			{
				printf("BLACKJACK! YOU WIN!\n"); 
				return; 
			}
			
			//Ask user to hit or stand. 
			printf("Would you like to hit or stand? ");
			scanf("%s", &userChoice);
			printf("\n");
		}
	}
	//Have dealer hit or stand. 
	while(dealerTotal < 17)
	{
		printf("\n");
		printf("The dealer choices to hit. \n");
		
		value = rand() % 14 + 1;
		knownDealerNumber += deck[value];
		if(deck[value] == 11)
		{
			acePresent = 1; 
		}
		dealerTotal += deck[value];
		if(dealerTotal > 21 && acePresent == 1)
		{
			dealerTotal = dealerTotal - 11 + 1;
			numDealerAce--; 
			if(numDealerAce == 0)
				acePresent = 0;
			else
				acePresent = 1; 
			knownDealerNumber = knownDealerNumber - 11 + 1; 
		}
		printf("The dealer: \n ? + %d\n", knownDealerNumber); 
		printf("\n"); 
	}
	
	//Check who wins if neither has gone over 21. 
	if(dealerTotal >= 17 && dealerTotal < 21 && strcmp(&userChoice, "stand") == 0)
	{
		int dealerDifference = 21 - dealerTotal; 
		int userDifference = 21 - userTotal; 
		if(userDifference > dealerDifference)
		{
			printf("The Dealer was closer to 21 with a total of %d. Dealer Wins!\n", dealerTotal); 
			printf("Your total was %d.", userTotal); 
		}
		else if(userDifference < dealerDifference)
		{
			printf("You are closer to 21 with a total of %d. YOU WIN!\n", userTotal);
			printf("The Dealer's total was %d.", dealerTotal); 
		}
		else{
			printf("The dealer and you had the same total, Dealer: %d User: %d Dealer Wins!", dealerTotal, userTotal);
		}
	}
	if(dealerTotal > 21)
	{
		printf("The Dealer Busted with a total of %d. YOU WIN!", dealerTotal); 
	}
	else if (dealerTotal == 21)
	{
		printf("The Dealer got Blackjack. Dealer Total = %d Dealer Wins!", dealerTotal); 
	}

}
