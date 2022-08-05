// NAME: Tom Hills
// NETID: jhills
// CS 494: Lab6 - Rainbow Graph
/* DESCRIPTION:
        The purpose of this lab is to find the minimum cost of traversing a given graph with the condition
        that edges made be chosen to be negative (same magnitude) at the expense of 1 charge. The number of
        allowed charges are provided and the the traversal algortihm doesn't necessarily use all of them
        to find the minimu cost. A traversal must start at node 1 and end at node N where N is the number
        of nodes in the graph.
//*/

#include <cstdlib>
#include <iostream>
#include <sstream> 
#include <string>
#include <cstring>
#include <vector>
#include <cstdio>

#define dwv 6000000 //dwv == default weight value

using namespace std;

//Testing functions
void printIntVector(vector <int> v, string vecName)
{
    for (unsigned int i = 0; i < v.size(); ++i) printf("%s[%-4d]: %d\n", vecName.c_str(), i, v[i]);
    printf("\n");
}

void print2dIntVector(vector < vector <int> > v2d, string vecName)
{
    for (unsigned int i = 0; i < v2d.size(); ++i)
    {
        //printf("%s[%3d]: ", vecName.c_str(), i);
        
        //print members
        if (v2d[i].size() == 0) printf("\n");
        else 
        {
            for (unsigned int j = 0; j < v2d[i].size(); ++j)
            {
                //if (j < v2d[i].size() - 1) printf("%10d, ", v2d[i][j]);
                if (j < v2d[i].size() - 1) printf("%10d ", v2d[i][j]);
                else printf("%10d\n", v2d[i][j]);
            }
        }
    }
    //printf("\n");
}

void print2dLLVector(vector < vector <long long> > v2d, string vecName)
{
    for (unsigned int i = 0; i < v2d.size(); ++i)
    {
        //printf("%s[%3d]: ", vecName.c_str(), i);
        
        //print members
        if (v2d[i].size() == 0) printf("\n");
        else 
        {
            for (unsigned int j = 0; j < v2d[i].size(); ++j)
            {
                //if (j < v2d[i].size() - 1) printf("%10d, ", v2d[i][j]);
                if (j < v2d[i].size() - 1) printf("%10lld", v2d[i][j]);
                else printf("%10lld\n", v2d[i][j]);
            }
        }
    }
    //printf("\n");
}

