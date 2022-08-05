// NAME: Tom Hills
// NETID: jhills
// CS 494: Lab6 - Rainbow Graph
/* DESCRIPTION:
        With a given graph containing subgraphs of nodes with all the same color, the goal of this lab
        is to find all "legal walks" through the graph. A "walk" in the graph is a path that visits every 
        node in the graph exactly once, and may be partitioned into subpaths, where the nodes in each 
        subpath all have the same color. Likewise, a "legal walk" is one where there are no two subpaths 
        that contain nodes of the same color.
//*/

#include "RG.h"

using namespace std;

//globals purely for printing if Verbose contains 'D'
int spacesB4;
int spacesB4NIP;
bool printDVerbose = false;

//This value is crucial to get final answer b/c it prevents long long overflow in NumWalks
//It must be applied to all long long return values from NumWalks() and
//all other values derived from those returns
//Turns out, this is standard practice for most competitive coding sites
const long long preventLLOverflow = 1000000007;

//Test prints
void printIntVector(vector <int> v, string vecName)
{
    printf("%s:\n", vecName.c_str());
    for (int i = 0; i < v.size(); ++i) printf("v[%-4d]: %d\n", i, v[i]);
    printf("\n");
}

void print2dIntVector(vector < vector <int> > v2d, string vecName)
{
    for (int i = 0; i < v2d.size(); ++i)
    {
        printf("%s[%3d]: ", vecName.c_str(), i);
        
        //print members
        if (v2d[i].size() == 0) printf("\n");
        else 
        {
            for (int j = 0; j < v2d[i].size(); ++j)
            {
                if (j < v2d[i].size() - 1) printf("%d, ", v2d[i][j]);
                else printf("%d\n", v2d[i][j]);
            }
        }
    }
    printf("\n");
}

//Finds and returns the number of legal walks in the graph
int RainbowGraph::countWays(vector <int> color, vector <int> a, vector <int> b)
{
    int currColor = color[0];
    int colorSize = color.size(), abSize = a.size();
    int maxSubgraphSize;
    int setid;
    long long legalPaths = 0;

    Color = color; //make copy of color

    //establish V and NP vectors
    V.resize(colorSize);
    NP.resize(colorSize);
    for (int i = 0; i < colorSize; ++i) 
    {
        NP[i].resize(colorSize);
        for (int j = 0; j < colorSize; ++j) NP[i][j]= 0;
    }
    
    //determine and set outter vector sizes of Same, Diff, and CNodes
    Same.resize(colorSize);
    Diff.resize(colorSize);

    for (int i = 1; i < colorSize; ++i)
    { if (color[i] > currColor) currColor = color[i]; }
    CNodes.resize(currColor + 1);

    //build Same, Diff, and CNodes
    currColor = color[0];
    for (int nodeNum = 0; nodeNum < colorSize; ++nodeNum)
    {
        //build CNodes
        if (color[nodeNum] == currColor) CNodes[currColor].push_back(nodeNum);
        else {currColor = color[nodeNum]; CNodes[currColor].push_back(nodeNum);}

        //build adjacency lists
        for (int i = 0; i < abSize; ++i)
        {
            //build Same adjacency list for each node
            if (a[i] == nodeNum && color[b[i]] == color[nodeNum]) Same[nodeNum].push_back(b[i]);
            else if (b[i] == nodeNum && color[a[i]] == color[nodeNum]) Same[nodeNum].push_back(a[i]);

            //build Diff adjacency list for each node
            else if (a[i] == nodeNum && color[b[i]] != color[nodeNum]) Diff[nodeNum].push_back(b[i]);
            else if (b[i] == nodeNum && color[a[i]] != color[nodeNum]) Diff[nodeNum].push_back(a[i]);
        }
    }

    //Part 1: Call enhanced DFS routine on each node in graph using CountPaths
    if (Verbose.find('D') != string::npos)
    {
        maxSubgraphSize = CNodes[0].size();
        for (int i = 1; i < CNodes.size(); ++i) 
        {if (maxSubgraphSize < CNodes[i].size()) maxSubgraphSize = CNodes[i].size();} 
        spacesB4 = 0;
        spacesB4NIP = maxSubgraphSize*3+3;//when maxSubgraphSize == 4, Dr. Plank's spaces == 15, or maxSubgraphSize*3+3
    }

    for (int nodeNum = 0; nodeNum < colorSize; ++nodeNum)
    {
        //setup for recursive CountPath()
        Source = nodeNum;
        Target = CNodes[ color[nodeNum] ].size(); //BUG NOTE: I had "nodeNum" as CNode index instead of "color[nodeNum]"
        for (int i = 0; i < colorSize; ++i) V[i] = 0; //zero out V
        NIP = 0;

        //calling recursive CountPath() for currently selected node
        CountPaths(nodeNum);
    }

    if (Verbose.find('N') != string::npos)
    {
        for (int i = 0; i < NP.size(); ++i)
        {
            for (int j = 0; j < NP[i].size(); ++j)
            {
                if (NP[i][j] != 0) printf("NP[%d][%d] = %d\n", i, j, NP[i][j]);
            }
        }
    }

    //Part 2: (NOTE: Dr. Plank's writeup states "long long NumWalks(int starting_node, int remaining_components)")
    //  int "setid" uses individual bits to designate colored subgraphs by the following scheme:
    //      "setid" bit 0 corresponds to (c.t.) CNode[0], "setid" bit 1 c.t. CNode[1], "setid" bit 2 c.t. CNode[2], etc
    //      If "setid" bit == 1, then corresponding subgraph is in "remaining_components".
    //      If "setid" bit == 0, then "starting_node" is a member of the corresponding subgraph 
    
    Cache.resize(colorSize);
    for (int i = 0; i < colorSize; ++i) 
    {
        Cache[i].resize(1024);
        for (int j = 0; j < 1024; ++j) Cache[i][j] = -1;
    }

    setid = 0;
    for (int i = 0; i < CNodes.size(); ++i)  
    {if (CNodes[i].size() != 0) setid += 1 << i;}

    for (int nodeNum = 0; nodeNum < colorSize; ++nodeNum)
    {
        setid -= 1 << color[nodeNum];
        legalPaths += NumWalks(nodeNum, setid); 
        setid += 1 << color[nodeNum];
    }

    if (Verbose.find('C') != string::npos)
    {
        for (int i = 0; i < Cache.size(); ++i)
        {
            for (int j = 0; j < 1024; ++j)
            {
                if (Cache[i][j] != -1) printf("Cache[%d][0x%x] = %lld\n", i, j, Cache[i][j]);
            }
        }
    }

    return legalPaths % preventLLOverflow;
}

