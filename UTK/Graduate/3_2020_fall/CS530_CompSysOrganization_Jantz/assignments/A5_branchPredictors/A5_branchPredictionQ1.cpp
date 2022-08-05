#include <cstdlib>
#include <iostream>
#include <sstream> 
#include <string>
#include <cstring>
#include <vector>
#include <cstdio>

using namespace std;

int main()
{
    int sum = 0;
    int i, j;
    int B1hits = 0, B1misses = 0;
    int B2hits = 0, B2misses = 0;
    int B3hits = 0, B3misses = 0;
    int B1 = 1, B2 = 0, B3 = 0;
    char B1taken = 'n', B2taken = 'n', B3taken = 'n';
    int B2loopCnt = 0;

    B3taken = 'y';

    for (i = 1; i < 101; i++)
    {
        //----------------------------------------------------------------------------------------------           
        if (B3taken == 'y' && B3 == 0) { B3misses++; B3 = 1; } //misprediction 
        else if (B3taken == 'y' && B3 == 1) { B3hits++; } //predicted correctly

        if ( (i%4) == 0) B1taken = 'y';
        else B1taken = 'n';
        //----------------------------------------------------------------------------------------------           

        if ( (i%4) == 0 ) 
        {
            //----------------------------------------------------------------------------------------------           
            if (B1taken == 'y' && B1 == 0) { B1misses++; B1 = 1; } //misprediction 
            else if (B1taken == 'y' && B1 == 1) { B1hits++; } //predicted correctly

            B2taken = 'y';
            //----------------------------------------------------------------------------------------------           
            for (j = 1; j < 11; j++) 
            { 
                //----------------------------------------------------------------------------------------------           
                if (B2taken == 'y' && B2 == 0) { B2misses++; B2 = 1; } //misprediction 
                else if (B2taken == 'y' && B2 == 1) { B2hits++; } //predicted correctly
                //----------------------------------------------------------------------------------------------           

                sum += i*j;
            }
            //----------------------------------------------------------------------------------------------           
            B2taken = 'n';
            if (B2taken == 'n' && B2 == 0) { B2hits++; } //predicted correctly
            else if (B2taken == 'n' && B2 == 1) { B2misses++; B2 = 0; } //misprediction 
            //----------------------------------------------------------------------------------------------           
        }
        //----------------------------------------------------------------------------------------------                   
        if      (B1taken == 'n' && B1 == 0) { B1hits++;  }  //predicted correctly
        else if (B1taken == 'n' && B1 == 1) { B1misses++; B1 = 0;}//misprediction
        //----------------------------------------------------------------------------------------------           

        sum += i;
    }

    B3taken = 'n';
    if (B3taken == 'n' && B3 == 0) { B3hits++;  } //predicted correctly
    else if (B3taken == 'n' && B3 == 1) { B3misses++; B3 = 0;} //misprediction 

    printf("\nB1hits: %d, B1misses: %d\n", B1hits, B1misses);
    printf("B2hits: %d, B2misses: %d\n", B2hits, B2misses);
    printf("B3hits: %d, B3misses: %d\n\n", B3hits, B3misses);
    printf("TOTAL HITS: %d, TOTAL MISSES: %d\n", B1hits + B2hits + B3hits, B1misses + B2misses + B3misses);

    return 0;

}