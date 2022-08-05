//Tom Hills (jhills)
//Lab 9, Part 2: ShapeShifter

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

class ShapeShifter
{
	//Made all methods and variables public because it was easier to simply construct members variables (normally protected) directly in main()
	public:
		//MEMBER METHODS-------------------------------------------------------------------------------------------------
		void printGrid();	
		void printShapes();
		void printPrintStrings();

		//applies shapes from "shapes" vector to "grid" vector
		void Apply (int gridRow, int gridCol, int shapesIndex);
		
		//recursive method to find solution
		void findSolution(int shapesIndex);
	
		//method to determine if "grid" has been transformed into all ones (i.e., a valid solution)
		bool allOnes();
		
		//MEMBER VARIABLES------------------------------------------------------------------------------------------------
		//game grid and dimensional constants
		vector <vector<int> > grid;
		int gridRows; 
		int gridCols; 
		
		//vector of shapes that will be applied to "grid" 
		vector <vector <vector<int> > > shapes;
		
		//vectors of shape dimensions to ensure that shapes are NOT partially applied to "grid"
		vector <int> shapeHeights;
		vector <int> shapeWidths;

		//vector of LOOP limits within "findSolution" method to ensure shapes are NOT partially applied "grid"
		vector <int> rowLimits;
		vector <int> colLimits;
		
		//vector of final print statements if problem can be solved. 
		vector <string> printStrings;
		
		//used to store the result of "allOnes" function to save processing time
		bool solved;
};

//prints elements of "grid". Used for debugging.
void ShapeShifter::printGrid()
{
	for (int gridRow = 0; gridRow < this->gridRows; gridRow++)
	{
		for (int gridCol = 0; gridCol < this->gridCols; gridCol++)
		{
			if (gridCol < (this->gridCols - 1)) {printf("%d ", this->grid[gridRow][gridCol]);}
			else {printf("%d\n", this->grid[gridRow][gridCol]);}
		}
	}
}

//prints elements of "shapes". Used for debugging.
void ShapeShifter::printShapes()
{
	int shapeOuterRows = this->shapes.size();
	int shapeInnerRows;
	int shapeInnerCols;

	for (int shapeOuterRow = 0; shapeOuterRow < shapeOuterRows; shapeOuterRow++)
	{
		printf("\nSHAPE %d:\n",shapeOuterRow);
		
		shapeInnerRows = this->shapes[shapeOuterRow].size();

		for (int shapeInnerRow = 0; shapeInnerRow < shapeInnerRows; shapeInnerRow++)
		{
			shapeInnerCols = this->shapes[shapeOuterRow][shapeInnerRow].size();
			
			for (int shapeInnerCol = 0; shapeInnerCol < shapeInnerCols; shapeInnerCol++)
			{
				if (shapeInnerCol < (shapeInnerCols - 1) ) {printf("%d ", this->shapes[shapeOuterRow][shapeInnerRow][shapeInnerCol]);}
				else {printf("%d\n", this->shapes[shapeOuterRow][shapeInnerRow][shapeInnerCol]);}
			}
		}
	}
}

//prints final output for program if a solution was found
void ShapeShifter::printPrintStrings()
{
	for (int i = 0; i < this->printStrings.size(); i++)
	{
		printf("%s\n", this->printStrings[i].c_str());
	}
}

//applies shapes to "grid" at the given row and column and...
//...bounds loops by shapes' dimensions to ensure shapes are not partially applied to "grid"
void ShapeShifter::Apply (int gridRow, int gridCol, int shapesIndex)
{
	for (int i = gridRow; i < (gridRow + shapeHeights[shapesIndex]); i++)
	{
		for (int j = gridCol; j < (gridCol + shapeWidths[shapesIndex]); j++)
		{
			this->grid[i][j] = this->grid[i][j] ^ this->shapes[shapesIndex][i - gridRow][j - gridCol];
		}
	}
}

