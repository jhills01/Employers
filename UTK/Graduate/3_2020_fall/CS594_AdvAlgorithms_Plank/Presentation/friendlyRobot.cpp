#include <cstdlib>
#include <iostream>
#include <sstream> 
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>

using namespace std;

//Debug print of single string
void printStr(string s)
{
    printf("%s\n", s.c_str());
}

//CLASS DEF & METHODS---------------------------------------------------------------------------------------------
class FriendlyRobot
{
    public:
        //variables
        int maxVal;
        int debug;
        int tab;

        vector<int> cntResults;

        //Methods
        int findMaximumReturns(string instructions, int changesAllowed);
        int MaxReturnsToOrigin_recursive(int i, string instructions, int changesAllowed);
};

int FriendlyRobot::MaxReturnsToOrigin_recursive(int i, string instructions, int changesAllowed)
{
    int U, D, L, R, changeCnt;
    int x,y, originCnt;
    int ret;

    //debug variables
    ++tab;
    int tabLocal = tab;

    //BASE CASES-------------------------------------------------------------------------------------------------------------------
    //if (changesAllowed == 0) 
    if (changesAllowed == 0 || i > instructions.length()) 
    {
        //This is where we simply run through the remaining string to see how many origin returns occur.
        //we then return that number. 
        //x = y = 0 initializaion assumes we are starting from origin.
        x = 0; y = 0; originCnt = 0;

        for (int z = 0; z < instructions.length(); ++z)
        {
            switch (tolower(instructions[z]))
            {
                case 'u': y += 1; break;
                case 'd': y -= 1; break;
                case 'l': x -= 1; break;
                case 'r': x += 1; break;
            }

            if (x == 0 && y == 0) 
            {
                originCnt++; //we returned to origin past the starting point
            }    
        }

        if (debug) 
        {
            for (int k = 0; k < tabLocal; ++k) printf(" ");   
            printf("%d:BASE CASE (changesAllowed == 0 || i > instructions.length()) ----> instructions: %-20s originCnt: %-10d\n", tabLocal, instructions.c_str(), originCnt);
        }

        return originCnt;
    }
    else if (instructions.length() == 0) 
    {
        if (debug) 
        {
            for (int k = 0; k < tabLocal; ++k) printf(" ");   
            printf("%d:BASE CASE (instructions.length() == 0) ---> instructions(should be blank): %-20s\n", tabLocal, instructions.c_str());
        }

        return 0; 
    } 
    
    //ALGORITHM: uses U/D/R/L counts to determine how many changes are needed to RETURN BACK to origin (0,0) ------------------------------------------
    //if U == D && L == R for a string segment, then that segment returns to the origin AT LEAST once.
    //This assumes that "j = 0" to "i = #" characters of instruction string START at origin intially 
    //NOTE: j = 0 for initialization is correct b/c we want to start at the beginning of the string and work up until i-1 element
    changeCnt = 0; U = 0; D = 0; L = 0; R = 0;
    for (int j = 0; j < i; ++j) //j < 2, 4, 6, 8, ....
    {
        //count individual commands for the first "i" instructions
        switch (tolower(instructions[j]))
        {
            case 'u': U += 1; break;
            case 'd': D += 1; break;
            case 'l': L += 1; break;
            case 'r': R += 1; break;
        }
    }

    //find out how many changes are needed to make U == D && L == R
    if (U != D && L == R) //Only need to swap between U's/D's. Leave R's/L's alone.
    {
        if (U > D) { while (U != D) {U--; D++; changeCnt++;} }
        else { while(U != D) {U++; D--; changeCnt++;} }
    }
    else if (U == D && L != R) //Only need to swap between R's/L's. Leave U's/D's alone.
    {
        if (L > R) { while (L != R) {L--; R++; changeCnt++;} }
        else { while(L != R) {L++; R--; changeCnt++;} }
    }
    else //U != D && L != R --- most difficult case to handle. Need to swap between U/D and L/R pairs.
    {
        if      (U > D && L > R) { while (U != D && L != R) {U--; R++; changeCnt++;} }
        else if (U > D && R > L) { while (U != D && L != R) {U--; L++; changeCnt++;} }
        else if (D > U && L > R) { while (U != D && L != R) {D--; R++; changeCnt++;} }
        else if (D > U && R > L) { while (U != D && L != R) {D--; L++; changeCnt++;} }
    }

    if (debug) 
    {
        for (int k = 0; k < tabLocal; ++k) printf(" ");
        printf("%d:BEFORE RECURSION CALL ---> instructions: %-20s changesAllowed: %-5d subStr: %-10s changeCnt: %-5d\n", tabLocal, instructions.c_str(), changesAllowed, instructions.substr(0, i).c_str(), changeCnt);
    }

    if (changeCnt <= changesAllowed)
    {
        ret = 1 + MaxReturnsToOrigin_recursive(i, instructions.substr(i), changesAllowed - changeCnt);

        if (debug) 
        {
            for (int k = 0; k < tabLocal; ++k) printf(" ");   
            printf("%d:AFTER RECURSIVE CALL ----> ret: %-10d\n", tabLocal, ret);
        }

        return ret;
    }
    else 
    {
        /*There's no answer if changeCnt > changesAllowed. Move on with OUTTER LOOP*/ 
        cntResults[i/2 - 1] = -1; 

        if (debug) 
        {
            for (int k = 0; k < tabLocal; ++k) printf(" ");   
            printf("%d:NO RECURSIVE CALL MADE!!!!! OUTTER LOOP MOVES ON. cntResults[%-3d]: %-10d\n", tabLocal, i/2-1, cntResults[i/2 - 1]);
        }
    }
}

int FriendlyRobot::findMaximumReturns(string instructions, int changesAllowed)
{
    //OUTTER LOOP to control i + 2, i + 4, i + 6, etc progresssion
    //NOTE: I think I realized something: 
    debug = 1; //1 == print debug info. 0 == DON'T print debug info.
    for (int i = 2; i <= instructions.length(); i+=2) //i == 2, 4, 6, 8, ....
    {
        tab = -1;

        if (debug) printf("i == %d////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n", i);
        cntResults[i/2 - 1] = MaxReturnsToOrigin_recursive(i, instructions, changesAllowed);
    }

    if (debug) printf("OUT OF RECURSIVE LOOP////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");

    //find maximum value in cntResults
    maxVal = -1;
    for (int i = 0; i < cntResults.size(); ++i)
    {
        if (cntResults[i] > maxVal) maxVal = cntResults[i];
    }

    return maxVal;
}

//-------------------------------------------------------------------------------------------------
int main()
{
    string line, instructions;
    int colonPos, changesAllowed;
    FriendlyRobot *fr = new FriendlyRobot();

    while (getline(cin, line))
    {
        colonPos = line.find(":"); 
        instructions =  line.substr(0, colonPos);
        changesAllowed = atoi(  line.substr(colonPos+1).c_str() );


        fr->cntResults.resize(0);
        fr->cntResults.resize(instructions.length() + 2, 0);

        printf("----------------------------------------------------------------------------------------------------------------------\n");
        printStr(instructions);
        printf("changesAllow: %d\n", changesAllowed);
        printf("\n");

        printf("\n%d\n\n", fr->findMaximumReturns(instructions, changesAllowed));

        printf("----------------------------------------------------------------------------------------------------------------------\n\n");
    }

    delete fr;
    return 0;
}