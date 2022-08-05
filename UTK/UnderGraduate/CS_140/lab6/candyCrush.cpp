// Tom Hills (jhills)
// Lab 6 - candyCrush

#include <string>
#include <list>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "candyCrush.h"

using namespace std;

candyCrush::candyCrush(string inputFile) : score(0)
{
	ifstream inFile;
	string fileLineStr, flavor, badReadStr;
	istringstream buffer;
	int seed, cumulativeProbs = 0, probability, points;

	inFile.open(inputFile.c_str());

	//ERROR CHECK: ensures file opened correctly
	if (inFile.fail()) {fprintf(stderr, "%s: No such file or directory\n", inputFile.c_str()); exit(1);}

	getline(inFile, fileLineStr);
	buffer.str(fileLineStr); 
	//Read in "seed" from the first line of inputFile
	if (!(buffer >> seed)) //ERROR CHECK: ensures proper data type
	{
		buffer.clear();
		buffer >> badReadStr;
		fprintf(stderr, "line 1: Bad seed for the random number generator\n\terroneous input was: %s\n", badReadStr.c_str()); exit(1);
	}
	
	srand(seed);
	
	//Read in "rowLength" from the first line of inputFile
	if (!(buffer >> this->rowLength)) //ERROR CHECK: ensures proper data type
	{
		buffer.clear();
		buffer >> badReadStr;
		fprintf(stderr, "line 1: Bad row length--must be an integer\n\terroneous input was: %s\n", badReadStr.c_str()); exit(1);
	}
	
	//ERROR CHECK: ensures rowLength doesn't exceed predetermined range
	if ((this->rowLength < 1) || (this->rowLength > 100)) {fprintf(stderr, "line 1: The row length you entered, %d, must be from 1-100\n", this->rowLength); exit(1);}
	
	//read flavors into "flavors" vector from second line of "inputFile"
	buffer.clear();
	getline(inFile, fileLineStr);
	buffer.str(fileLineStr);
	while (buffer >> flavor) {this->flavors.push_back(flavor); flavor.clear();}
	
	//read into member vectors "probabilities" and "points" from "inputFile"
	while(getline(inFile, fileLineStr))
	{
		buffer.clear();
		buffer.str(fileLineStr);
		
		//ACCUMULATED PROBABILITY ---------------------------------------------------------------------------------------------------------------------------------------
		if (!(buffer >> probability))//ERROR CHECK: ensures proper data type
		{
			buffer.clear();
			buffer >> badReadStr;
			fprintf(stderr, "line 5: Probability and points for this sequence must be non-negative integers\n\terroneous input was: %s\n", badReadStr.c_str()); exit(1);
		}
		
		//ERROR CHECK: ensures probability doesn't exceed predetermined range 
		if (probability < 0) {fprintf(stderr, "Line 5: The probability you entered, %d, must be 0-100\n", probability); exit(1);}
		if (probability > 100) {fprintf(stderr, "Line 3: The probability you entered, %d, must be 0-100\n", probability); exit(1);}

		cumulativeProbs += probability;
		
		//ERROR CHECK: ensures probabilities don't add up to more than 100%
		if (cumulativeProbs > 100) {fprintf(stderr, "Line 27: The cumulative probability exceeds 100. It must be exactly 100 when all probabilities have been entered\n"); exit(1);}
		
		this->probabilities.push_back(cumulativeProbs);
		
		//POINTS -------------------------------------------------------------------------------------------------------------------------------------------------------	
		if (!(buffer >> points))//ERROR CHECK: ensures proper data type 
		{
			buffer.clear(); 
			buffer >> badReadStr;
			fprintf(stderr, "line 12: Probability and points for this sequence must be non-negative integers\n\terroneous input was: %s\n", badReadStr.c_str()); exit(1);
		}
		
		//ERROR CHECK: ensures "points" is not negative
		if (points < 0) {fprintf(stderr, "Line 6: points, %d, is negative. It must be non-negative\n", points); exit(1);}
		
		this->points.push_back(points);
	}
	
	inFile.close();
	
	//ERROR CHECK: ensures probabilities from "inputFile" don't add up to less than 100%  
	if (cumulativeProbs < 100) {fprintf(stderr, "The probabilities you entered must sum to 100 but their sum was %d\n", cumulativeProbs); exit(1);}

	//ERROR CHECK: ensures the size of "probabilities" and "points" vectors equals "rowLength" (only use "probabilities" size b/c it equals "points" size) 
	if (this->probabilities.size() != this->rowLength) 
	{
		fprintf(stderr, "You entered %d pairs of probabilities and points.", this->probabilities.size());
		fprintf(stderr," You must enter the same number of pairs as the rowLength, which is %d\n", this->rowLength);
		exit(1);
	}
	
	//create "candy" list of size "rowLength" with random sequences of candies  
	fillCandyList();
}
 
