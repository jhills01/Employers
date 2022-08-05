//Tom Hills (jhills)
//Lab 2 - pgminfo

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

int main ()
{
	string firstReadChk;
	int columnTotal = 0, rowTotal = 0, white = 0, count = 0, pixel = 0;
	double pixelAverage = 0.0, pixelSum = 0.0;//, pixel = 0.0;
	
	cin >> firstReadChk;

	//ERROR CHECK: first read must == "P2"
	if(firstReadChk != "P2") {fprintf(stderr, "Bad PGM file -- first word is not P2\n"); return 1;}
	
	cin >> columnTotal >> rowTotal >> white;

	//ERROR CHECK: for negative column and row values
	if (columnTotal <= 0) {fprintf(stderr, "Bad PGM file -- No column specification\n"); return 1;}
	if (rowTotal <= 0) {fprintf(stderr, "Bad PGM file -- No row specification\n"); return 1;}
	
	//ERROR CHECK: read into "white" must == 255. 
	if (white != 255) {fprintf(stderr, "Bad PGM file -- 255 not after number of rows and columns\n"); return 1;}
	
	//loop for reading in pixel values and updating sum of pixels. 
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
		pixelSum += pixel;
	}

	pixelAverage = pixelSum/count;

	//final print statements
	printf("# Rows:   %9d\n", rowTotal);
	printf("# Columns:%9d\n", columnTotal);
	printf("# Pixels: %9d\n", count);
	printf("Avg Pixel:%9.3lf\n", pixelAverage);

	return 0;
}

