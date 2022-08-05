Project 04: Path Finding
========================

Project members: Tom Hills (jhills)

This project implements a [Dijkstra's Algorithm] to perform rudimentary path
finding on a 2D tile-based map.

[Dijkstra's Algorithm]: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm


| N (NxN map)| Memory Usage (Kb) [5x avg] | Elapsed Time (sec) [5x avg] |
|------------|----------------------------|-----------------------------|
| 10         |  1,244                     |      0.00                   |
| 20         |  1,252                     |      0.00                   |
| 50         |  1,304                     |      0.00                   |
| 100        |  1,440                     |      0.00        			|
| 200        |  1,864                     |      0.02         			|
| 500        |  6,352                     |      0.16    		        |
| 1000       | 21,664                     |      0.66            		|
|------------|----------------------------|-----------------------------|


## How did you represent the map as a graph?

To represent the graph, I used a vector of chars for the NxN map and a vector of ints with 256 elements where a char's ascii 
code would provide the index (idea from Dr. Plank's Superball code). The relationship between vertices was easily calculated
because edges only existed between neighboring tiles not including diagonals. This simple relationship not only allowed me to
use the same logic from superball, but saved on memory usage since I didn't need to store an adjacency list or matrix. To obtain
a tile's weight, I would use the tile's char value from the NxN sized vector of chars as an index into the vector of
ints storing each tile name's weight value (e.g., tileCostVec[map[i]] would reference 2 if map[i] == 'G'). 

## What is complexity of your implementation of Dijkstra's Algorithm?

My implementation, from a Big-O standpoint, depends entirely on the size of the multimap object and the total number of 
insertion operations performed (erase operations are also performed, but can be ignored which I explain below). Now, my 
multimap object will only contain, at any given time, one pair associated with a vertex. This is possible because I "replace" 
(i.e., erase and insert) instead of just inserting which prevents multiple pairs for one vertex (each with a different or equal
distance) existing within the multimap. Consequentially, each insertion and erase operation will have a time complexity of
O[log2(|V|)] where |V| == NxN, the total number of vertices in the graph (NOTE: erase complexity is technically listed as
"amortized constant", but for a worse case, I'll just say it's logrithmic with a small constant). Now, for the number of total
insertions and erasures, we have two cases to consider: 1) a vertex does NOT have a previous distance, 2) a vertex with a
previous distance needs to be updated with a smaller distance. For 1), |V| insertions will be performed since all vertices
start without a previous distance. For 2), c*(|E| - |V|) insertions and erasures will be performed where c is a fractional
constant and |E| = total number of edges in the graph. Since both of these cases happen independently, we can say that there
will be a total of |V| + c*|E| - c*|V| = c*|E| + (1 - c)*|V| insertions and c*|E| - c*|V| erasures. Clearly, there will be more
insertions performed. Additionally, the time complexity of the insertion operation is generally greater than erase (see my
"amortized constant" argument above) so we only have to consider the insertions in obtaining an overall time complexity which
turns out to be O[(c*|E| + (1-c)*|V|) * log2(|V|)] = O[(|E| + |V|) * log2(|V|)]. 

## How well does your implementation scale?

My implementation seems to scale quite well. I plotted the number of vertices (N^2) versus both time and memory usage 
in excel (granted, only 3 time data points are worth anything). The graphs showed a very good linear relationship with both 
having R^2 factors of 0.9999. The slope of the time graph was 0.0000007 and the memory graph was 0.0205.

## INDIVIDUAL CONTRIBUTIONS

I didn't have a partner. So yeah.....just me.....yep.
