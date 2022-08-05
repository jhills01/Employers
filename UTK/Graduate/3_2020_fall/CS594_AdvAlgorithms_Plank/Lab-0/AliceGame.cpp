#include <cstdlib>
#include <iostream>
#include <sstream> 
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>

using namespace std;

//NON-CLASS OBJECTS/METHODS------------------------------------------------------
//finds maximum "h" value (0 <= h <= r) using binary search which satisfies...
//... 0 <= aScore - (2*rnds*h - h*h) <= 2*(rnds-h) + 1
double findMaxHVal(long long aScore, long long rnds)
{
    long long h = rnds;
    long long leftH, rightH; //left bounding h and right bounding h for binary search

    //COMPARISON: 0 <= aScore - (2*rnds*h - h*h) <= 2*(rnds-h) + 1
    //left == 0, middle == aScore - (2*rnds*h - h*h), right == 2*(rnds-h) + 1
    long long leftCompare, middleCompare, rightCompare; 
    
    leftCompare = 0;
    middleCompare = aScore - (2*rnds*h - h*h);
    rightCompare = 2*(rnds-h) + 1;

    //check if max possible h value satisfies
    if (leftCompare <= middleCompare && middleCompare <= rightCompare) return h;

    //binary search start
    leftH = 0;
    rightH = rnds - 1;
    h = leftH + (rightH - leftH)/2;

    //COMPARISON: 0 <= aScore - (2*rnds*h - h*h) <= 2*(rnds-h) + 1
    while (1)
    {
        //re-establish COMPARISON with new h values
        middleCompare = aScore - (2*rnds*h - h*h);
        rightCompare = 2*(rnds-h) + 1;
    
        //subtracted too much from aScore. h value too high and middleCompare is negative.
        if (middleCompare < leftCompare)
        {
            rightH = h - 1; //"h" already checked. Set bound to one below h.
            h = leftH + (rightH - leftH)/2;
        }
        //subtracted too little from aScore. h value too low.
        else if (middleCompare > rightCompare)
        {
            leftH = h + 1; //"h" already checked. Set bound to one above h.
            h = leftH + (rightH - leftH)/2;
        }

        else return h;
    }
}

long long findMinimumValue_recursion(long long aScore, long long rnds)
{
    //base cases
    if      (aScore == 2)                   return -1;
    else if (aScore == 0)                   return  0;
    else if (aScore <  2*rnds &&  aScore%2) return  1; //aScore is odd
    else if (aScore <= 2*rnds && !aScore%2) return  2; //aScore is even
    else if (aScore == 2*rnds+1)            return  3;

    //recursion
    return findMinimumValue_recursion(aScore - (2*rnds-1), rnds-1) + 1;
}

//CLASS OBJECTS/METHODS---------------------------------------------------------
class AliceGame
{
    public:
        long long findMinimumValue(long long aScore, long long hScore);
};


long long AliceGame::findMinimumValue(long long aScore, long long hScore)
{
    //NOTE: aScore, hScore, rnds, and h variables should all be the same type: long long.  
    long long rnds, h; 
    double sqrtChk;

    rnds = sqrtl( aScore + hScore );
    sqrtChk = sqrtl( aScore + hScore );
    
    //check if aScore or hScore is illegal value of 2 or if the sum between Alice and Harvey's score is not a perfect square
    if (aScore == 2 || hScore == 2) return -1;
    if (sqrtChk != floor(sqrtChk)) return -1;

    //optimize with "h" value here using binary search
    //the "h" value returned is the the LAST consecutive sequence of rounds that Alice could have won.
    //For instance, if we have 1 through r rounds, then we'd have 1....h......r where h - r rounds go to Alice.
    h = findMaxHVal(aScore, rnds);

    //Based on h, "2*rnds*h - h*h" points is deducted from "aScore" and "h" rounds are deducted from "rnds".
    return ( h + findMinimumValue_recursion(aScore - (2*rnds*h - h*h), rnds - h) );
}
