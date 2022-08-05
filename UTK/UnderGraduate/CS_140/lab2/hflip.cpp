//Tom Hills (jhills)
//Lab 2 - hflip

#include <cstdlib>
#include <cstdio>
#include <iostream>//DON'T USE "COUT"!!! CIN is ok though
#include <string>
#include <vector>

using namespace std;

int main ()
{
	string firstReadChk;
	int columnTotal = 0, rowTotal = 0, white = 0, count = 0, pixel = 0, column, row, swap;
	vector <int> pixels;
	
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
	
	//loop for reading in pixel values and updating sum of pixels. 
	while (1)
	{	
		cin >> pixel;

		//ERROR CHECK: ensures we didn't get a bad read right before eof
		if ((count < (columnTotal * rowTotal)) && (cin.eof())) {fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", count); return 1;}
		
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
		
		//stores pixel values that pass error checking into 1-d pixels vector
		pixels.push_back(pixel); //
	}

	//rearranges 1-d pixels vector to its horizontally flipped counterpart and then writes to stdout
	count = 0;

	for (row = 0; row < rowTotal; row++)
	{
		//horizontally flips pixel values in pixels vector by traversing a row via column value	
		for (column = 0; column < (columnTotal/2); column++)
		{
			//"row * columnTotal" yields the lowest index in a row, and "+ column" increments this index
			swap = pixels[row * columnTotal + column];	
			//On right-hand side: "columnTotal * (row + 1) - 1" yields the highest index in a row, and "- column" decrements this index
			pixels[row * columnTotal + column] = pixels[(columnTotal * (row + 1) - 1) - column];
			pixels[(columnTotal * (row + 1) - 1) - column] = swap;
		}
	}
	
	for (int i = 0; i < pixels.size(); i++)
	{
		//printing rowTotal * columnTotal table to output pgm file
		if ((i + 1) % columnTotal == 0 && i != 0) {fprintf(stdout, "%d\n", pixels[i]);}// "i + 1" b/c columnTotal is not zero indexed like i. 
		else {fprintf(stdout, "%d ", pixels[i]);}
	}

	return 0;
}

