//Tom Hills (jhills)
//Lab 2 - neg

#include <cstdlib>
#include <cstdio>
#include <iostream>//DON'T USE "COUT"!!! CIN is ok though.  
#include <string>

using namespace std;

int main ()
{
	string firstReadChk;
	int columnTotal = 0, rowTotal = 0, white = 0, count = 0, pixel = 0;
	
	cin >> firstReadChk;

	//ERROR CHECK: first read from input pgm file must == "P2"
	if(firstReadChk != "P2") {fprintf(stderr, "Bad PGM file -- first word is not P2\n"); return 1;}
	
	cin >> columnTotal >> rowTotal >> white;

	//ERROR CHECK: for negative column and row values from input pgm file
	if (columnTotal <= 0) {fprintf(stderr, "Bad PGM file -- No column specification\n"); return 1;}
	if (rowTotal <= 0) {fprintf(stderr, "Bad PGM file -- No row specification\n"); return 1;}
	
	//ERROR CHECK: read from input pgm file into "white" must == 255. 
	if (white != 255) {fprintf(stderr, "Bad PGM file -- No 255 following the rows and columns\n"); return 1;}
	
	//prints "header" entries to output pgm file
	fprintf(stdout, "P2\n%d %d\n255\n", columnTotal, rowTotal);
	
	//loop for reading in pixel values and calculating/outputting neg pixel 
	while (1)
	{	
		cin >> pixel;

		//ERROR CHECK: ensures good read into pixel before eof	
		if (!cin.good() && !cin.eof()) {fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", count); return 1;}

		//ERROR CHECK: ensures that more data was NOT after pixel data from input pgm file
		if ((count > (columnTotal * rowTotal)) && cin.eof()) {fprintf(stderr, "Bad PGM file -- Extra stuff after the pixels\n"); return 1;}
		
		//Exit while loop if cin reads end of file (eof) 
		if (cin.eof()) {break;}
		
		//ERROR CHECK: pixel value must be between 0 and 255
		if ((pixel < 0 || pixel > 255) && (count < (columnTotal * rowTotal))) 
		{
			fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", count); return 1;
		}	

		count++;
		
		//calculate negative of pixel and print to output pgm file
		pixel = 255 - pixel;
		if (count % columnTotal == 0) {fprintf(stdout, "%d\n", pixel);}
		else {fprintf(stdout, "%d ", pixel);}
	
	}

	return 0;
}

