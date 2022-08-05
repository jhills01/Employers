// NAME: Tom Hills
// NETID: jhills
// CS 494: Lab5 - SIMD_TheTips
/* DESCRIPTION:
        The purpose of this lab is to use Intel's SIMD extensions to speed up the Floyd-Warshall calculations
        in the solution for TheTips topcoder problem. For maximum performance benefit, the row elements of the
        adjacency matrix are represented by individual bits which allows ORing 128 elements simultaneously.  
//*/

#include <bits/stdc++.h>
#include <emmintrin.h>
#include "The-Tips.h"

using namespace std;

double TheTips::solve(vector <string> clues, vector <int> probability, int print)
{

    unsigned long long newSize;
	double v;
	vector< vector<unsigned char> > adjMatrix;
	vector<double> p;

	__m128i *p1, *p2;

	//Convert clues into an adjacency matrix
	//newSize is computed by dividing by 8 bits 
    //newSize must also be a multiple of 16
	newSize = ceil((double)clues.size() / 8);
	if (newSize % 16 != 0) newSize += 16 - (newSize % 16);
	
	//create the adjacency matrix
	adjMatrix.resize(clues.size(), vector<unsigned char>(newSize, 0));

	for (int j = 0; j < clues.size(); j++) 
    {
		for (int i = 0; i < clues[j].size(); i++) 
        {
			if (clues[j][i] == 'Y') adjMatrix[j][i / 8] |= ( 1 << (i % 8) );
		}

        adjMatrix[j][j / 8] |= ( 1 << (j % 8) );
	}
	
	//perform the Floyd-Warshall algorithm
	for (int k = 0; k < adjMatrix.size(); k++) 
    {
		for (int i = 0; i < adjMatrix.size(); i++) 
        {
			if ( (adjMatrix[i][k / 8] >> (k % 8) ) & 1) //if adjMatrix bit == 1
            {
				//establish SIMD pointers
				p1 = (__m128i*) &adjMatrix[i][0];
				p2 = (__m128i*) &adjMatrix[k][0];

				//use SIMD to OR 128 bits of the adjMatrix simultaneously...
                //...as "j" jump 16 bytes at a time
				for (int j = 0; j < newSize; j += 16, p1++, p2++)
				{ *p1 = _mm_or_si128(*p1, *p2); }
			}
		}
	}

    //compute the probabilities and store them in p
	p.resize(adjMatrix.size(), 0);
	for (int j = 0; j < adjMatrix.size(); j++) 
    {
		v = probability[j];
		v /= 100.0;

		for (int i = 0; i < adjMatrix.size(); i++)
        {
			if ( (adjMatrix[j][i / 8] >> (i % 8)) & 1 ) p[i] += ((1 - p[i]) * v);
        }
	}
	
    //Use p to create the final result and return it
	v = 0;
	for (int i = 0; i < p.size(); i++)
    { v += p[i]; }

	return v;
}