//Sai Konduru

#include <stdio.h>
int main(int argc, char** argv)
{
	FILE *f; 
	//Check if a file has been entered. 
	if(argc < 2 || argc > 2)
	{
		printf("Please run again with a file."); 
		return -1;
	}

	//Open 
	f = fopen(argv[1], "r");
	if(f == NULL)
	{
		printf("File could not be found."); 
		return -1; 
	}
	
	//Read and store first character of file
	char input; 
	fseek(f, 0, SEEK_SET);
	fread(&input, sizeof(char), 1, f);
	 
	//Track current offset
	int endoffset = ftell(f); 
	int startoffset = 0; 
	while(feof(f) == 0) 
	{ 
		//Count number of readable characters 
		while((input > 31 && input < 127) || input == 9)
		{ 
			fread(&input, sizeof(char), 1, f);
		} 
		endoffset = ftell(f); 
		
		//If more than 4 readable characters 
		//Create array that can hold the number of readable characters
		//Read and store the readable characters 
		//Print readable characters
		if(endoffset - startoffset >= 4)
		{
			char output[endoffset - startoffset + 1]; 
			fseek(f, startoffset - 1, SEEK_SET); 
			fread(&output, sizeof(output), 1, f); 
			output[endoffset - startoffset] = '\0';
			printf("%s\n", &output); 
		} 
		
		//Increment to next character
		fread(&input, sizeof(char), 1, f); 
		endoffset = ftell(f);
		startoffset = endoffset;
	}
	fclose(f); 
	return 0; 
}