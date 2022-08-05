//Project 4: Shortest Path using Dijkstra's Algorithm

//Name: Tom Hills (jhills)

//Brief Description:
//This program reads in and creates a 2d map of tiles. A runner is given with a starting...
//...position within the map and a destination position. The program then determines, via...
//Dijkstras algorithm, the shortest for the runner from starting to destination positions. 

#include <iostream>
#include <map>
#include <vector>
#include <queue> 

using namespace std;

//FUNCTIONS---------------------------------------------------------------------------------------------------------------------------

//test print for "map" and "tileCostVec" vectors
void printTiles_Cost (vector <char> &tiles, vector<int> &vc)
{
	for (unsigned int i = 0; i < tiles.size(); ++i) 
	{
		printf("vc[%c]: %d\n", tiles[i], vc[tiles[i]]);
	}
}

//test print for displaying NxN map 
void printMap (vector<char> &vc, int mapRows, int mapCols)
{
	for (int i = 0; i < mapRows; ++i) 
	{
		for (int j = 0; j < mapCols; ++j) 
		{
			printf("%c ", vc[i*mapCols + j]);
		}
		printf("\n");	
	}
}

//created to help with repeating code logic in "dijkstra" function
//create new or update existing distances as necessary and insert unvisited tile into multimap (acts as priority queue)...
//...ONLY if there was no previous distance OR a shorter distance was calculated
void dijkstra_helper(vector <int> &tileCostVec, vector <char> &map, vector <pair<int, int> > &distBEdge, multimap <int, int> &mmq, 
		int currIndex, int neighborIndex, vector <multimap<int, int>::iterator > &mmItVec)
{
	int neighborCost = tileCostVec[map[neighborIndex]];
	int cPlusNCost = distBEdge[currIndex].first + neighborCost;//disBEdge[currIndex].first will never be -1

	//neighborIndex didn't have a previous distance
	if (distBEdge[neighborIndex].first == -1)
	{
		distBEdge[neighborIndex].first = cPlusNCost;
		distBEdge[neighborIndex].second = currIndex;
		
		mmItVec[neighborIndex] = mmq.insert(pair<int, int> (cPlusNCost, neighborIndex)); //inserts and returns iterator to location
	}
	
	//previous distance is larger than newly calculated distance  
	else if (distBEdge[neighborIndex].first > cPlusNCost)
	{
		distBEdge[neighborIndex].first = cPlusNCost;
		distBEdge[neighborIndex].second = currIndex;

		mmq.erase(mmItVec[neighborIndex]); //erasing keeps the multimap from growing past the number of vertices
		mmItVec[neighborIndex] = mmq.insert(pair<int, int> (cPlusNCost, neighborIndex)); 
	}
}

//Determines shortest path from starting index to all other indices in map vector containing gameboard tiles
//shortest paths are calculated by accumulating the total weight cost of all tiles in the shortest path 
void dijkstra(vector <int> &tileCostVec, vector <char> &map, vector <pair<int, int> > &distBEdge, int currIndex, int mapCols)
{
	vector <bool> visited(map.size(), false);
	multimap <int, int> mmq; //first == accumulated distance from source tile , second == corresonding index of source tile from "map" vector
	int top, bottom, left, right, leftRow, leftCol, rightRow, rightCol, currRow, currCol;
	
	vector <multimap<int, int>::iterator > mmItVec (map.size());//initialize it to the number of vertices
	
	//Initalize with runner's starting tile 
	mmq.insert(pair<int, int> (0, currIndex));//starting index has 0 distance from itself
	distBEdge[currIndex].first = tileCostVec[map[currIndex]];//update starting index's distance to the cost of leaving starting index
		
	while (!mmq.empty())
	{
		//store and erase tile to be processed
		currIndex = mmq.begin()->second;
		mmq.erase(mmq.begin());

		//don't process tiles that have already been visited. Otherwise, mark as visited. 
		if(visited[currIndex]) {continue;}
		visited[currIndex] = true;

		//calculate indices of neighbors
		top = currIndex - mapCols;
		bottom = currIndex + mapCols;
		left = currIndex - 1;
		right = currIndex + 1;

		//calculate column and row values corresponding to the gameboard matrix (NOTE: column result needed for row calculation)
		if (left < 0) {leftRow = -1;}//left == -1 (i.e., currIndex == 0) resulted in leftRow == 0 which incorrectly triggered 
									 //...associated IF below. Otherwise, calculation is correct.
		else
		{
			leftCol = left % mapCols;
			leftRow = (left - leftCol)/mapCols;
		}
		
		rightCol = right % mapCols;
		rightRow = (right - rightCol)/mapCols;
		currCol = currIndex % mapCols;
		currRow = (currIndex - currCol)/mapCols;

		//dijkstra_helper function: 
		//updates distance and inserts neighbor if no previous distance existed or erases/inserts neighbor if a shorter distance is found 

		//IFs: executes dijkstra_helper if neighbor is a valid index in gameboard and hasn't been visited before
		if (top >= 0 && !visited[top]) {dijkstra_helper(tileCostVec, map, distBEdge, mmq, currIndex, top, mmItVec);}
		if (currRow == leftRow && !visited[left]) {dijkstra_helper(tileCostVec, map, distBEdge, mmq, currIndex, left, mmItVec);}
		if (currRow == rightRow && !visited[right]) {dijkstra_helper(tileCostVec, map, distBEdge, mmq, currIndex, right, mmItVec);}
		if (bottom < (int)map.size() && !visited[bottom]) {dijkstra_helper(tileCostVec, map, distBEdge, mmq, currIndex, bottom, mmItVec);}
	}
}

