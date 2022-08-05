//Tom Hills (jhills)
//CS140 - Lab1, checkerboard

#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

int main()
{
	int rows, columns, cycleSize, width;
	char startingChar;
	
	//Error check for "rows" variable
	if (!(cin >> rows)) {cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W\n"; return 1;}
	if (rows <= 0) {return 1;}
  
    //Error check for "columns" variable
	if (!(cin >> columns)) {cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W\n"; return 2;}
	if (columns <= 0) {return 1;}

	//Error check for "startingChar" variable
	if (!(cin >> startingChar)) {cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W\n"; return 3;}
	if ((int)startingChar <= 0) {return 1;}
	
	//Error check for "cycleSize" variable
	if (!(cin >> cycleSize)) {cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W\n"; return 4;}
	if (cycleSize <= 0) {return 1;}
	
	//Error check for "width" variable
	if (!(cin >> width)) {cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W\n"; return 5;}
	if (width <= 0) {return 1;}

	if (((int)startingChar + cycleSize) > 127) {return 1;}

	//Loop and variables to calculate values and printout results
	int loopRow = 0, loopColumn = 0;
	char runningChar;
	
	for (int i = 0; i < rows; i++)
	{
		//loop prints out the same row (these rows contain characters determined by "runningChar") "width" times
		for (int j = 0; j < width*columns; j++)
		{
			//starts new row but DOES NOT increment loopRow because we're printing the same row "width" times
			if (j%columns == 0 && j != 0)
			{
				cout << '\n';
				loopColumn = 0;
			}

			//determines the running character for a particular row
			runningChar = startingChar + (loopRow + loopColumn)%cycleSize;

			//prints the same character w times 
			for (int k = 0; k < width; k++)
			{
				cout << runningChar;
			}
			
			//iterates to the next column number to calculate the next "runningChar"
			loopColumn++;

			if (j == (width*columns - 1)) {cout << '\n';}
		}
		
		loopColumn = 0;
		loopRow++;
	}
	
	return 0;
}
