//Sai Konduru 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <time.h>
#define MYPORT  51259
#define MAX_THREAD_COUNT 3 

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
void *handleGET(void *p); 

//Struct will be used to pass two arguments to handleGET method. 
typedef struct Argument{
	int *connfd;
	struct sockaddr_in addr; 
}arg; 

int main()
{
	int threadCount = 0; 
	pthread_t thread; 
	int id; 
	//Create socket and addr. 
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
	
	//Bind the socket 
	if(bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror(NULL);  
	}

	//Keep creating threads until max thread count is reached. 
	//Change thread count to whatever number. 
	//It is 3 for testing purposes. 
	while(threadCount < MAX_THREAD_COUNT)
	{
		printf("Server Listening...\n");
		if(listen(sfd, 10) == -1)
		{
			perror(NULL);   
		}
		int len = sizeof(addr); 
		//Accept the connection 
		int connfd = accept(sfd, (struct sockaddr *)&addr, &len); 
		if(connfd == -1)
		{
			perror(NULL);  
		}
		
		//create the argument variable. 
		arg* a = malloc(sizeof(struct Argument)); 
		a->connfd = &connfd; 
		a->addr = addr; 
		
		//Create thread and call handleGET method. 
		id = pthread_create(&thread, NULL, handleGET, (void *)a); 
		pthread_join(thread, NULL); 
		threadCount++; 
		printf("File Sent\n\n"); 
		//free(a); 
	}
	
	//Close the socket and end the server connection. 
	close(sfd); 
}
void *handleGET(void *p)
{
	//Grab the argument and set up local variables. 
	arg *a = p; 
	int *connfd = a->connfd;
	struct sockaddr_in addr = a->addr; 
	char file[1024]; 
	char input[4096];
	FILE *userFile; 
	char *msgToSend; 
	time_t currentTime; 
	struct tm *localTime;  
	char *fileContents; 
	char get[1000]; 
	unsigned short port = ntohs(addr.sin_port);
	char *ip = inet_ntoa(addr.sin_addr); 
	int myport = MYPORT; 
	FILE *output; 
	
	//Call receive and store in input. 
	if(recv(*connfd, input, 4096, 0) == -1)
	{
		perror(NULL);  
	} 

	//Check for a GET in the first line. 
	if(strncmp(input, "GET ", 4) == 0)
	{
		
		
		int i = 0;
		//Store the first line to edit to file later. 
		for(i = 0; i< 1000; i++)
		{
			get[i] = input[i]; 
			if((input[i] == '\n') || (input[i] == '\r'))
			{
				break; 
			}
		}
		
		//Get the file name from input
		for(i = 0; i < 1000; i++)
		{
			if((input[i+5] == '\n') || (input[i+5] == '\r') || (input[i+5] == ' ') || (input[i+5] == '\0'))
			{
				break;
			}
			file[i] = input[i+5]; 
		}
		
		//Terminate file with null character. 
		//Look for file with the name given. 
		file[i] = '\0'; 
		userFile = fopen(file, "r"); 
		
		//If no file is found set msgToSend to the error message. 
		if(userFile == NULL)
		{
			msgToSend = (char *)malloc(30 * sizeof(char)); 
			strcpy(msgToSend, "\nHTTP/1.1 404 Not Found\n"); 
		}
		else
		{
			//If file is found prepare the msgToSend with all necessary information. 
			
			//Find file size and number of digits in the file length number.
			int fileSize; 
			fseek(userFile, 0, SEEK_END); 
			fileSize = ftell(userFile); 
			fseek(userFile, 0, SEEK_SET); 
			int numDigits = (int)(log10(fileSize) + 1); 

			//Read the entire file and store in fileContents. 
			fileContents = (char *)malloc((fileSize + 1) * (sizeof(char))); 
			fread(fileContents, 1, fileSize, userFile); 
			
			//allocate space of msgToSend
			msgToSend = (char *)malloc((130 * sizeof(char)) + (numDigits* sizeof(char)) + (fileSize * (sizeof(char))));
			
			strcpy(msgToSend, "\nHTTP/1.1 200 OK\n"); 
			
			//Add time to message 
			currentTime = time(NULL); 
			localTime = localtime(&currentTime); 
			char strTime[40]; 
			strftime(strTime, 40, "Date: %A, %d %B %Y %X %Z\n", localTime); 
			strcat(msgToSend, strTime); 
			
			//Content length
			strcat(msgToSend, "\nContent-Length: "); 
			char temp[numDigits]; 
			sprintf(temp, "%d", fileSize); 
			strcat(msgToSend, temp); 
			
			//Add connection, content-type and rest of file. 
			strcat(msgToSend, "\nConnection: close\nContent-Type: text/html\n\n");
			strcat(msgToSend, fileContents); 
			strcat(msgToSend, "\n"); 
			//free(fileContents); 
			fclose(userFile);
		}
		 
	}
	//Send the msgToSend to the client. 
	send(*connfd, msgToSend, strlen(msgToSend), 0); 
	

	//Synchronize file editing. 
	pthread_mutex_lock(&mutex); 
	output = fopen("stats.txt", "a"); 
	fprintf(output, "\n%s\nHost: %s:%d\nClient: %s:%hu\n", get, ip, myport, ip, port); 
	fclose(output); 
	pthread_mutex_unlock(&mutex); 
	close(*connfd); 
	//free(msgToSend); 
}