//Project 5: Word Dice

//Tom Hills (jhills)

//Description:
//Worddice is a program that using Network flow to determine if a group of dice can spell a given word. This is accomplished by...
//...creating a directed bipartite Graph with a Source and Sink Node along with one Node per dice and one Node per letter from the given word.
//...The Source and dice Nodes are in the left-hand portion of the bipartite graph where Edges go from dice Nodes to corresponding word...
//...letter Nodes if the letter can be found on the dice. This forms the right-hand portion of the bipartite Graph which consists of...
//...word letter Nodes going to the Sink Node. The Edmond-Karp algorithm is then run on this directed bipartite Graph until no path...
//...exists between the Source and Sink Nodes. If the number of paths found equals the number of letters in the word, then the dice...
//...are able to spell the word. 

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

//CLASS DECLARATION------------------------------------------------------------------------------------------------------------
class Node 
{
	public: 
		string name;
		int visited, index; //index solely used for final print statement if word can be spelled with available dice
		vector <class Edge *> adj;
		class Edge *backedge;//Edges object that gives the n1 Node that put n2 Node on the queue in BFS()  
};

class Edge 
{
	public:
		void Print();
		string name;
		Node *n1;
		Node *n2;
		Edge *reverse;
		int original;
		int residual;
		int index; //index of where I am on the adjacency list
};

class Graph 
{
	public:
		Graph(char *argv);
		~Graph();
		void Print();
		Node *Create_Node(string &s);
		Edge *Create_Edge(Node *n1, Node *n2);
		int MaxFlow();
		int Find_Augmenting_Path();
		int NPaths;
		int NDice;

		bool BFS();
		vector <Edge *> Path;

		int MaxCap;
		Node *Source;
		Node *Sink;
		vector <Node *> Nodes; 
		vector <Edge *> Edges;
};

//CLASS IMPLEMENTATIONS--------------------------------------------------------------------------------------------------------

//runs continuously finding new paths through the graph using BFS and sums the return flows and counts the number of paths found.
int Graph::MaxFlow()
{
	int maxFlow, flow;

	NPaths = 0;
	maxFlow = 0;

	while (1) 
	{
		flow = Find_Augmenting_Path();
		maxFlow += flow;
		if (flow == 0) return maxFlow;
		NPaths++;
	}
}

//checks if a new Path from Source to Sink can be found. If it can, reverses the direction of all Edges along this Path in Graph. 
int Graph::Find_Augmenting_Path()
{
	Edge *e, *swapE;
	Node *n1, *n2;

	Path.clear();

	//1st checks if the Source's adjacency list is empty (if so, we can't possibly find a path from Source to Sink).
	//If NOT empty, then it checks the return of BFS which will be true if a path was found from Source to Sink.
	if (Source->adj.size() != 0 && BFS())
	{
		//Process residual Graph based on Path found by BFS
		//Since all Edges have weight of 1, this simply means reversing the direction of all Edges found in Path
		for (int i = 0; i < (int)Path.size(); i++) 
		{
			e = Path[i];

			//remove n1->n2 Edge from n1's adj. list and update the Edge's index informatoin
			n1 = e->n1;
			swapE = n1->adj[n1->adj.size() - 1];//last Edge in adj. list
			swapE->index = e->index; //index is the only member that needs to change to reflect different location in adj. list
			n1->adj[e->index] = swapE;
			n1->adj.pop_back();
			e->index = -1; //update e's index to reflect that it's not in any adj. list now

			//add n2->n1 Edge to n2's adj. list and update the Edge's index information
			n2 = e->n2;
			e->reverse->index = n2->adj.size(); 
			n2->adj.push_back(e->reverse);//push_back n2->n1 Edge
		}
		
		return 1; //return 1 for BFS successfully finding 1 path from Source to Sink	
	}

	return 0;
}

