//Project 03: Superball!!

//Name: Tom Hills (jhills)

//Brief Description:
//The sb-play file from Superball project takes a static Superball board and analyzes it for making the best move (SCORE a goal or SWAP two cells).
//Once the best move is determined, the program outputs the appropriate command the Dr. Planks's sb-player applies the move and returns another board...
//...which will start the process all over with the new board.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
//TOM ADDED
#include <stack> 
#include "disjoint.h" 
#include <map> 

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
	
	//[0] == root index of color set rooted at goal cell
	//[1]...[n-1] == color components in DFS traversal order
	vector <vector<int> > vecAllGoalColorSets;

	//[0] == root index of color set rooted at non-goal cell
	//[1]...[n-1] == color components in DFS traversal order
	vector <vector<int> > vecAllNonGoalColorSets;

	//swapCell: key == color point sum of the entire "vecAllGoalColorSets" vector
	//			value == pair of cell indices to be swapped to obtain key value (NOTE: indices need matrix conversion)
	//scoreCells: key == color point sum of smallest SCORABLE row in "vecAllGoalColorSets" vector
	//			  value == pair where only FIRST member hold index to be scored. SECOND will have -1 value so...
	//			  ...it can be distinguished from "swapCells". (NOTE: FIRST of pair will need matrix conversion)
	multimap <int, pair<int, int> > swapCells_Goals_LTMss;
	multimap <int, pair<int, int> > swapCells_Goals_GTEqMss;
	multimap <int, pair<int, int> > swapCells_NonGoals_LTMss;
	multimap <int, pair<int, int> > swapCells_NonGoals_GTEqMss;
	multimap <int, int> scoreCells;
};

//created to consolidate repeating code in updateColorVec function
//this takes any arbitrary 2d color vector and updates its contents based on Disjoint Set data 
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

//updates vector containing all colors sets rooted at a goal cell
void updateAllGoalColorVec(Superball *s, int root, int cell)
{
	//update all color sets vector
	updateColorVec_helper(s->vecAllGoalColorSets, root, cell);
}

//updates vector containing all colors sets NOT rooted at a goal cell
void updateAllNonGoalColorVec(Superball *s, int root, int cell)
{
	//update all color sets vector
	updateColorVec_helper(s->vecAllNonGoalColorSets, root, cell);
}

//created to consolidate repeating code in createGoalColorSet_DFS function
void createGoalColorSet_DFS_helper(Superball *s, DisjointSetByRankWPC *dsRankPC, int &i, int &adj, stack <int> &path)
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

	updateAllGoalColorVec(s, adjRoot, adj);
	
	path.push(adj); 
	s->visited[adj] = 1;
	i = adj; 
}


//A depth-first search that creates a disjointed set component of same colored Superball cells (if any)...
//...rooted at goal cell "i" 
void createGoalColorSet_DFS(Superball *s, DisjointSetByRankWPC *dsRankPC, int i)
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
	createGoalColorSet_DFS_helper(s, dsRankPC, rootIndex, i, path);

	//NOTE: only push_back into  if we push onto stack
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
			createGoalColorSet_DFS_helper(s, dsRankPC, i, top, path);
		}
		
		//left is a valid matrix index of "board", hasn't been visited before, and contains the same color as index i
		else if (iRow == leftRow && !s->visited[left] && s->board[left] == s->board[i])
		{
			//merges the sets containing "i" and "left" if roots are different
			//pushes "left" on stack, marks "left" as visited, assigns "left" to "i"
			createGoalColorSet_DFS_helper(s, dsRankPC, i, left, path);
		}

		//right is a valid matrix index of "board", hasn't been visited before, and contains the same color as index i
		else if (iRow == rightRow && !s->visited[right] && s->board[right] == s->board[i])
		{
			//merges the sets containing "i" and "right" if roots are different
			//pushes "right" on stack, marks "right" as visited, assigns "right" to "i"
			createGoalColorSet_DFS_helper(s, dsRankPC, i, right, path);
		}
		
		//bottom is a valid matrix index of "board", hasn't been visited before, and contains the same color as index i
		else if (bottom <  s->board.size() && !s->visited[bottom] && s->board[bottom] == s->board[i])
		{
			//merges the sets containing "i" and "bottom" if roots are different
			//pushes "bottom" on stack, marks "bottom" as visited, assigns "bottom" to "i"
			createGoalColorSet_DFS_helper(s, dsRankPC, i, bottom, path);
		}
		
		//if stack isn't empty, pops and then sets "i" to new top cell in path to check previously unvisited neighbors
		else 
		{
			path.pop();
			if (!path.empty()) {i = path.top();}
		}
	}
}

