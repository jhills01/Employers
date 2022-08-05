//Project 03: Superball!!

//Name: Tom Hills (jhills)

//Brief Description:
//The sb-analyze file from Superball project simply determines all scorable disjoint color sets and prints set size, color, and scoring cell.  

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <stack> //TOM ADDED
#include "disjoint.h" //TOM ADDED

using namespace std;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

class Superball {
  public:
    Superball(int argc, char **argv);
    int r;
    int c;
    int mss;
    int empty;
    vector <int> board;//each element contains the ascii value of color chars: 'p' == purple, etc
    vector <int> goals;//0 == non-goal cell, 1 == goal cell
    vector <int> colors;//colors stores point values of each color given 'p', 'b', etc ascii values as indices

//TOMS ADDITIONS--------------------------------------------------------------------------------
	
	//variables
	vector <int> visited; //0 == false/not visited, 1 == true/visited
	
	//[0] == root index of color set created during DFS function
	//[1]...[n-1] == color components in DFS traversal order
	vector <vector<int> > vecAllColorSets;
};

//created to consolidate repeating code in updateColorVec function
void updateColorVec_helper(vector<vector<int> > &matrix, int root, int cell)
{
	
	//empty == push root to start new path
	if (matrix.empty()) 
	{
		matrix.resize(1);
		matrix[0].push_back(root);
		if (cell == root) {return;}
	}
	
	int rowSize = matrix.size();
	int rootRow = -1;
	
	//check if root is already in matrix
	for (int row = 0; row < rowSize; ++row)
	{
		//root already in matrix
		if (matrix[row][0] == root) {rootRow = row;}
	}

	//root wasn't already in matrix and root == cell
	if (rootRow == -1 && root == cell)
	{
		//add one new row to matrix
		++rowSize;
		matrix.resize(rowSize);
		//place root at index 0
		matrix[rowSize - 1].push_back(root);
	}
	
	//root wasn't already in matrix and root != cell
	else if (rootRow == -1 && root != cell)
	{
		//add one new row to matrix
		++rowSize;
		matrix.resize(rowSize);
		//place root at index 0, and then non-root cell at index 1
		matrix[rowSize - 1].push_back(root);
		matrix[rowSize - 1].push_back(cell);
	}

	//root was already in matrix and cell == root
	//nothing new to add to matrix so just return
	else if (rootRow != -1 && root == cell) {return;}

	//root was already in matrix and cell != root
	else if (rootRow != -1 && root != cell)
	{
		//check to see if cell already exists in matrix row rooted by rootRow
		for (unsigned int col = 0; col < matrix[rootRow].size(); ++col)
		{
			if (cell == matrix[rootRow][col]) {return;}
		}
	
		//cell not found. push into appropriate matrix row
		matrix[rootRow].push_back(cell);
	}
}

//updates all color specfic vectors
void updateAllColorVec(Superball *s, int root, int cell)
{
	//update all color sets vector
	updateColorVec_helper(s->vecAllColorSets, root, cell);
}

//created to consolidate repeating code in createColorSet_DFS function
void createColorSet_DFS_helper(Superball *s, DisjointSetByRankWPC *dsRankPC, int &i, int &adj, stack <int> &path)
{
	//"Find" returns the root while performing PC on all set members
	int iRoot = dsRankPC->Find(i);
	int adjRoot = dsRankPC->Find(adj);

	if (iRoot != adjRoot)
	{
		//makes "iRoot" the root and "adjRoot" child. 
		dsRankPC->Union(adjRoot, iRoot);
		adjRoot = iRoot;
	}

	updateAllColorVec(s, adjRoot, adj);
	
	path.push(adj); 
	s->visited[adj] = 1;
	i = adj; 
}


