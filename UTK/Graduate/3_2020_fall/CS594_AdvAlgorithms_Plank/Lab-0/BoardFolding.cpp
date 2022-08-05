#include <cstdlib>
#include <iostream>
#include <sstream> 
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>

using namespace std;

//v is vector, and s is the vector name
void printIntVector(vector<int> &v, string s)
{
    for (int i = 0; i < v.size(); ++i) printf("%s[%d]: %d\n", s.c_str(), i, v[i]);
    printf("\n");
}

//prints vector of strings
void printStrVector(vector<string> &v, string s)
{
    for (int i = 0; i < v.size(); ++i) printf("%s[%d]: %s\n", s.c_str(), i, v[i].c_str());
    printf("\n");
}

//"gridRowNums" contains each string row enumerated with an integer for faster comparison
//"gridRowStartNums" will end up containing 1 if corresponding row from "gridRowNums" can being a starting row
//NOTE: "colNums" from "howMany" has been transposed such that the same algorithm for rows works for both
void findStartingPlaces(vector<int> &gridRowNums, vector<int> &gridRowStartNums)
{
    for (int i = 1; i < gridRowStartNums.size()-1; ++i)
    {
        for (int j = 0; ( i-j-1 >= 0 ) && ( i+j < gridRowStartNums.size() ); ++j)
        {
            if ( (gridRowNums[i-j-1] == gridRowNums[i+j]) && (gridRowStartNums[i-j-1] == 1) ) 
            {
                gridRowStartNums[i] = 1;
                break;
            }
            else if (gridRowNums[i-j-1] != gridRowNums[i+j]) 
            {
                gridRowStartNums[i] = -1; //TEST: since vector is automatically filled w/ 0's, this helps ID which ones 
                                          //      the code touched
                break;
            }
        } 
    } 
}

//reverse contents of a int vector
void reverseIntVector(vector<int> &v)
{
    vector<int>vReversed(v.size());
    for (int i = 0; i < v.size(); ++i) vReversed[(v.size()-1) - i] = v[i];
    v = vReversed;
}

//rowNums and colNums vectors, together, should give all info contained in grid
//finds starting and ending places of folds in grid
vector< vector<int> > findStartingEndingPlaces(vector<int> &rowNums, vector<int> &colNums)
{
    vector <vector<int> > rv(4); 

    rv[0] = vector<int>( rowNums.size()+1 ); //STARTING ROWS marked 1 (0 otherwise)
    rv[1] = vector<int>( rv[0].size()     ); //ENDING ROWS marked 1 (0 otherwise)
    rv[2] = vector<int>( colNums.size()+1 ); //STARTING COLUMNS marked 1 (0 otherwise)
    rv[3] = vector<int>( rv[2].size()     ); //ENDING COLUMNS marked 1 (0 otherwise)

    //initialize rv values already known
    rv[0][0] = 1; 
    rv[0][ rowNums.size() ] = 0; 
    //--------------------------
    rv[1][0] = 1; 
    rv[1][ rowNums.size() ] = 0;
    //--------------------------
    rv[2][0] = 1; 
    rv[2][ colNums.size() ] = 0; 
    //--------------------------
    rv[3][0] = 1; 
    rv[3][ colNums.size() ] = 0; 

    //loop to find all STARTING places
    findStartingPlaces(rowNums, rv[0]);
    findStartingPlaces(colNums, rv[2]);

    //loop to find all ENDING places
    //first, rowNums and colNums need to be reversed b4 passing to findStartingPlaces.
    reverseIntVector(rowNums);
    reverseIntVector(colNums);
    findStartingPlaces(rowNums, rv[1]);
    findStartingPlaces(colNums, rv[3]);

    //finally, rv[1] and rv[3] vectors need to be reversed themselves
    reverseIntVector(rv[1]);
    reverseIntVector(rv[3]);

    return rv;
}

//enumerates strings into a single integer for faster comparison between rows/columns
void enumerateStringRowsToInt(vector<int> &gridNums, vector<string> &gridStr)
{
    int currNum; //stores the next available integer to be assigned to either a row or col.

    gridNums[0] = 1;

    //ROWS (the ith row is the row needing determination, and jth row is a previously determined row)
    currNum = 2;
    for (int i = 1; i < gridStr.size(); ++i)
    {   
        gridNums[i] = currNum; //assume we have a never-before-seen row until proven otherwise
        
        for (int j = i-1; j >= 0; --j)
        {
            //if rows i and j are the same, assign same row number and don't increment "currNum"
            if ( !gridStr[i].compare(gridStr[j]) ) 
            {
                gridNums[i] = gridNums[j];
                break;
            }
            //if j becomes first row and rows i and j don't match, increment currNum as that number...
            //...has been assigned to a new row
            else if ( gridStr[i].compare(gridStr[j]) && j == 0) ++currNum;
        }
    }
}

//counts the number of folds within the grid
long long cntFolds(vector<int> &foldStarts, vector<int> &foldEnds)
{
    long long cnt = 0;

    for (int i = 0; i < foldStarts.size(); ++i)
    {
        if (foldStarts[i] == 1)
        {
            for (int j = i+1; j < foldEnds.size(); ++j) if (foldEnds[j] == 1) ++cnt;
        }
    }

    return cnt;
}

//CLASS OBJECTS/METHODS---------------------------------------------------------
class BoardFolding
{
    public:
        //int howMany (int rows, int cols, vector<string> grid);
        long long howMany (int rows, int cols, vector<string> grid);
};

//counts how many unique folds can be found in grid
long long BoardFolding::howMany(int rows, int cols, vector<string> grid)
{
    vector<int>rowNums( grid.size() );
    vector<int>colNums( grid[0].length() );
    vector<string>gridTransposed( colNums.size() );
    vector< vector<int> > startEndPlaces;
    
    //create "gridTransposed"
    for (int i = 0; i < gridTransposed.size(); ++i)
    {
        //transpose loop where grid rotates 90 degrees clockwise
        for (int j = rowNums.size()-1; j >= 0; --j)
        {
            gridTransposed[i].push_back(grid[j].at(i));
        }
    }
    
    //enumerate rows and columns of grid 
    enumerateStringRowsToInt(rowNums, grid);
    enumerateStringRowsToInt(colNums, gridTransposed);
 
    //find STARTING and ENDING fold places in grid
    startEndPlaces = findStartingEndingPlaces(rowNums, colNums);

    return cntFolds(startEndPlaces[0], startEndPlaces[1]) * cntFolds(startEndPlaces[2], startEndPlaces[3]);
}
