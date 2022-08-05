//Project 4: generate map

//Name: Tom Hills (jhills)

//Brief Description:
//Generates an NxN map given N from std input where map elements are chosen randomly from f(3), g(1), G(2), h(4), m(7), and r(5) tiles...
//...[tile weights are given in parenthesis]. These randomly generated maps will be used to benchmark "dijkstras.cpp" file in both time...
//...and memory consumption via /usr/bin/time -f "%E %P %M" <command>.

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;


int main(int argc, char* argv[])
{
	//command line arguments should be N (for NxN map) and file name
	if (argc != 3) {printf("usage: ./generate_map N filename"); return 1;}

	ofstream file (argv[2]);
	srand(time(nullptr));
	int N = atoi(argv[1]);//dimension of NxN map

	if (file.is_open())
	{
		//output file header: number of different tiles, tile names and cost, and map dimensions 
		file << N << "\n";
		
		//output random NxN map to file
		for (int i = 0; i < N*N; ++i)
		{
			if (i == 0) {file << "-1 ";}	
			else if (i % 2 == 0 && i % N != 0) {file << "-1 ";}	
			else if (i % 2 != 0 && i % N != 0) {file << (rand() % 98) + 1 << " ";}
			else {file << "\n" << "-1 ";}
		}
		
		file.close();
	}

	return 0;
}
