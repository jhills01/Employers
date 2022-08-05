// John (Tom) Hills
// lab1b
// Bitwise operators

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

//stand-alone functions
string ToBinary(int value, int spacing)
{
    string res = "";
    int spaceIndex = spacing - 1;

    for (int i = 0; i < 32; i++)
    {
        //creates string containing the 32 bit binary representation of "value".
        res = (char) (((value >> i) & 1) + 48) + res;

        // adds ' ' to the appropriate indices of res as determined by "spacing".
        if (i == spaceIndex && i != 31)
        {
            res.insert(0," ");
            spaceIndex += spacing;
            //cout << "spaceIndex: " << spaceIndex << endl;                //test section
        }
    }
    return res;
}

class BITSET
{
private:
    vector<int> sets;

public:
    BITSET();
    bool Test(int location);
    void Set(int location);
    void Clear(int location);
    int GetNumSets();
    int returnSetsSizeMinusOne();
};

//BITSET default constructor
BITSET::BITSET()
{
    sets.resize(1,0);
}

//BITSET member functions
bool BITSET::Test(int location)
{
    int setNum = location/32;
    int bitNum = location%32;

    return (sets[setNum] >> bitNum) & 1;
}

void BITSET::Set(int location)
{
    int setNum = location/32;
    int bitNum = location%32;

    //Grows "sets" vector to appropriate number of sets/indices
    if (setNum > (int)(sets.size()-1))
    {
        sets.resize(setNum + 1, 0);
    }

    //Changes the requested bit location within appropriate set to 1
    sets[setNum] |= 1 << bitNum;

    return;
}

void BITSET::Clear(int location)
{
    int setNum = location/32;
    int bitNum = location%32;
    int setsSize = sets.size();

    if (setNum > (setsSize - 1))
    {
        printf("\nERROR: Requested bit location is in a set not yet created. Please use \"s\" command with same bit location to add the appropriate number of sets to vector and try again.\n");
        return;
    }

    //Sets the requested bit location within appropriate set to 0
    sets[setNum] &= ~(1 << bitNum);

    //Deletes vector elements of sets which == 0 starting with the highest index and stopping once an index returns a non-zero value.
    int counts = 0;
    for (int i = setsSize - 1; i >= 0; i--)
    {
        if (sets[i] == 0 && i != 0){counts++;}
        else {sets.resize(setsSize - counts); return;}
    }
}

int BITSET::GetNumSets()
{
    return (int)sets.size();
}

int main ()
{
    BITSET bs;
    int spacing;
    char op;
    int location;

    while (1)
    {
        printf("\nEnter command letter with number: ");
        scanf("%c", &op);
        if (op == 'n'||op == 'q')
        {
            goto switchLocation;
        }

        scanf("%d", &location);

        if (op == 'g')
        {
            printf("\nEnter a spacing value for the binary set: ");
            scanf("%d", &spacing);
        }

        switchLocation:

        switch (op)
        {
            case 't':
                    if ((location/32) > (bs.GetNumSets()-1))
                    {
                        printf("\nERROR: Requested bit location is in a set not yet created. Please use \"s\" command with same bit location to add the appropriate number of sets to vector and try again.\n");
                        break;
                    }
                    printf("The bit at location %d is \n%d\n",location,bs.Test(location));
                    break;
            case 's':
                    bs.Set(location);
                    break;
            case 'c':
                    bs.Clear(location);
                    break;
            case 'g':
                    printf("\nThe binary representation of integer %d is \n%s\n", location, ToBinary(location, spacing).c_str());
                    break;
            case 'n':
                    printf("\nThe number of sets in BITSET object \"bs\" is \n%d\n", bs.GetNumSets());
                    break;
            case 'q':
                    return 0;
            default:
                    printf("ERROR: The character entered is not recognized. Please input 't', 's', 'c', 'g', 'n', or 'q'.\n");
                    break;
        }

        //Clears scanf() buffer. Otherwise, scanf() doesn't read in subsequent char inputs (i.e., &op) correctly after 1st while loop of main().
        while ((getchar()) != '\n');
    }

    return 0;
}