int candyCrush::getRowLength()
{
	return this->rowLength;
}
  
int candyCrush::getScore()
{
	return this->score;
}

//prints the candy crush row as 8 flavors per row, left justified, in fields 10 characters wide.  
void candyCrush::printCandy()
{
	list<string>::iterator lit;
	int count = -1;
	
	for (lit = this->candy.begin(); lit != this->candy.end(); lit++)
	{
		count++;
		if ( ((count % 8) + 1) == 8) {printf("%-10s\n", lit->c_str());}
		else {printf("%-10s", lit->c_str());}
	}

	printf("\n");
}
  
//creates a list of candy sequences 
void candyCrush::fillCandyList()
{
	int randomCandyIndex, candySeqProb, candySeqLength;
	
	while (this->candy.size() < this->rowLength)
	{
		//randomly choose a candy between 0 and flavors.size() - 1
		randomCandyIndex = rand() % this->flavors.size();
	
		//randomly choose probability and length of candy sequence for previously chosen candy
		candySeqProb = rand() % 100;//only 0 thru 99 --- no need to check if candySeqProb == 100 in following FOR loop!!!

		//determine sequence length of repeating candies
		for (int i = 0; i < this->probabilities.size(); i++)
		{
			if (this->probabilities[i] > candySeqProb) {candySeqLength = i + 1; break;}
		}

		//appends newly created candy sequence to the end of "candy" list 
		for (int i = 0; i < candySeqLength; i++)
		{
			this->candy.push_back(this->flavors[randomCandyIndex]);
			//ensures we don't exceed "rowLength" elements in "candy" member list
			if (this->candy.size() == this->rowLength) {break;}
		}
	}
}


int candyCrush::play(int choice)
{
	int itCount = -1, rightErase = -1, leftErase = -1, startingPoints = this->score;
	list<string>::iterator lit, litChoice, litEraseLeft, litEraseRight;

	//points "litChoice" to the "candy" element corresponding to "choice"
	for (lit = this->candy.begin(); lit != this->candy.end(); lit++) 
	{
		itCount++; 
		if (itCount == choice) {litChoice = lit; break;}
	}

	//incrementing "lit" to the RIGHT of "litChoice" and counts candies which are equal to the candy pointed at by "litChoice" 
	if (choice == (this->rowLength - 1)) {rightErase++; litEraseRight = this->candy.end();}
	else
	{
		for (lit = litChoice; lit != this->candy.end(); lit++)
			{
				if (*lit == *litChoice) {rightErase++;}
				else {break;}
			}
		
		//sets an iterator to the RIGHT bounds of the candy sequence to erase  
		litEraseRight = lit;
	}

	//decrementing "lit" to the LEFT of "litChoice" and counts candies which are equal to the candy pointed at by "litChoice" 
	if (choice == 0) {leftErase++; litEraseLeft = this->candy.begin();}
	else
	{
		for (lit = litChoice; lit != this->candy.begin(); lit--)
		{
			if (*lit == *litChoice) {leftErase++;}
			else {break;}		
		}

		//sets an iterator to the LEFT bounds of the candy sequence to erase  
		if ( (lit == this->candy.begin()) && (*lit == *litChoice) ) {leftErase++; litEraseLeft = lit;}
		else {lit++; litEraseLeft = lit;}
	}

	//calculates running tally of points earned for erasing candy sequence
	this->score += this->points[rightErase + leftErase];

	//erases repeating candy sequence whose elements are in the range [litEraseLeft, litEraseRight)
	this->candy.erase(litEraseLeft, litEraseRight);

	//fills "candy" list back to size "rowLength" with random candy sequences  
	fillCandyList();	

	//returns points earned for deleting one candy sequence
	return (this->score - startingPoints);
}