//MAIN---------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) 
{
	vector <int> tileCostVec(256, 0);
	vector <char> map;
	vector <pair<int, int> > distBEdge;//first == smallest accumulated weight distance from runner's starting index to destination index,
									   //second == source index responsible for smallest accumulated weight distance for destination index		
	vector <int> shPath;
	int numOfTiles, tileCost, mapRows, mapCols, rStartRow, rStartCol, rEndRow, rEndCol, rStartIndex, rEndIndex;
	int iRow, iCol;
	char tileName;

	while (cin >> numOfTiles)
	{
		//precautionary check to ensure while loop stops as soon as file input ends
		if (cin.fail()) {break;}
	
		//store tile cost indexed by tile name
		for (int i = 0; i < numOfTiles; ++i)
		{
			cin >> tileName >> tileCost;
			if (cin.fail()) {printf("Bad read for tileName or tileCost. Exiting...\n"); exit(1);}
			tileCostVec[tileName] = tileCost;
		}

		//initialize map related vectors  
		cin >> mapRows >> mapCols;
		map.resize(mapRows * mapCols);
		distBEdge.resize(map.size(), pair<int, int> (-1, -1));
		shPath.resize(map.size(), -1); 
		
		//create map
		for (int i = 0; i < mapRows * mapCols; ++i)
		{
			cin >> tileName;
			if (cin.fail()) {printf("Bad read during map creation. Exiting...\n"); exit(2);}
			map[i] = tileName;
		}

		//runner start and ending map coordinates
		cin >> rStartRow >> rStartCol >> rEndRow >> rEndCol;
		if (cin.fail()) {printf("Bad read for runner's starting or ending coordinates. Exiting...\n"); exit(3);}
	}

	//convert start and end row/col values to map indices  
	rStartIndex = rStartRow * mapCols + rStartCol;
	rEndIndex = rEndRow * mapCols + rEndCol;
	
	//calculate shortest distances from starting index to all map indices and store results in distBEdge 
	dijkstra(tileCostVec, map, distBEdge, rStartIndex, mapCols);
	
	//transfer shortest path from rEndIndex to rStartIndex to another vector so we can print in reverse
	int rIndex = rEndIndex;
	int spIndex = 0;

	shPath[spIndex] = rIndex; 

	while (rIndex != rStartIndex)
	{
		rIndex = distBEdge[rIndex].second;
		++spIndex;
		shPath[spIndex] = rIndex;
	}

	//resize so that shPath only contains valid map indices
	shPath.resize(spIndex + 1);

	//print shortest path total weight and traversed indices from rStartIndex to rEndIndex 
	printf("%d\n", distBEdge[rEndIndex].first - tileCostVec[map[rEndIndex]]);
	
	for (int i = shPath.size() - 1; i >= 0; --i)
	{
		iCol = shPath[i] % mapCols;
		iRow = (shPath[i] - iCol)/mapCols;
		printf("%d %d\n", iRow, iCol);
	}

	return 0;
}