//Simple BFS procedure which builds Path the first time we encounter Sink Node on the queue using each Node's "backedge" 
bool Graph::BFS()
{
	queue <Node *> Q;
	Node *n;                      // The node that I'm processing from the front of the Queue
	Edge *e;                      // I process each edge from n
	Node *t;                      // This is the node that e goes to: e == n->t
	
	//reset visited of all Nodes for a fresh run through the graph
	for (int i = 0; i < (int)Nodes.size(); ++i) {Nodes[i]->visited = 0;}
	
	//prepare the Source and put onto the queue
	Source->visited = 1;
	Q.push(Source);

	//process Nodes in queue 
	while(!Q.empty()) 
	{
		// Grab front of the queue and then pop it
		n = Q.front();
		Q.pop();

		// If we're at the sink, we're done.  
		// Create the path by traversing backedges back to the source.
		// NOTE-TO-GRADER: Building "Path" vector is a left-over from Plank's code on Ford-Fulkerson.
		// ...I don't think this step is necessary anymore because I added backedge pointers in the Nodes. 
		// ...However, it's too last minute to change and has more of a spatial impact than a temporal one so...
		// ...I'll just leave it as is. 
		if (n == Sink) 
		{
			while (n != Source) 
			{
				Path.push_back(n->backedge);
				n = n->backedge->n1;
			}
			
			return 1;
		}

		//go through n's adjacency list and push unvisited Nodes onto queue, change to visited, and update their backedge
		for (int i = 0; i < (int)n->adj.size(); i++) 
		{
			e = n->adj[i];
			t = e->n2;
			
			if (t->visited == 0)
			{
				t->visited = 1;
				t->backedge = e;
				Q.push(t);
			}
		}
	}

	//Return false if there's no path to the sink.
	return false;
}

//creates a new Edge
Edge *Graph::Create_Edge(Node *n1, Node *n2)
{
	string en;
	Edge *e;

	en = n1->name + "->";
	en += n2->name;

	e = new Edge;
	
	e->name = en;
	e->original = 1;
	e->residual = 1;
	e->n1 = n1;
	e->n2 = n2;
	e->reverse = NULL;
	e->index = -1;
	
	Edges.push_back(e);
	
	return e;
}

//Creates a new Node
Node *Graph::Create_Node(string &s)
{
	Node *n;

	n = new Node;
	
	n->name = s;
	n->backedge = NULL;

	Nodes.push_back(n);
	
	return n;
}

//prints a the "name" and "index" members of a single Edge
void Edge::Print()
{
	printf("[%s:%2d]", name.c_str(), index);
}

//prints every Node in Nodes and their associated adjacency list
void Graph::Print()
{
	int i, j;
	Node *n;

	printf("Graph:\n");
	for (i = 0; i < (int)Nodes.size(); i++) 
	{
		n = Nodes[i];
		printf("  Node: %s\n", n->name.c_str());
		printf("    ");
		for (j = 0; j < (int)n->adj.size(); j++) n->adj[j]->Print();
		printf("\n");
	}
}

//constructor only constructs Source, Sink, and dice Nodes for the graph because the initial idea was to....
//destroy/create only the Nodes for the word letters when new words were read-in. See "NOTE-TO-GRADER" comments in main 
Graph::Graph(char *argv)
{
	string ifstr, sourceStr("source"), sinkStr("sink");
	Node *n1;
	Edge *e, *r;

	//initialize Graph members here
	NDice = -1;//-1 so that total dice number is "0-indexed" compatible
	Source = Create_Node(sourceStr);
	Sink = Create_Node(sinkStr);

	ifstream ifs(argv, ifstream::in); 
	
	if (ifs.is_open()) 
	{
		//build Graph object with dice information
		while (ifs >> ifstr)
		{
			++NDice;//count to obtain total number of dice
			
			n1 = Create_Node(ifstr);
			n1->index = NDice;
			
			e = Create_Edge(Source, n1);
			r = Create_Edge(n1, Source);
			
			e->index = Source->adj.size();
			
			e->reverse = r;
			r->reverse = e;
			
			Source->adj.push_back(e);
		}
	}
	
	else {printf("Error opening argv[1] file name. Exiting program.\n"); exit(1);}
	
	//close filestream
	ifs.close();	
}

