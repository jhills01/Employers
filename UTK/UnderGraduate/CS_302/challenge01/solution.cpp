// Challenge 01: Rotating Arrays; null solution
// Tom Hills (jhills)
// CS 302

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

void printVector(const vector <int> &vec)
{
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (i < vec.size() - 1) {printf("%d ", vec[i]);}
		else {printf("%d\n", vec[i]);}
	}
}

int main(int argc, char *argv[]) 
{
    string inputStr;
	stringstream inputSStr; 
	int vectorSize, numRotations, vectorElement;
	char direction;
	vector <int> vec;

	while (getline(cin, inputStr))
	{
		vec.resize(0);
		inputSStr.clear();
		
		inputSStr.str(inputStr);
		
		if (inputSStr >> vectorSize >> numRotations >> direction){/*Do nothing. No errors*/}
		else {fprintf(stdout, "BAD INPUT: vectorSize, numRotations, direction\n"); return 1;}

		//reads in subsequent line after WHILE read. 
		//If valid, populates vector
		if (getline(cin, inputStr))
		{
			inputSStr.clear();
			inputSStr.str(inputStr);

			for (int i = 0; i < vectorSize; i++)
			{
				if (inputSStr >> vectorElement){/*Do nothing. No errors.*/}
				else {fprintf(stdout, "BAD INPUT: vectorElement\n"); return 1;}
				
				vec.push_back(vectorElement);	
			}
		}
		//exits loop if getline returns any error
		else {break;}

		//rotate vector depending "direction" value
		for (int i = 0; i < numRotations; i++)
		{
			for (unsigned int j = 0; j < (vec.size() - 1); j++)
			{
				if (j == 0) {swap(vec[j], vec[vec.size() - 1]);}
				else if (direction == 'L') {swap(vec[j], vec[j - 1]);}
				else if (direction == 'R') {swap(vec[vec.size() - (j + 1)], vec[vec.size() - j]);}
			}
		}

		//print rotated vector
		printVector(vec);
	}
	
	return 0;
}