//created to consolidate repeating code in createNonGoalColorSet_DFS function
void createNonGoalColorSet_DFS_helper(Superball *s, DisjointSetByRankWPC *dsRankPC, int &i, int &adj, stack <int> &path)
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

	updateAllNonGoalColorVec(s, adjRoot, adj);
	
	path.push(adj); 
	s->visited[adj] = 1;
	i = adj; 
}

//A depth-first search that creates a disjointed set component of same colored Superball cells (if any)...
//...rooted at NON-goal cell "i". 
void createNonGoalColorSet_DFS(Superball *s, DisjointSetByRankWPC *dsRankPC, int i)
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
	createNonGoalColorSet_DFS_helper(s, dsRankPC, rootIndex, i, path);

	//NOTE: only push_back into  if we push onto stack
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
			createNonGoalColorSet_DFS_helper(s, dsRankPC, i, top, path);
		}
		
		//left is a valid matrix index of "board", hasn't been visited before, and contains the same color as index i
		else if (iRow == leftRow && !s->visited[left] && s->board[left] == s->board[i])
		{
			//merges the sets containing "i" and "left" if roots are different
			//pushes "left" on stack, marks "left" as visited, assigns "left" to "i"
			createNonGoalColorSet_DFS_helper(s, dsRankPC, i, left, path);
		}

		//right is a valid matrix index of "board", hasn't been visited before, and contains the same color as index i
		else if (iRow == rightRow && !s->visited[right] && s->board[right] == s->board[i])
		{
			//merges the sets containing "i" and "right" if roots are different
			//pushes "right" on stack, marks "right" as visited, assigns "right" to "i"
			createNonGoalColorSet_DFS_helper(s, dsRankPC, i, right, path);
		}
		
		//bottom is a valid matrix index of "board", hasn't been visited before, and contains the same color as index i
		else if (bottom <  s->board.size() && !s->visited[bottom] && s->board[bottom] == s->board[i])
		{
			//merges the sets containing "i" and "bottom" if roots are different
			//pushes "bottom" on stack, marks "bottom" as visited, assigns "bottom" to "i"
			createNonGoalColorSet_DFS_helper(s, dsRankPC, i, bottom, path);
		}
		
		//if stack isn't empty, pops and then sets "i" to new top cell in path to check previously unvisited neighbors
		else 
		{
			path.pop();
			if (!path.empty()) {i = path.top();}
		}
	}
}

//starts the process of updating all color vectors
//prioritizes color vectors rooted at goal cell
void updateSbBoardStats(Superball *s, DisjointSetByRankWPC *dsRankPC)
{
	int i;
	
	//update all color vectors associated with goal cells
	for (i = s->r*s->c - 1; i >= 0; --i)
	{
		//this locks onto a goal cell that is populated with a color
		if (s->goals[i] && s->board[i] != '*') 
		{
			//update color vectors associated with goal cells
			createGoalColorSet_DFS(s, dsRankPC, i);
		}
	}
		
	//update all color vectors NOT associated with goal cells
	for (i = s->r*s->c - 1; i >= 0; --i)
	{
		//this locks onto a NON-goal cell that is populated with a color and hasn't yet been visited
		if (!s->goals[i] && s->board[i] != '.' && !s->visited[i]) 
		{
			//update color vectors NOT associated with goal cells
			createNonGoalColorSet_DFS(s, dsRankPC, i);
		}
	}

	//reset visited vector in preparation for next update
	for (unsigned int i = 0; i < s->visited.size(); ++i) {s->visited[i] = 0;}
}

//clears all color vectors in preparation for new update after swap
void clearSbBoardStats(Superball *s)
{
	//clear all Goal color vectors
	s->vecAllGoalColorSets.clear();

	//clear all Non-Goal color vectors
	s->vecAllNonGoalColorSets.clear();
}

