// Challenge 03: Closest Numbers
// Name:  Tom Hills (jhills)

// Brief description:
// This program reads N integers into a vector and sorts them. Then, the smallest difference between any neighboring number in ...
// ... the vector is found and those two integers stored. If multiple minimums are found, those numbers are also stored in ascending order. 
// The stored pairs are finally printed.

// This code solves finding the minimum difference between intergers in a vector  based on the following idea/implementation...

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

//FUNCTIONS-------------------------------------------------------------------------------------------------------------------------------
void printVec(vector<int> &v)
{
	int vecSize = v.size();

	for (int i = 0; i < vecSize; ++i) 
	{
		if (i < (vecSize - 1)) {printf("%d ", v[i]);}
		else {printf("%d\n", v[i]);}
	} 
}

//MAIN------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) 
{
	int vecSize, minDiff, diff, index, num;
	vector <int> allInts, minPairs;
	
	while (cin >> vecSize)
	{
		//end outer while loop when EOF
		if (cin.fail()) {break;}

		//reset allInts in preparation for new integer set
		//read all integers into allInts
		allInts.resize(vecSize);
		index = 0;
		while (index < vecSize)
		{
			cin >> num;
			allInts[index] = num;
			++index;
		}

		//sort allInts vector (ascending order) to allow linear time in finding minimum differences
		sort(allInts.begin(), allInts.end());

		//reset and re-initialize minPair vector in preparation for new integer set in allInts.  
		//initially assume the first two integers in allInts have the minimum difference
		minPairs.clear();
		minPairs.push_back(allInts[0]);
		minPairs.push_back(allInts[1]);
		minDiff = abs(allInts[1] - allInts[0]);
		
		//cycle allInts vector and compare differences between neighbors with the current minimum difference
		for (int i = 2; i < vecSize; ++i)
		{
			diff = abs(allInts[i] - allInts[i - 1]);
			
			//new minimum difference found
			if (diff < minDiff)
			{
				minDiff = diff;
				minPairs.clear();
				minPairs.push_back(allInts[i-1]);
				minPairs.push_back(allInts[i]);
			}
			
			//new integer pair found with the same minimum difference
			else if (diff == minDiff)
			{
				minPairs.push_back(allInts[i-1]);
				minPairs.push_back(allInts[i]);
			}
		}

		//print minPairs
		printVec(minPairs);
	}
	
	return EXIT_SUCCESS;
}
