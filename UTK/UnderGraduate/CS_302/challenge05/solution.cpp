// Challenge 05: Graph Paths

//Tom Hills (jhills)

//Brief Description:
//Code reads in information for a directed Graph in the following format:
	//NEDGES 
	//SRC1 DST1 
	//...
	//NPATHS
	//SRC1 DST1
	//...
//where NEDGES is the number of edges, 1st set of SRC/DST pairs are source/destination nodes defining edges,... 
//...NPATHS is the number of paths, and 2nd set of SRC/DST pairs are source/destination nodes defining paths.
//The code constructs a Graph via an adjacency list (vector of list objects) and then determines if the prescribed...
//...paths exist or not and prints the results.

#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <queue>

using namespace std;

//FUNCTIONS--------------------------------------------------------------------------------------------------------------------

//prints vector of list objects
void printVecList(vector<list<string> > &vls)
{
	list<string>::iterator lit;

	for (unsigned int i = 0; i < vls.size(); ++i)
	{
		for (lit = vls[i].begin(); lit != vls[i].end(); ++lit)
		{
			printf("%5s", lit->c_str());
		}
	
		printf("\n");
	}
}

//returns adjacency list index where source node is found
//returns -1 if source is not found
int sourceIndex(string &source, vector<list<string> > &vls)
{
	for (unsigned int i = 0; i < vls.size(); ++i)
	{
		if (vls[i].front() == source) {return i;}
	}

	return -1;
}

//returns true if destination node was found in adjacency list, or false if not found
bool findDestination (string &destination, list<string> &l)
{
	list<string>::iterator lit;

	for (lit = l.begin(); lit != l.end(); ++lit)
	{
		if (*lit == destination) {return true;}
	}

	return false;
}

//pushes all destination nodes from adjacency list onto the queue
void pushDestinationNodes_BFS(queue<string> &q, list<string> &l)
{
	list<string>::iterator lit;

	lit = l.begin();//lit points to source node
	++lit;//moves iterator past source node to first destination node 

	for (; lit != l.end(); ++lit) {q.push(*lit);}
}

//returns true if a path exists between source and destination, or false if no path exists. 
bool findPath_BFS(string source, string destination, vector<list<string> > &vls) 
{
	int sIndex;
	queue<string> nodesToProcess; 
	vector<int> visited (vls.size(), 0);

	nodesToProcess.push(source);
	
	while (!nodesToProcess.empty())
	{
		//checks if front of queue == destination node
		//if not, pops it and finds source index within adjacency list, if present
		source = nodesToProcess.front();
		if (source == destination) {return true;}
		nodesToProcess.pop();
		sIndex = sourceIndex(source, vls);
		//source node doesn't have any destination nodes. No need to process any further.
		if (sIndex == -1) {continue;}
		
		//early check to see if destination node is in source's adjacency list to avoid unnecessary pushes onto the queue
		if (findDestination(destination, vls[sIndex])) {return true;}

		//if source node has not already been visited, mark as visited and push all destination nodes onto the queue
		if (!visited[sIndex])
		{
			visited[sIndex] = 1;
			pushDestinationNodes_BFS(nodesToProcess, vls[sIndex]);
		}
	}

	//destination node not found
	return false;
}

//MAIN-----------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[]) 
{
	int nEdges, nPaths, graphNo = 0;
	string source, destination;
	vector<list<string> > adjList;
	list<string> adjListElement;
	bool found;

	while (cin >> nEdges)
	{
		if (cin.fail()) {return 0;}

		++graphNo;

		//creates appropriately sized vector
		adjList.clear();
	
		//read in source and destination nodes based on nEdges	
		for (int i = 0; i < nEdges; ++i)
		{
			//inital conditions needed at the start of each source/destination node read
			found = false;

			//read in source/destination node pair
			cin >> source >> destination;

			//check if source already exists in adjList
			for (unsigned int j = 0; j < adjList.size(); ++j)
			{
				if (!adjList[j].empty() && adjList[j].front() == source) 
				{
					adjList[j].push_back(destination); 
					found = true;
					break;
				}
			}
			
			//new adjacency list needs to be created and added to adjList
			if (adjList.empty() || !found) 
			{
				adjListElement.clear();
				adjListElement.push_back(source);
				adjListElement.push_back(destination);
				adjList.push_back(adjListElement);
			}
		}

		//read in number of paths needing to be checked
		cin >> nPaths;

		for (int i = 0; i < nPaths; ++i)
		{
			cin >> source >> destination;

			//findPath_BFS() returns true if a path exists between source and destination, or false if path doesn't exist. 
			if (findPath_BFS(source, destination, adjList)) 
			{
				printf("In Graph %d there is a path from %s to %s\n", graphNo, source.c_str(), destination.c_str());
			}
			else 
			{
				printf("In Graph %d there is no path from %s to %s\n", graphNo, source.c_str(), destination.c_str());
			}
		}
	
		if (graphNo < 5) {printf("\n");}
	}
	
	return 0;
}