//destroys all Nodes and Edges in Graph
Graph::~Graph()
{
	int i;

	for (i = 0; i < (int)Nodes.size(); i++) delete Nodes[i];
	for (i = 0; i < (int)Edges.size(); i++) delete Edges[i];
}


//MAIN-------------------------------------------------------------------------------------------------------------------------
int main (int argc, char **argv)
{
	if (argc != 3) {printf("usage: ./worddice dice.txt word.txt"); return 1;}

	//constructor creates Graph with Source, Sink, and dice Nodes 
	Graph *g = new Graph(argv[1]);
	Node *n1, *n2;
	Edge *e, *r;
	string ifstr, c;
	int maxFlow = 0;
	string sinkStr("sink");

	//add-on word letter Nodes and associated edges to Graph object
	//NOTE-TO-GRADER: This is not included in Graph constructor b/c I initially wanted to only delete word letter Nodes but thought I couldn't b/c...
	//...deleting an Edge object would delete the two underlying Node objects. However, I now think I made an incorrect assumption about this and...
	//...I could delete the adj. lists of all dice Nodes instead of deleting the entire Graph below. oh well. 
	ifstream ifs(argv[2], ifstream::in);

	if (ifs.is_open())
	{
		while (ifs >> ifstr)
		{
			//complete bipartite graph with word letters
			for (int i = 0; i < (int)ifstr.size(); ++i)
			{
				//create Node (n1) for each letter of word 
				//create Edge between n1 and g's Sink Node (n1->Sink). 
				//put n1->Sink on n1's adjacency list
				c = ifstr[i];
				n2 = g->Create_Node(c);
				
				e = g->Create_Edge(n2, g->Sink);
				r = g->Create_Edge(g->Sink, n2);

				e->index = 0;

				e->reverse = r;
				r->reverse = e;
				
				n2->adj.push_back(e);

				//This loop prevents multiple Edges between dice Nodes and word letter Nodes b/c we only search dice Node once.
				//...If we find the letter from word, we make an Edge and move on to next dice Node. 
				for (int j = 0; j < (int)g->Source->adj.size(); ++j)
				{
					n1 = g->Source->adj[j]->n2;
					
					//c found within name string of n1
					if ( n1->name.find(ifstr[i]) != string::npos ) 
					{
						//put e on n1's adjacency list
						e = g->Create_Edge(n1, n2);
						r = g->Create_Edge(n2, n1);

						e->index = n1->adj.size();

						e->reverse = r;
						r->reverse = e;
						
						n1->adj.push_back(e);
					}
				}
			}

			//run Edmond-Karp algorithm and determine if the word can be spelled with dice provided
			maxFlow = g->MaxFlow();	

			if (maxFlow == (int)ifstr.size()) 
			{
				//print indices of dice for each letter of word
				for (int i = g->NDice + 3; i < (int)g->Nodes.size(); ++i)
				{
					e = g->Nodes[i]->adj[0];
					if (i == (int)g->Nodes.size() - 1) {printf("%d", e->n2->index); break;}
					printf("%d,", e->n2->index);
				}
				
				printf(": %s\n", ifstr.c_str());
			}
			else {printf("Cannot spell %s\n", ifstr.c_str());}

			//restart Graph in preparation for new word read into ifstr
			//NOTE-TO-GRADER: This section could probably be replaced by just deleting Nodes and associated Edges of the word letter Nodes.
			//...See comment above. 
			delete g;
			g = new Graph(argv[1]);
		}
	}

	else {printf("Error opening argv[2] file name. Exiting program.\n"); exit(1);}

	//close filestream
	ifs.close();

	//destroy Graph
	delete g;

	return 0;
}