//recursive solution to game 
void ShapeShifter::findSolution(int shapesIndex)
{
	for (int gridRow = 0; gridRow < this->rowLimits[shapesIndex]; gridRow++)
	{
		for (int gridCol = 0; gridCol < this->colLimits[shapesIndex]; gridCol++)
		{
			//first apply to change "grid"
			this->Apply(gridRow, gridCol, shapesIndex);
			
			//conditional ensures that recursive calls are made until we reach the final shape in "shapes"
			if (shapesIndex < (this->shapes.size() - 1)) {this->findSolution(shapesIndex + 1);}

			//checks "solved" first to save on processing time.
			//if satisfied, it concatenates current indices to corresponding "printString" index for final printing 
			if ( (this->solved == true) || this->allOnes()) 
			{
				//hope you enjoy the variable names :). I found it fitting.
				string std98_sucks;
				stringstream to_stringWontCompile;
				to_stringWontCompile << " " << gridRow << " " << gridCol;
				std98_sucks = to_stringWontCompile.str();
				printStrings[shapesIndex] += std98_sucks; 

				return;
			}

			//second apply to undo changes in preparation for applying shape at different location
			this->Apply(gridRow, gridCol, shapesIndex);
		}
	}
}

//checks "grid" and returns true if all elements == 1. Or false otherwise. 
bool ShapeShifter::allOnes()
{
	this->solved = true;

	//Loop to check if all elements of "grid" member == 1
	for (int gridRow = 0; gridRow < this->gridRows; gridRow++)
	{
		for (int gridCol = 0; gridCol < this->gridCols; gridCol++)
		{
			if (this->grid[gridRow][gridCol] == 0) {this->solved = false; break;}
		}

		if (this->solved == false) {break;}
	}

	return this->solved;
}

int main (int argc, char *argv[])
{
	string command, lineItem;
	istringstream buffer;
	int gridRows = 0, shapeOuterRows = 0, shapeInnerRows = 0;
	ShapeShifter game;

	//creating "grid"
	for (int i = 1; i < argc; i++)
	{
		gridRows++;
		game.grid.resize(gridRows);

		for (int j = 0; argv[i][j] != '\0'; j++)
		{
			game.grid[gridRows - 1].push_back(argv[i][j] - '0');
		}
	}
	
	//creating "shapes" with shape elements in the same order as input files 
	while (getline(cin, command))
	{
		//create strings vector for easy printing if solution is found.
		game.printStrings.push_back(command);
		
		buffer.clear();
		buffer.str(command);

		shapeOuterRows++;
		game.shapes.resize(shapeOuterRows);

		shapeInnerRows = 0;

		while (buffer >> lineItem)
		{
			shapeInnerRows++;
			game.shapes[shapeOuterRows - 1].resize(shapeInnerRows);
			
			for (int i = 0; i < lineItem.size(); i++)
			{
				game.shapes[shapeOuterRows - 1][shapeInnerRows - 1].push_back(lineItem[i] - '0');
			}
		}
	}

	//initalize member variables
	game.gridRows = game.grid.size();
	game.gridCols = game.grid[0].size();
	game.solved = false;
	
	//establishing the following vectors to save on processing time for "findSolution" and "Apply" methods
	for (int i = 0; i < game.shapes.size(); i++)
	{
		//vector of shape dimensions to ensure constant time for determining loop conditionals in "Apply" method 
		game.shapeHeights.push_back(game.shapes[i].size());
		game.shapeWidths.push_back(game.shapes[i][0].size());

		//vector of loop constraints to ensure constant time when processing loop conditionals in "findSolution" method
		//the formulas being pushed ensure shapes are not partially applied to "grid"
		game.rowLimits.push_back((game.gridRows - game.shapeHeights[i]) + 1);
		game.colLimits.push_back((game.gridCols - game.shapeWidths[i]) + 1);
	}
	
	//passing 0 parameter corresponds to the first shape in "shapes"
	game.findSolution(0);

	//prints solution if solved or prints nothing if not
	if (game.solved) {game.printPrintStrings(); return 0;}
	else {/*not solvable*/return 0;}
}

