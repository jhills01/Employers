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
	char tiles[] = {'f', 'g', 'G', 'h', 'm', 'r'};//tile names
	int N = atoi(argv[1]);//dimension of NxN map

	if (file.is_open())
	{
		//output file header: number of different tiles, tile names and cost, and map dimensions 
		file << "6\n";
		file << "f 3\ng 1\nG 2\nh 4\nm 7\nr 5\n";
		file << N << " " << N << "\n";
		
		//output random NxN map to file
		for (int i = 0; i < N*N; ++i)
		{
			if (i == 0) {file << tiles[rand()%6] << " ";}
			else if (i % N == 0) {file << "\n" << tiles[rand()%6] << " ";}
			else {file << tiles[rand()%6] << " ";}
		}
		
		//output runner's starting position (top left) and ending position (bottom right)
		file << "\n0 0\n";
		file << N-1 << " " << N-1 << "\n";
		
		file.close();
	}

	return 0;
}
