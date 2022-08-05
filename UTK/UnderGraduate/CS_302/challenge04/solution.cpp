// Challenge 04: kth smallest numbers
// Niame: Tom Hills (jhills)  

// Brief description:
// This code reads in "N" (quantity of integers to store in vector) and "k" (for k smallest value determination) from one line, 
// and on the next line reads the integers into the vector. Then, we first check to see if vector arrangement is a MAX heap. Next,
// we turn vector into a MIN heap (smallest integer will then be at index 0) so we can find the kth smallest value by removing the value 
// at index 0 "k - 1" times. This results in the kth smallest value being at index 0. We then print the maxHeap result and kth smallest value.

// This code reads in a number of ints and determines if vector arrangement is a MAX heap and then finds the kth smallest integer.

#include <iostream>
#include <vector>

using namespace std;

//FUNCTIONS-------------------------------------------------------------------------------------------------------------

//simply prints all elements of a vector --- TEST function
void printVec(vector <int> &v)
{
	int vecSize = v.size();

	for (int i = 0; i < vecSize; ++i)
	{
		printf("v[%d]: %d\n",i , v[i]);
	}
}

//repeatedly swaps parent integer with smallest child integer if parent is greater to create a MIN heap
void percolateDown_MinHeap (vector<int> &v, int parentIndex)
{
	int leftChildIndex, rightChildIndex, swap;
	bool swapOccurred = true;
	int vecSize = v.size();

	while (swapOccurred)
	{
		//setup for new loop
		swapOccurred = false;
		leftChildIndex = 2*parentIndex + 1;
		rightChildIndex = leftChildIndex + 1;
		
		//CASE #1: no children exist for "parentIndex"
		if (leftChildIndex >= vecSize) {return;}
		
		
		//CASE #2: only a leftChild exists for "parentIndex"
		else if (leftChildIndex == vecSize - 1)
		{
			//swap needed if leftChild is less than parent
			if (v[leftChildIndex] < v[parentIndex]) 
			{
				swapOccurred = true;
				swap = v[parentIndex];
				v[parentIndex] = v[leftChildIndex];
				v[leftChildIndex] = swap;

				//ensures next loop performs the same checks at leftChildIndex
				parentIndex = leftChildIndex;
			}
		}

		
		//CASE #3: both children exist for "parentIndex"
		else 
		{
			//swap with rightChild needed if rightChild is less than/equal to leftChild AND rightChild is less than parent
			//arbitrarily choosing rightChild to swap with parent if both children are equal AND less than parent
			if ( (v[rightChildIndex] <= v[leftChildIndex]) && (v[rightChildIndex] < v[parentIndex]) ) 
			{
				swapOccurred = true;
				swap = v[parentIndex];
				v[parentIndex] = v[rightChildIndex];
				v[rightChildIndex] = swap;

				//ensures next loop performs the same checks at rightChildIndex
				parentIndex = rightChildIndex;
			}

			//swap with leftChild needed if leftChild is less than rightChild AND leftChild is less than parent
			else if ( (v[leftChildIndex] < v[rightChildIndex]) && (v[leftChildIndex] < v[parentIndex]) ) 
			{
				swapOccurred = true;
				swap = v[parentIndex];
				v[parentIndex] = v[leftChildIndex];
				v[leftChildIndex] = swap;

				//ensures next loop performs the same checks at leftChildIndex
				parentIndex = leftChildIndex;
			}
		}
	}
}

//MAIN------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) 
{
	int vecSize, vecIndex, vecElement, b4SwapValue, k;
	char maxHeap;
	vector <int> vecInt;
	
	while (cin >> vecSize >> k)
	{
		if (cin.fail()) {break;}

		//prepare vector and then read in next set of integers
		vecInt.resize(vecSize);
		vecIndex = 0;

		while (vecIndex < vecSize)
		{
			cin >> vecElement;
			vecInt[vecIndex] = vecElement;
			++vecIndex;
		}
	
		//check to see if vector contains a MAX heap
		vecIndex = 0;
		maxHeap = 'Y';

		while (vecIndex*2 + 2 <= vecSize)
		{
			if ( ( vecIndex*2 + 1 < vecSize) && (vecInt[vecIndex] < vecInt[vecIndex*2 + 1]) ) 
			{
				maxHeap = 'N';
				break;
			}
	
			if ( (vecIndex*2 + 2 < vecSize) && (vecInt[vecIndex] < vecInt[vecIndex*2 + 2]) ) 
			{
				maxHeap = 'N';
				break;
			}

			++vecIndex;
		}

		//create MIN heap
		for (int i = 0; i <= vecSize/2;)
		{
			b4SwapValue = vecInt[i];
			
			percolateDown_MinHeap(vecInt, i);

			//if no swapping occurs, parent of "i" contains an int less than element "i" so move on to next element. 
			//Else, a swap occurred so reset "i" to parent index which now contains the value previously contained in "i" ...
			//... and check against new parent to see if another swap is needed. 
			if (b4SwapValue == vecInt[i]) {++i;}
			else {i = (i - 1)/2;}
		}

		//remove k - 1 smallest integers from index 0 (percolateDown_MinHeap() places smallest integer at index 0) 
		//until kth smallest integer is placed at index 0
		for (int i = 0; i < k - 1; ++i)
		{
			vecInt[0] = vecInt[vecSize - 1];
			vecInt.pop_back();
			--vecSize;
			percolateDown_MinHeap(vecInt, 0);
		}

		//final print of "maxHeap" determination (see "MAX heap check" code) and kth smallest element
		printf("%c  %d\n", maxHeap, vecInt[0]);

	}

	return 0;
}