//scores one row from color vector. 
//Each element of v is an s.board index
int scoreRow(Superball *s, vector <int> &v)
{
	return (v.size() * s->colors[s->board[v[0]]]);
}

//calculates total color score for any 2d color vector of Superball whose set size equals or exceeds mss
int score2dVec_GTEqMss(Superball *s, vector<vector<int> > &v)
{
	int totalColorScore = 0;
	
	for (unsigned int row = 0; row < v.size(); ++row)
	{
		if (v[row].size() >= s->mss) 
		{
			totalColorScore += s->colors[s->board[v[row][0]]] * v[row].size();
		}
	}

	return totalColorScore;
}

//calculates total color score for any 2d color vector of Superball whose set size is less than mss, but greater than 1
int score2dVec_LTMss(Superball *s, vector<vector<int> > &v)
{
	int totalColorScore = 0;
	
	for (unsigned int row = 0; row < v.size(); ++row)
	{
		if (v[row].size() < s->mss && v[row].size() > 1)
		{
			totalColorScore += s->colors[s->board[v[row][0]]] * v[row].size();
		}
	}

	return totalColorScore;
}

//prints Swap command and then ends program
void printSwap(multimap<int, pair<int, int> >::reverse_iterator rSwapIt, Superball *s, DisjointSetByRankWPC *dsRankPC)
{
		int iRow, iCol, jRow, jCol;
		int ii = rSwapIt->second.first;
		int jj = rSwapIt->second.second;

		iCol = ii%s->c;
		iRow = (ii - iCol)/s->c;
		jCol = jj%s->c;
		jRow = (jj - jCol)/s->c;
		printf("SWAP %d %d %d %d\n", iRow, iCol, jRow, jCol);
		
		delete s;
		delete dsRankPC;
		exit(0);
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
	int i, j, iRow, iCol, jRow, jCol;
	int allGoalsGTEqMss_start, allGoalsLTMss_start, allNonGoalsGTEqMss_start, allNonGoalsLTMss_start;
	int allGoalsGTEqMss_swap;
	int ngoal, tgoal;
//	multimap<int, pair<int, int> >::iterator swapIt;
//	multimap<int, pair<int, int> >::iterator swapIt2;
	multimap<int, int>::iterator scoreIt;
	multimap<int, pair<int, int> >::reverse_iterator rSwapIt;
	multimap<int, pair<int, int> >::reverse_iterator rSwapIt2;
	multimap<int, pair<int, int> >::reverse_iterator rSwapIt3;
	multimap<int, pair<int, int> >::reverse_iterator rSwapIt4;
//	multimap<int, int>::reverse_iterator rScoreIt;
	char temp;

	s = new Superball(argc, argv);
	dsRankPC  = new DisjointSetByRankWPC(s->r*s->c);

	tgoal = 0;
	ngoal = 0;
	
	//NOTE: don't think I'll need this, but keeping it in just in case
	for (i = s->r*s->c - 1; i >= 0; --i)
	{
		//this locks onto a goal cell that is populated with a color
		if (s->goals[i] && s->board[i] != '*') 
		{
			tgoal += s->colors[s->board[i]];
			ngoal++;
		}
	}
	
	//update all color vectors with initial state of Superball board before looking for best move
	updateSbBoardStats(s, dsRankPC);
	
	//store total scorable points from initial board configuration for comparison with later swap scores
	allGoalsGTEqMss_start = score2dVec_GTEqMss(s,s->vecAllGoalColorSets);
	allGoalsLTMss_start = score2dVec_LTMss(s,s->vecAllGoalColorSets);
	allNonGoalsGTEqMss_start = score2dVec_GTEqMss(s,s->vecAllNonGoalColorSets);
	allNonGoalsLTMss_start = score2dVec_LTMss(s,s->vecAllNonGoalColorSets);

	//if empty cells number less than 5, a score must be made to create new empty cells or game is over
	if (s->empty < 5)
	{
		//score2dVec_GTEqMss() returns 0 if no set rooted at a goal cell is scorable
		if (score2dVec_GTEqMss(s, s->vecAllGoalColorSets))
		{
			for (unsigned int row = 0; row < s->vecAllGoalColorSets.size(); ++row)
			{
				if (s->vecAllGoalColorSets[row].size() >= s->mss) 
				{
					//key == color score of row
					//value == root of set (i.e., board index i which needs to be scored)
					s->scoreCells.insert(pair<int,int>(scoreRow(s, s->vecAllGoalColorSets[row]),
						dsRankPC->Find(s->vecAllGoalColorSets[row][0])));
				}
			}

			//print scorable set with the smallest total (NOTE: what about sets with same total?!?)
			scoreIt = s->scoreCells.begin();
			iCol = scoreIt->second%s->c;
			iRow = (scoreIt->second - iCol)/s->c;
			printf("SCORE %d %d\n", iRow, iCol);
			
			delete s;
			delete dsRankPC;
			exit(0);
		}

		//swap first two random colored cells found to end the game ()
		else
		{
			for (i = 0; i < s->r*s->c; ++i)
			{
				//index i must be a color before I start the exhaustive swapping process 
				if (s->board[i] != '.' && s->board[i] != '*')
				{
					for (int j = i + 1; j < s->r*s->c; ++j)
					{
						if (s->board[j] != '.' && s->board[j] != '*')
						{
							//swap
							temp = s->board[i];
							s->board[i] = s->board[j];
							s->board[j] = temp;

							//print final game ending swap and exit
							iCol = i%s->c;
							iRow = (i - iCol)/s->c;
							jCol = j%s->c;
							jRow = (j - jCol)/s->c;
							printf("SWAP %d %d %d %d\n", iRow, iCol, jRow, jCol);
							
							delete s;
							delete dsRankPC;
							exit(0);
						}
					}
				}
			}
		}
	}

	//empty cells number greater than or equal to 5. We determine best swap to make.
	else 
	{
		for (i = 0; i < s->r*s->c; ++i)
		{
			if (s->board[i] != '.' && s->board[i] != '*')
			{
				for (int j = i + 1; j < s->r*s->c; ++j)
				{
					if (s->board[j] != '.' && s->board[j] != '*')
					{
						//swap
						temp = s->board[i];
						s->board[i] = s->board[j];
						s->board[j] = temp;

						//reset disjoint set and board statistics, and then update
						delete dsRankPC;
						dsRankPC  = new DisjointSetByRankWPC(s->r*s->c);
						clearSbBoardStats(s);
						updateSbBoardStats(s, dsRankPC);

						allGoalsGTEqMss_swap = score2dVec_GTEqMss(s,s->vecAllGoalColorSets);
						
						//store Superball board score statistics in appropriate multimap members
						s->swapCells_Goals_LTMss.insert(pair<int, pair<int, int> > (score2dVec_LTMss(s, s->vecAllGoalColorSets), 
						pair<int, int>(i,j)));
						
						if (allGoalsGTEqMss_swap >= allGoalsGTEqMss_start)
						{
							s->swapCells_Goals_GTEqMss.insert(pair<int, pair<int, int> > (score2dVec_GTEqMss(s, s->vecAllGoalColorSets), 
							pair<int, int>(i,j)));
						}
						
						s->swapCells_NonGoals_LTMss.insert(pair<int, pair<int, int> > (score2dVec_LTMss(s, s->vecAllNonGoalColorSets), 
						pair<int, int>(i,j)));
						
						s->swapCells_NonGoals_GTEqMss.insert(pair<int, pair<int, int> > (score2dVec_GTEqMss(s, s->vecAllNonGoalColorSets),
						pair<int, int>(i,j)));

						//reverse swap to put board back in original configuration and clear all Superball color vectors
						temp = s->board[i];
						s->board[i] = s->board[j];
						s->board[j] = temp;
					}
				}
			}
		}
	}
	

	//Variables for swap analysis
	rSwapIt = s->swapCells_Goals_LTMss.rbegin();
	rSwapIt2 = s->swapCells_Goals_GTEqMss.rbegin();
	rSwapIt3 = s->swapCells_NonGoals_LTMss.rbegin();
	rSwapIt4 = s->swapCells_NonGoals_GTEqMss.rbegin();
	
	int compareScore_G_GTEqMss = rSwapIt2->first;
	int bestScore_NG_LTMss = allNonGoalsLTMss_start;
	int bestScore_NG_GTEqMss = allNonGoalsGTEqMss_start;
	int bestScore_G_LTMss = allGoalsLTMss_start;
	
	pair<int, int> bestSwap_NG_GTEqMss (rSwapIt2->second.first, rSwapIt2->second.second);
	pair<int, int> bestSwap_NG_LTMss (rSwapIt2->second.first, rSwapIt2->second.second);
	pair<int, int> bestSwap_G_LTMss (rSwapIt2->second.first, rSwapIt2->second.second);
	
	//looks at all cell swap pairs that produced the largest increase in scorable points and compares other board statistics to decide the best pair	
	while (rSwapIt2->first == compareScore_G_GTEqMss)
	{
		//find same cell from rSwapIt2->second and see if rSwapIt->first increased from initial value 
		for (rSwapIt = s->swapCells_Goals_LTMss.rbegin(); rSwapIt != s->swapCells_Goals_LTMss.rend(); ++rSwapIt)
		{
			if (rSwapIt->second == rSwapIt2->second && rSwapIt->first > bestScore_G_LTMss)
			{
				bestScore_G_LTMss = rSwapIt->first;
				bestSwap_G_LTMss = rSwapIt->second;
				break;
			}
		}
		
		//find same cell from rSwapIt2->second and see if rSwapIt3->first increased from initial value 
		for (rSwapIt3 = s->swapCells_NonGoals_LTMss.rbegin(); rSwapIt3 != s->swapCells_NonGoals_LTMss.rend(); ++rSwapIt3)
		{
			if (rSwapIt3->second == rSwapIt2->second && rSwapIt3->first > bestScore_NG_LTMss)
			{
				bestScore_NG_LTMss = rSwapIt3->first;
				bestSwap_NG_LTMss = rSwapIt3->second;
				break;
			}
		}

		//find same cell from rSwapIt2->second and see if rSwapIt4->first increased from initial value 
		for (rSwapIt4 = s->swapCells_NonGoals_GTEqMss.rbegin(); rSwapIt4 != s->swapCells_NonGoals_GTEqMss.rend(); ++rSwapIt4)
		{
			if (rSwapIt4->second == rSwapIt2->second && rSwapIt4->first > bestScore_NG_GTEqMss)
			{
				bestScore_NG_GTEqMss = rSwapIt4->first;
				bestSwap_NG_GTEqMss = rSwapIt4->second;
				break;
			}
		}
		
		++rSwapIt2;
	}

	//calculate differences from starting values
	int NG_LTMssDiff = bestScore_NG_LTMss - allNonGoalsLTMss_start;
	int NG_GTEqMssDiff = bestScore_NG_GTEqMss - allNonGoalsGTEqMss_start;
	int G_LTMssDiff = bestScore_G_LTMss - allGoalsLTMss_start;
	
	//find best swap based on chosen cells and score differences
	if (bestSwap_G_LTMss == bestSwap_NG_GTEqMss && bestSwap_NG_GTEqMss == bestSwap_NG_LTMss) 
	{rSwapIt2->second = bestSwap_G_LTMss; printSwap(rSwapIt2, s, dsRankPC);} 
	
	else if (G_LTMssDiff > NG_LTMssDiff) {rSwapIt2->second = bestSwap_G_LTMss; printSwap(rSwapIt2, s, dsRankPC);}
	else if (G_LTMssDiff > NG_GTEqMssDiff) {rSwapIt2->second = bestSwap_G_LTMss; printSwap(rSwapIt2, s, dsRankPC);}
	else if (bestSwap_NG_GTEqMss == bestSwap_NG_LTMss) {rSwapIt2->second = bestSwap_NG_GTEqMss; printSwap(rSwapIt2, s, dsRankPC);} 
	else if (NG_GTEqMssDiff > NG_LTMssDiff) {rSwapIt2->second = bestSwap_NG_GTEqMss; printSwap(rSwapIt2, s, dsRankPC);} 
	else if (NG_LTMssDiff >= NG_GTEqMssDiff) {rSwapIt2->second = bestSwap_NG_LTMss; printSwap(rSwapIt2, s, dsRankPC);} 
	else  {rSwapIt2 = s->swapCells_Goals_GTEqMss.rbegin(); printSwap(rSwapIt2, s, dsRankPC);} 
	
	//release no longer needed memory and exit	
	delete s;
	delete dsRankPC;
	
	exit(0);
}