int main()
{
    unsigned int numOfNodes, ftwSize;
    int lineCnt, tmp;
    int charges;
    long long result; 
    string s;
    stringstream ss;
    vector<int> from;
    vector<int> to;
    vector<int> weights;
    vector< vector<long long> > matrixO; //O == original
    vector< vector<long long> > matrixS; //S == shortest
    vector< vector<long long> > matrixL; //L == largest
    vector< vector<long long> > matrixA; //A == matrix A from part 2 of writeup
    vector< vector<long long> > matrixAc; //Ac == matrix A with arbitrary c charges from each node to every other node
    vector <vector<long long > > tempAc;
    vector <vector<long long > > final;
    vector <vector<long long > > finalTemp;

    //read in file from cin
    lineCnt = -1;
    while(getline(cin, s) )
    {
        ss.clear(); //BUG NOTE: Without this line, while(ss >> tmp) in case 1wasn't executing b/c ss needs reset after 
                    //          each case. So, in case 1, eof was set preventing ss >> tmp from executing 
        ss << s;
        lineCnt++;

        switch (lineCnt)
        {
            case 0: //nodes
                ss >> numOfNodes;
                break;

            case 1: //from
                while ( ss >> tmp ) from.push_back(tmp);
                break;
            
            case 2: //to
                while ( ss >> tmp ) to.push_back(tmp);
                break;

            case 3: //weights
                while ( ss >> tmp ) weights.push_back(tmp);
                break;

            case 4: //nodes
                ss >> charges;
                break;
            
            default:
                break;
        }
    }   

    ftwSize = from.size(); //same for "to" and "weights" vectors

    //PART 1--------------------------------------------------------------------------------------------------------
    //Size matrixO and matrixL
    matrixO.resize(numOfNodes); 
    for (unsigned int i = 0; i < numOfNodes; ++i) matrixO[i].resize(numOfNodes, dwv); 
    matrixL.resize(numOfNodes); 
    for (unsigned int i = 0; i < numOfNodes; ++i) matrixL[i].resize(numOfNodes, 0); 

    //create adjacency matrixO from smallest weighted edges. Remeber, an edge can have more than 1 weight assignment
    //Also, create adjacency matrixLfrom largest weighted edges. 
    for (unsigned int i = 0; i < ftwSize; ++i)
    { 
        if (matrixO[ from[i]-1 ][ to[i]-1 ] >  weights[i]) matrixO[ from[i]-1 ][ to[i]-1 ] = weights[i]; 
        if (matrixL[ from[i]-1 ][ to[i]-1 ] <  weights[i]) matrixL[ from[i]-1 ][ to[i]-1 ] = weights[i]; 
    }

    matrixS = matrixO;

    //prepare matrixS for Floyd-Warshall Algorithm by making diagonals of adjacency matrix 0s
    for (unsigned int i = 0; i < numOfNodes; ++i) matrixS[i][i] = 0;

    //perform the Floyd-Warshall Algorithm
    for (unsigned int i = 0; i < numOfNodes; ++i)
    {
        for (unsigned int x = 0; x < numOfNodes; ++x)
        {
            for (unsigned int y = 0; y < numOfNodes; ++y)
            {
                if (matrixS[x][y] > (matrixS[x][i] + matrixS[i][y]) ) matrixS[x][y] = matrixS[x][i] + matrixS[i][y];
            }
        }
    }

    if (charges == 0) { printf("%lld\n", matrixS[0][numOfNodes-1]); return 0; }//NOTE: I'm pretty sure this is it....hmmm..

    //PART 2--------------------------------------------------------------------------------------------------------
    //Establish matrixA
    matrixA = matrixS; 

    //Perform O(N^4) loop 
    for (unsigned int f = 0; f < numOfNodes; ++f)
    {
        for (unsigned int i = 0; i < numOfNodes; ++i)
        {
            for (unsigned int j = 0; j < numOfNodes; ++j)
            {
                for (unsigned int t = 0; t < numOfNodes; ++t)
                {  
                    //if (matrixA[f][t] == dwv) continue;  
                    if (matrixA[f][t] == dwv || matrixL[i][j] == 0) continue;
                    
                    result = matrixS[f][i] + (-1*matrixL[i][j]) + matrixS[j][t];
                    if (matrixA[f][t] > result) matrixA[f][t] = result;
                }
            }
        }
    }

    if (charges == 0) { printf("%lld\n", matrixA[0][numOfNodes-1]); return 0; }

    //PART 3--------------------------------------------------------------------------------------------------------
    final = matrixS;
    matrixAc = matrixA;

    while (charges > 0)
    {
        if ( charges % 2 ) //charges is odd
        {   
            finalTemp = final; 

            //perform the Floyd-Warshall Algorithm
            for (unsigned int i = 0; i < numOfNodes; ++i)
            {
                for (unsigned int x = 0; x < numOfNodes; ++x)
                {
                    for (unsigned int y = 0; y < numOfNodes; ++y)
                    {
                        if ( final[x][y] > (finalTemp[x][i] + matrixAc[i][y]) ) 
                        {final[x][y] = finalTemp[x][i] + matrixAc[i][y];}
                    }
                }
            }
        }

        //perform the Floyd-Warshall Algorithm
        tempAc = matrixAc;

        for (unsigned int i = 0; i < numOfNodes; ++i)
        {
            for (unsigned int x = 0; x < numOfNodes; ++x)
            {
                for (unsigned int y = 0; y < numOfNodes; ++y)
                {

                    if ( matrixAc[x][y] > (tempAc[x][i] + tempAc[i][y]) ) 
                    {
                        matrixAc[x][y] = tempAc[x][i] + tempAc[i][y];
                    }
                }
            }
        }

        //half the charges at the end of each iteration
        charges /= 2;
    }// End of while

//print2dLLVector(final, "final");
printf("%lld\n", final[0][numOfNodes-1]);

    return 0;
}
