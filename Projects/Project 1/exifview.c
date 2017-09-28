//Sai Konduru

#include <stdio.h>

//Define header struct to contain a member to represent each values 
//Adds to 20 bytes
struct Header{
	unsigned short fileMarker; 
	unsigned short app1Marker;
	unsigned short appMarkerLength;
	char exif[4]; 
	unsigned short nulMarker; 
	char endianness[2]; 
	unsigned short version; 
	unsigned int offset; 
}head; 

//Define tag struct to contain a member to represent each tag values
//Adds to 12 bytes
struct Tag{
  unsigned short id;
  unsigned short type;
  unsigned int numItems;
  unsigned int offset;
}tag;

int main(int argc, char **argv)
{
	FILE *f;
	unsigned short count, newCount;
	char str[100]; 
	int i = 0;
	int j = 0; 
	int currentOffset, newOffset;	
	
	//Check if a file has been entered. 
	if(argc < 2 || argc > 2)
	{
		printf("Please run again with a file."); 
		return 0;
	}
	
	//Open and read exif header. 
	f = fopen(argv[1], "rb");
	//Check for invalid file. 
	if(f == NULL)
	{
		printf("Invalid file entered!");
		return 0; 
	}
	fread(&head, sizeof(head), 1, f); 
	
	//Check for files that don't have App1 marker 
	if(head.app1Marker != 0xE1FF)
	{
		printf("Sorry this program does not support a file with your app marker.");
		return 0; 
	}
	
	//Check for files that don't contain Exif data.
	if(strcmp(head.exif, "Exif") != 0)
	{
		printf("This file does not contain Exif data."); 
		return 0; 
	}
	
	//Check for files that are are not Little endian. 
	if(strcmp(head.endianness, "II*") != 0)
	{
		printf("This program does not support Big Endian.");
		return 0; 
	}
	
	//Read in the number of tags. 
	fread(&count, sizeof(short), 1, f);

	while(i < count)
	{
		fread(&tag, sizeof(tag), 1, f);
		currentOffset = ftell(f); 
		
		//Manufacturer String
		if(tag.id == 0x010F)
		{
			fseek(f, tag.offset + 12, SEEK_SET); 
			fread(&str, sizeof(char), tag.numItems, f);
			printf("Manufacture: \t%s\n", str); 
		}
		//Camera Model String
		else if(tag.id == 0x0110)
		{ 
			fseek(f, tag.offset + 12, SEEK_SET); 
			fread(&str, sizeof(char), tag.numItems, f);
			printf("Model: \t\t%s\n", str);
		}
		//Exif block
		else if(tag.id == 0x8769)
		{
			fseek(f, tag.offset + 12, SEEK_SET);
			fread(&newCount, sizeof(short), 1, f); 
			struct Tag tag2; 
			unsigned int num1;
			unsigned int num2; 
			
			while(j < newCount)
			{
				fread(&tag2, sizeof(tag2), 1, f);
				newOffset = ftell(f); 
				//Width
				if(tag2.id == 0xa002)
				{
					printf("Width: \t\t%d pixels\n", tag2.offset);
				}
				//Height
				else if(tag2.id == 0xa003)
				{
					printf("Height: \t%d pixels\n", tag2.offset); 
				}
				//ISO
				else if(tag2.id == 0x8827)
				{
					printf("ISO: \t\tISO %d\n", tag2.offset); 
				}
				//Exposure Time
				else if(tag2.id == 0x829a)
				{
					fseek(f, tag2.offset + 12, SEEK_SET); 
					fread(&num1, sizeof(num1), 1, f); 
					fread(&num2, sizeof(num2), 1, f); 
					printf("Exposure Time: \t%d/%d second\n", num1, num2); 
				}
				//F-Stop
				else if(tag2.id == 0x829d)
				{
					fseek(f, tag2.offset + 12, SEEK_SET); 
					fread(&num1, sizeof(num1), 1, f); 
					fread(&num2, sizeof(num2), 1, f); 
					printf("F-stop: \tf/%.1f\n", (double)num1/num2);
				}
				//Focal Length
				else if(tag2.id == 0x920a)
				{
					fseek(f, tag2.offset + 12, SEEK_SET); 
					fread(&num1, sizeof(num1), 1, f); 
					fread(&num2, sizeof(num2), 1, f); 
					printf("Focal Length: \t%.0f mm\n", (double)num1/num2);
				}
				//Date
				else if(tag2.id == 0x9003)
				{
					fseek(f, tag2.offset + 12, SEEK_SET); 
					fread(&str, sizeof(char), tag2.numItems, f); 
					printf("Date Taken: \t%s\n", str); 
				}
				fseek(f, newOffset, SEEK_SET); 
				j++;
			} 
		}
		i++; 
		fseek(f, currentOffset, SEEK_SET); 
	}
}