//A depth-first search that creates a disjointed set component of same colored Superball cells (if any) rooted at "i" 
void createColorSet_DFS(Superball *s, DisjointSetByRankWPC *dsRankPC, int i)
{
	//check for valid index in Superball board and exit program if invalid
	if (i >= s->board.size() || i < 0) {fprintf(stderr, "%d is not a valid index for Superball board.\n\n",i); exit(1);}

	int rootIndex = dsRankPC->Find(i);
	int top, bottom, left, right; 
	int iRow, iCol, leftRow, leftCol, rightRow, rightCol;
	stack <int> path;
	
	//initialize "visited" and "path" data structures
	s->visited[i] = 1;
	path.push(i); 

	//start disjoint set rooted by i's root
	createColorSet_DFS_helper(s, dsRankPC, rootIndex, i, path);

	while (!path.empty())
	{
		//calculate indices of neighboring cells
		top = i - s->c;
		bottom = i + s->c;
		left = i - 1;
		right = i + 1;

		//calculate matrix indices corresponding to the r*c Superball "board" matrix (column calculated b4 row)
		//these are required to ensure we stop at the edges of the matrix board without wrapping around to a new row
		leftCol = left%s->c;
		leftRow = (left - leftCol)/s->c;
		rightCol = right%s->c;
		rightRow = (right - rightCol)/s->c;
		iCol = i%s->c;
		iRow = (i - iCol)/s->c;

		//top is a valid index in "board", hasn't been visited before, and contains the same color as index i
		if (top >= 0 && !s->visited[top] && s->board[top] == s->board[i])
		{
			//merges the sets containing "i" and "top" if roots are different
			//pushes "top" on stack, marks "top" as visited, assigns "top" to "i"
			createColorSet_DFS_helper(s, dsRankPC, i, top, path);
		}
		
		//left is a valid matrix index of "board", hasn't been visited before, and contains the same color as index i
		else if (iRow == leftRow && !s->visited[left] && s->board[left] == s->board[i])
		{
			//merges the sets containing "i" and "left" if roots are different
			//pushes "left" on stack, marks "left" as visited, assigns "left" to "i"
			createColorSet_DFS_helper(s, dsRankPC, i, left, path);
		}

		//right is a valid matrix index of "board", hasn't been visited before, and contains the same color as index i
		else if (iRow == rightRow && !s->visited[right] && s->board[right] == s->board[i])
		{
			//merges the sets containing "i" and "right" if roots are different
			//pushes "right" on stack, marks "right" as visited, assigns "right" to "i"
			createColorSet_DFS_helper(s, dsRankPC, i, right, path);
		}
		
		//bottom is a valid matrix index of "board", hasn't been visited before, and contains the same color as index i
		else if (bottom <  s->board.size() && !s->visited[bottom] && s->board[bottom] == s->board[i])
		{
			//merges the sets containing "i" and "bottom" if roots are different
			//pushes "bottom" on stack, marks "bottom" as visited, assigns "bottom" to "i"
			createColorSet_DFS_helper(s, dsRankPC, i, bottom, path);
		}
		
		//if stack isn't empty, pops and then sets "i" to new top cell in path to check previously unvisited neighbors
		else 
		{
			path.pop();
			if (!path.empty()) {i = path.top();}
		}
	}
}

//TOMS ADDITIONS--------------------------------------------------------------------------------

void usage(const char *s) 
{
  fprintf(stderr, "usage: sb-read rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);

//TOMS ADDITIONS--------------------------------------------------------------------------------
  
  visited.resize(r*c, 0);//all marked as unvisited (i.e., element == 0 or false)

//TOMS ADDITIONS--------------------------------------------------------------------------------

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if (s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
		board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }
}

main(int argc, char **argv)
{
	Superball *s;
	DisjointSetByRankWPC *dsRankPC;
	int i, j;
	int ngoal, tgoal;

	s = new Superball(argc, argv);

	//creates r*c disjoint sets.
	dsRankPC  = new DisjointSetByRankWPC(s->r*s->c);

	tgoal = 0;
	ngoal = 0;
	
	//update all data structures with board layout information
	for (i = s->r*s->c - 1; i >= 0; --i)
	{
		//this locks onto a goal cell that is populated with a color
		if (s->goals[i] && s->board[i] != '*') 
		{
			tgoal += s->colors[s->board[i]];
			ngoal++;
			
			//analyze goals of Superball "board"
			createColorSet_DFS(s, dsRankPC, i);
		}
	}

	//analyze final print
	int goalIndex, goalCol, goalRow, size;

	printf("Scoring sets:\n");

	for (unsigned int i = 0; i < s->vecAllColorSets.size(); ++i)
	{
		goalIndex = s->vecAllColorSets[i][0];
		goalCol = goalIndex%s->c;
		goalRow = (goalIndex - goalCol)/s->c; 
		size = s->vecAllColorSets[i].size();
		if (size >= s->mss)
		{
			printf("  Size: %-4dChar: %-3cScoring Cell: %d,%d\n", size, s->board[goalIndex], goalRow, goalCol);
		}
	}
	
	exit(0);
}
