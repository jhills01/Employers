// NAME: Tom Hills
// NETID: jhills
// CS 494: Lab5 - SIMD_APFlow
/* DESCRIPTION:
        The purpose of this lab is to use Intel's SIMD extensions to speed up the Floyd-Warshall calculations
        in the solution for Dr. Plank's APFlow problem. 
//*/

#include <emmintrin.h>
#include "AP-Flow.h"

using namespace std;

void APFlow::CalcFlow()
{
	__m128i alli, vv, fv, iv, rv;

    //copy adjacency to Flow
	for (int i = 0; i < N*N; i++) { Flow[i] = Adj[i]; }

    //Perform the Floyd-Warshall algorithm using 128 SIMD extensions
	for (int v = 0; v < N; v++) 
    {
		for (int i = 0; i < N; i++) 
        {
            /* Create a vector alli, which is 16 instances of Flow[i*N+v] */
			alli = _mm_set1_epi8(Flow[(i*N) + v]);

			for (int j = 0; j < N; j += 16) 
            {
				/* Load Flow[v*N+j] through Flow[v*N+j+15] to vector vv */
				vv = *(__m128i*) &Flow[(v*N) + j];

				/* Create fv, which is the flow from i to each of j through j+15
				   through v. This is simply the min of alli and vv. */
				fv = _mm_min_epu8(alli, vv);

				/* Load Flow[i*N+j] through Flow[i*N+j+15] to vector iv */
				iv = *(__m128i*) &Flow[(i*N) + j];

				/* Create rv, which is the max of each value of fv and iv. */
				rv = _mm_max_epu8(fv, iv);

				/* Store rv into Flow[i*N+j] through Flow[i*N+j+15] */
				*(__m128i*) &Flow[(i*N) + j] = rv;
			}   
		}
	}
}
