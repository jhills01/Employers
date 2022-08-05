// Challenge 7: Minimum Spanning Tree

//Tom Hills

//Brief Description:
//Reads in a NxN adjacency matrix for a graph and uses Prim's algorithm to build and print the minimum spanning tree (MST). 

///to confirm elapsed time and memory usage: usr/bin/time -f "%E %P %M" ./EXECUTABLE_NAME
///                                      OR  usr/bin/time -v ./EXECUTABLE_NAME for a full summary

#include <vector>
#include <iostream>
#include <set>
#include <climits>

using namespace std;

//FUNCTIONS---------------------------------------------------------------------------------------------------------------------------------------

void printIntVec (vector <int> &v, int col)
{
	for (int i = 0; i < (int)v.size(); ++i)
	{
		if (i == 0) {printf("%4d", v[i]);}
		else if (i%col == 0) {printf("\n%4d", v[i]);}
		else if (i == (int)v.size() - 1) {printf("%4d\n", v[i]);}
		else {printf("%4d", v[i]);}
	}
}

void printIntSet(set <int> &s)
{
	set <int>::iterator sit;

	for (sit = s.begin(); sit != s.end(); ++sit)
	{
		cout << *sit << endl;
	}
}

//MAIN------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) 
{

	int size, mIndex, mEdge, minEdge, minIndex, minVertex, mRow, mCol, vertex; 
	unsigned long long int mstWt;
	vector <int> matrix;
	vector <int> MST_vertices; 
	
	set <pair<char, char> > MST_edges;//FIRST == "smallest" letter of pair, SECOND == "largest letter of pair
	set <pair<char, char> >::iterator mstIt;
	
	set <int> notMST_vertices;
	set <int>::iterator nmstIt;

	//initial priming read and error check
	cin >> size;
	if (!cin.good()) {printf("Bad read for size of adjacency matrix. Exiting...\n"); exit(1);}
	
	while (cin.good())
	{
		//create adjacency matrix for new graph
		matrix.resize(size * size);
		
		for (int i = 0; i < (int)matrix.size(); ++i)
		{
			cin >> mIndex;
			if (!cin.good()) {printf("Bad read for element of adjacency matrix. Exiting...\n"); exit(2);}
			matrix[i] = mIndex;
		}
		
		//prepare for new graph
		MST_edges.clear();
		MST_vertices.clear();
		mstWt = 0;
		
		//put first Vertex into MST_vertices and load up notMST_vertices with vertices not in MST_vertices
		//NOTE: these two MST containers (set and vector) are storing indices from graph matrix, "matrix"
		MST_vertices.push_back(0); 
		for (int i = 1; i < size; ++i) {notMST_vertices.insert(i);}

		//start of Prim's algorithm
		while ((int)MST_vertices.size() < size)
		{
			//reset minEdge to "infinity" in order to find new minimum edge weight
			minEdge = INT_MAX;

			//search for smallest weighted edge between vertices in the MST and vertices NOT in the MST
			for (int i = 0; i < (int)MST_vertices.size(); ++i)
			{
				vertex = MST_vertices[i];
	
				//only check edges of vertices that are NOT already in the MST
				for (nmstIt = notMST_vertices.begin(); nmstIt != notMST_vertices.end(); ++nmstIt)
				{
					mIndex = vertex * size + *nmstIt;
					mEdge = matrix[mIndex];

					if (mEdge != -1 && mEdge <= minEdge)
					{
						minEdge = mEdge;
						minIndex = mIndex;
						minVertex = *nmstIt;
					}
				}
			}
		
			//update characteristics of MST 
			mstWt += minEdge;//update total weight of MST
			
			//stores MST path in alphabetical order where FIRST == "row" vertex and SECOND == "column" vertex from matrix
			mCol = minIndex % size;
			mRow = (minIndex - mCol) / size;
			if (mCol > mRow) {MST_edges.insert(pair<char, char> ('A' + mRow, 'A' + mCol));}
			else {MST_edges.insert(pair<char, char> ('A' + mCol, 'A' + mRow));}
			
			//add "column" vertex associated with minEdge to MST and remove it from non-MST vertices
			MST_vertices.push_back(minVertex);
			notMST_vertices.erase(minVertex);
		}
	
		//prints total edge weight and path (alphabetically) of  minimum spanning tree (MST)
//		printf("%d\n", mstWt); //NOTE: changed mstWt from int to unsigned long long int to test a 1000 vertex graph, each with 500 edges
		printf("%llu\n", mstWt);

		for (mstIt = MST_edges.begin(); mstIt != MST_edges.end(); ++mstIt)
		{
			printf("%c%c\n", mstIt->first, mstIt->second);
		}

		//read and check to see if final newline needs to be printed
		cin >> size;
		if (cin.good()) {printf("\n");}
		else {return 0;}
	}
}
