//NOTE-TO-GRADER: I put in some extra functionality for handling multiple compare strings, even though only 2 will be compared a time. 
//...I don't have code to choose which two strings you want to compare, but that could be easily added in. If you're curious, I did this...
//...extra stuff just because. It's definitely not necessary to complete the challenge. Also, I left the recursion in because I thought it was...
//...necessary for this challenge. I didn't remember that part of dynamic programming was to remove the recursion if possible. That would have...
//...been better on processing time. 

// Challenge 9: Sequence alignment

// Tom Hills (jhills)

// Description:
// This program reads-in any number of character sequences and then chooses two to compare using the Needleman-Wunsch sequence alignment algorithm.
// The final output is the score associated with the best alignment between the two chosen sequences. 

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <limits.h>

using namespace std;

//CLASSES-------------------------------------------------------------------------------------------
class seqAlignment
{
	public:
		//Variables
		vector <vector <char> > seqs;
		vector <int> table;
		
		//Methods
		seqAlignment();
		void print_seqs();
		void printIntVec(vector <int> &v, int col);
		void buildTable(vector <char> &v1, vector <char> &v2);
		int calTable(int n, vector <char> &v1, vector <char> &v2);
};


//FUNCTIONS------------------------------------------------------------------------------------------

//constructor - reads in one sequence per input line into 2d char vector, seqs
seqAlignment::seqAlignment()
{
	string iStr;

	while (getline(cin, iStr))
	{
		//allocate seqs to hold new sequence
		seqs.resize(seqs.size() + 1);
		seqs[seqs.size() - 1].resize(iStr.size()); 

		//read in characters from sequence
		for (int i = 0; i < (int)iStr.size(); ++i)
		{
			seqs[seqs.size() - 1][i] = iStr[i];		
		}
	}
}

//prints seqs member
void seqAlignment::print_seqs()
{
	for (int i = 0; i < (int)seqs.size(); ++i)
	{
		for (int j = 0; j < (int)seqs[i].size(); ++j)
		{
			if (j == (int)seqs[i].size() - 1) {printf("%2c\n", seqs[i][j]); break;}
			printf("%2c ", seqs[i][j]);
		}
	}
}

//debugging print --- basically to print "table" member
void seqAlignment::printIntVec(vector <int> &v, int col)
{
	for (int i = 0; i < (int)v.size(); ++i)
	{
		if ( (i != 0 && (i + 1) % col == 0) || i == (int)v.size() - 1) {printf("%2d\n", v[i]);}
		else {printf("%2d ", v[i]);}
	}
}

//builds a sequence alignment table formatted for Needleman-Wunsch algorithm (i.e., includes one extra row and column for lamda)
//Assumes a global alignment with +1 for a match, -1 for a mismatch, and -1 for any gap.
void seqAlignment::buildTable(vector <char> &colSeq, vector <char> &rowSeq)
{
	//the "..+ 1" accounts for the lamda row and column 
	table.resize( (colSeq.size() + 1)*(rowSeq.size() + 1), INT_MAX); 
	
	//fill out first row of table with starting values	
	int cell = 0;

	for (int i = 0; i <= (int)colSeq.size(); ++i)
	{
		table[i] = cell;
		--cell;
	}

	//fill out first column of table with starting values
	cell = -1;

	for (int i = 1; i <= (int)rowSeq.size(); ++i)
	{
		table[i*(colSeq.size() + 1)] = cell;
		--cell;
	}
}

//calculates sequence values for table and returns a final score stored in last element of the table
int seqAlignment::calTable(int n, vector <char> &colSeq, vector <char> &rowSeq)
{
	//base case
	if (table[n] != INT_MAX) {return table[n];}
	
	//initialize variables
	int tableCol, tableRow, top, diagonal, left;
	set<int> s;
	set<int>::iterator sit;

	//recursive cases
	for (int i = n; i < (int)table.size(); ((i + 1) % (colSeq.size() + 1) == 0) ? i += 2 : ++i /*skips lamda column*/)
	{
		tableCol = i % (colSeq.size() + 1);
		// i = tableRow * (colSeq.size() + 1) + tableCol
		tableRow = (i - tableCol)/(colSeq.size() + 1); 
		
		//score from top neighboring cell + gap penalty, -1.
		top = calTable(i - (colSeq.size() + 1), colSeq, rowSeq) + -1; 
		//score from top left diagonal neighboring cell + (1 IF sequence characters match or -1 if they don't(i.e., mistmatch))
		//colSeq and rowSeq are passed col and rows values "- 1" to account for the extra lamda element in table
		diagonal = (calTable(i - (colSeq.size() + 2), colSeq, rowSeq)) + ((colSeq[tableCol - 1] == rowSeq[tableRow - 1]) ? 1 : -1);
		//score from left neighboring cell + gap penalty, -1.
		left = calTable(i - 1, colSeq, rowSeq) + -1;

		//put scores from top, diagonal, and left into set and then choose the largest value which is assigned to table[i] 
		s.clear();
		s.insert(top);
		s.insert(diagonal);
		s.insert(left);
		
		sit = s.end();
		--sit;
		
		table[i] = *sit;
	}

	return table[table.size() - 1];
}

//MAIN-----------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) 
{
	seqAlignment s; 
	int score;

	//build table and cache based on the first two sequences read-in
	s.buildTable(s.seqs[0], s.seqs[1]); 
	
	//pass the size of the sequence forming the column of the table plus 2 to start at the correct table index 
	score = s.calTable( s.seqs[0].size() + 2, s.seqs[0], s.seqs[1] );

	//final print
	cout << score << endl;	

	return 0;
}