// This is the extended DFS.  Set Source, Target, V and NIP
// before you call CountPaths(Source) to set NP[Source][j].
void RainbowGraph::CountPaths(int n)
{
    //NOTE: Dr. Plank warned that Verbose == D checks might slow down code too much
    //      So, I just created a one time branch separating D code from non-D code
    int nextNode;

    if (printDVerbose)
    {
        if (Verbose == "D") 
        {
            printf("%*sNIP:%d -- Begin.  Increment NIP and Set V[%d] to 1.",
                    spacesB4NIP, "", NIP, n);
            printf("\r%*sDFS(%d):", spacesB4, "", n);
            printf("\n"); //newline automatically flushes stdout buffer

            // printf("%*sNIP:%d -- Begin.  Increment NIP and Set V[%d] to 1.\r%*sDFS(%d):\n",
            //         spacesB4NIP, "", NIP, n, spacesB4, "", n);
        }

        //setup
        NIP++;
        V[n] = 1;

        //Base Case --- check if nodes in path, NIP, equals all nodes in subgraph
        if (NIP == Target) //BUG NOTE: Originally, I had "CNodes[n].size()" instead of "Target".
        {
            if (Verbose == "D") 
            {
                printf("%*sNIP:%d -- Setting NP[%d][%d] to %d.",
                        spacesB4NIP, "", NIP, Source, n, NP[Source][n]+1);
                printf("\r%*sDFS(%d):", spacesB4, "", n);
                printf("\n"); //newline automatically flushes stdout buffer
            }
            
            NP[Source][n]++;
            NIP--;

            if (Verbose == "D") 
            {
                printf("%*sNIP:%d -- Done. Setting V[%d] = 0.", spacesB4NIP, "", NIP, n);
                printf("\r%*sDFS(%d):", spacesB4, "", n);
                printf("\n"); //newline automatically flushes stdout buffer
            }

            V[n] = 0;

            return;
        }

        if (Verbose == "D") 
        {
            printf("%*sNIP:%d -- Will call DFS on: ", spacesB4NIP, "", NIP);
            for (int i = 0; i < Same[n].size(); ++i)
            {
                if (V[ Same[n][i] ] != 1) printf("%d ", Same[n][i]);
            } 
            printf("\r%*sDFS(%d):", spacesB4, "", n);
            printf("\n"); //newline automatically flushes stdout buffer
        }
        
        //RECURSIVE LOOP --- calls all unvisited nodes in Same adjacency list
        for (int i = 0; i < Same[n].size(); ++i)
        {
            //Determine which Node to send to recursive call
            //Nodes that have already been visited need to be skipped in the loop
            if (V[ Same[n][i] ] != 1) nextNode = Same[n][i];
            else continue; //BUG NOTE: Forgot to skip recursive call below for already visted nodes

            if (Verbose == "D") 
            {
                printf("%*sNIP:%d -- Calling DFS(%d)", spacesB4NIP, "", NIP, nextNode);
                printf("\r%*sDFS(%d):", spacesB4, "", n);
                printf("\n"); //newline automatically flushes stdout buffer

            }

            //RECURSIVE CALL
            spacesB4 += 2; 
            CountPaths(nextNode);
            spacesB4 -= 2; 
        }

        NIP--; 

        if (Verbose == "D") 
        {
            printf("%*sNIP:%d -- Done. Setting V[%d] = 0.", spacesB4NIP, "", NIP, n);
            printf("\r%*sDFS(%d):", spacesB4, "", n);
            printf("\n"); //newline automatically flushes stdout buffer
        }

        V[n] = 0;

    }
    
    else if (!printDVerbose) /*Same code as above, except no verbose D checks*/
    {
        //setup
        NIP++;
        V[n] = 1;

        //Base Case --- check if nodes in path, NIP, equals all nodes in subgraph
        if (NIP == Target) //BUG NOTE: Originally, I had "CNodes[n].size()" instead of "Target".
        {
            NP[Source][n]++;
            NIP--;
            V[n] = 0;

            return;
        }

        //RECURSIVE LOOP --- calls all unvisited nodes in Same adjacency list
        for (int i = 0; i < Same[n].size(); ++i)
        {
            //Determine which Node to send to recursive call
            //Nodes that have already been visited need to be skipped in the loop
            if (V[ Same[n][i] ] != 1) nextNode = Same[n][i];
            else continue; //BUG NOTE: Forgot to skip recursive call below for already visted nodes

            //RECURSIVE CALL
            CountPaths(nextNode);
        }

        NIP--; 
        V[n] = 0;
    }
}

