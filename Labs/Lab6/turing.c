//Sai Konduru 

#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<stdio.h>
#define 	MYPORT  51259

int main()
{
	int sfd = socket(PF_INET, SOCK_STREAM, 0); 
	if(sfd == -1)
	{
		perror(NULL); 
	}
	struct sockaddr_in addr; 
	memset(&addr, 0, sizeof(addr)); 
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(MYPORT); 
	addr.sin_addr.s_addr = INADDR_ANY; 
	if(bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror(NULL);  
	}
	if(listen(sfd, MYPORT) == -1)
	{
		perror(NULL);   
	}
	int connfd = accept(sfd, NULL, NULL); 
	if(connfd == -1)
	{
		perror(NULL);  
	}
	char buffer[1024]; 
	//strcpy(buffer, "Hello there!"); 
	//send(connfd, buffer, strlen(buffer), 0);
	char input[1024]; 
	if(recv(connfd, input, 1024, 0) == -1)
	{
		perror(NULL);  
	}		

	while(strncmp(input, "Later", 5) != 0)
	{
		if(strncmp(input, "hi", 2) == 0)
		{
			
			strcpy(buffer, "Hello User.\n"); 
			send(connfd, buffer, strlen(buffer), 0);
		}
		else if(strncmp(input, "What's up?", 9) == 0)
		{
			printf("Nothing much how about you?\n"); 
			strcpy(buffer, "Nothing much how about you?\n"); 
			send(connfd, buffer, strlen(buffer), 0);
		}
		else if(strncmp(input, "I'm fine, just bored.", 21) == 0)
		{
			printf("What up homie?\n"); 
			strcpy(buffer, "Oh, let me entertain you.\n"); 
			send(connfd, buffer, strlen(buffer), 0);
		}
		else
		{
			strcpy(buffer, "Sorry I didn't understand you.\n");
			send(connfd, buffer, strlen(buffer), 0);
		}
		if(recv(connfd, input, 1024, 0) == -1)
		{
			printf("TEST"); 
			perror(NULL);  
		} 	
	}
	strcpy(buffer, "Bye Bye!\n"); 
	send(connfd, buffer, strlen(buffer), 0); 
	close(connfd); 
	close(sfd); 
}