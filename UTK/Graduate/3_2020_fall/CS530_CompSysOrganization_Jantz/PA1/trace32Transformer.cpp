#include <cstdlib>
#include <iostream>
#include <sstream> 
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <map>
#include "pa1.h"

using namespace std;

int main(int argc, char**argv)
{
    char rw;
    string line;

    if (argc != 2) {printf("forgot argument dipshit\n"); exit(1);}

    while (cin >> line)
    {
        rw = line[0];
        line = line.substr(atoi(argv[1]));
        cout << rw << ":" << line << endl;
    }
    
    return 0;
}