// Number of walks starting at node node that 
// still need to go through the nodes in setid.
long long RainbowGraph::NumWalks(int node, int setid)
{
    // if Cache[node][setid] already holds a cached result, we can simply return that value
    // instead of re-solving the same problem again
	if (Cache[node][setid] != -1) return Cache[node][setid];
    
    long long nodeL, nodeM, NPsum = 0;

    //change Cache[node][setid] from -1 to 0
    Cache[node][setid] = 0; 

    //Base case
    if (setid == 0) 
    {
        for (int i = 0; i < CNodes[ Color[node] ].size(); ++i)
        {
            nodeM = CNodes[ Color[node] ][i];
            NPsum += NP[node][nodeM];
        }

        Cache[node][setid] = NPsum % preventLLOverflow;
        return Cache[node][setid];
    }

    //For all nodes, m, in node's adjacency list who's NP[n][m] value > 0, 
    //recursively call NumWalks on all intercomponent edges
    for (int i = 0; i < CNodes[ Color[node] ].size(); ++i)
    {
        nodeM = CNodes[ Color[node] ][i]; 
        
        if (NP[node][nodeM] > 0)
        {
            for (int j = 0; j < Diff[nodeM].size(); ++j)
            {
                nodeL = Diff[nodeM][j];
                
                //check if nodeL's color is in "remaining_components" determined by bit in setid
                //if nodeL's color is NOT, then we continue
                if ( ( (setid >> Color[nodeL]) & 1 ) == 0) continue;

                Cache[node][setid] += (long long)NP[node][nodeM] * 
                                      (NumWalks(nodeL, setid - (1 << Color[nodeL])) % preventLLOverflow);
            }        
        } 
    }

    Cache[node][setid] %= preventLLOverflow;
    return Cache[node][setid];    
}
