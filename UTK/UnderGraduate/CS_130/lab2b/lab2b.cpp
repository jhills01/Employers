// John (Tom) Hills
// lab2b
// Cryptography Scheme

#include <cstdlib>
#include <cstdio>
#include <string>

using namespace std;

int main (int argc, char *argv[])
{
	FILE *infile;
	int filesize;
	char *data;

	if (argc != 4)
	{
		//user didn't input correct number of command line arguments. print error msg
		printf("argc != 4. Please re-enter correct number of command line arguments."); 		
		return -1;

	}

	infile = fopen(argv[1], "rb");

	if (infile == NULL)
	{
		//file didn't open, print error msg.
		perror(argv[1]);
		return -2;
	}
	
	//figure out how many blocks are in the file to properly allocate the array
	fseek(infile, 0, SEEK_END); //SEEK_END means it points to the end of the file.
	//error check for fseek
	if (fseek(infile, 0, SEEK_END) != 0)
	{
		printf("\nERROR: fseek did not successfully move stream cursor to the end of file.\n");
		return -3;
	}
	
	filesize = ftell(infile);  
	
	//divide filesize by 8 to get the number of blocks in encrypted file (1 block = 1 output character)
	filesize /= 8;

	data = new char [filesize];

	//returning stream position to the beginning of file.
	fseek(infile, 0, SEEK_SET);
	//error check for fseek
	if (fseek(infile, 0, SEEK_SET) != 0)
	{
		printf("\nERROR: fseek did not successfully reset stream cursor to beginning of file.\n");
		return -4;
	}
		
	//variables for decoding each 8 byte blocks from encrypted file
	int decodedIndex = 0;
	char decodedChar;
	char *byteBlock = new char[8];
	size_t bytes_read;
	//werror insists that I both initialize and use these four ints BEFORE I get to the FOR loop even though they are used within it. Odd..	
	int indexFour = 0, indexFive = 0, indexSix = 0, indexSeven = 0;  
	indexFour++; indexFive++; indexSix++; indexSeven++;

	//reading in file blocks, decoding data and index, and then assigning properly indexed data to "data".
	for (int i = 0; i < filesize-1; i++) 
	{
		bytes_read = fread(byteBlock, 1, 8, infile);
		//error check for fread
		if (bytes_read != 8) {printf("ERROR: fread did not read in 8 bytes for block %d.",i);}
		decodedChar = (byteBlock[0])^(*argv[2]);
		
		//stores the 4 bytes of encoded index into ints where indexFour is least significant byte.
		indexFour = byteBlock[4]; indexFive = byteBlock[5]; indexSix = byteBlock[6]; indexSeven = byteBlock[7];

		//Changes the endianness of the encoded 4 bytes (from little to big) and adds up to the full encoded int. XOR's with converted argv[3] to decode correct index. 	
		decodedIndex = (byteBlock[4] + (byteBlock[5] << 8) + (byteBlock[6] << 16) + (byteBlock[7] << 24)) ^ (atoi(argv[3]));
	        
		//error check to ensure argv[3] was entered correctly from user.	
		if (decodedIndex < 0 || decodedIndex > filesize)
		{	
			printf("\nERROR: Invalid nkey. Please provide a valid nkey command line argument\n\n");
			return -5;
		}	
		
		data[decodedIndex] = decodedChar;
	}	
	
	//Printing decoded message	
	printf("\n");
	for (int i = 0; i < filesize; i++)
	{
		printf("%c",data[i]);
	}
	printf("\n\n");

	delete [] data;
	delete [] byteBlock;

	fclose(infile);

	return 0;
}
