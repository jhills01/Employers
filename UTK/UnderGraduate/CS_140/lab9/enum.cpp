//Tom Hills (jhills)
//Lab 9 - Part 1: Enumerating Strings

#include <cstdlib>
#include <cstdio>
#include <string>

using namespace std;

class enumString
{
	public:
		enumString(int stringLength): stringLength(stringLength), eString(stringLength, '0') {}  
		void do_enumeration(int stringIndex, int numOfOnes);

	private:
		int stringLength;
		string eString;
};

void enumString::do_enumeration(int strIndex, int numOfOnes)
{
	//base case
	if (strIndex == this->eString.size() && numOfOnes == 0) {printf("%s\n", this->eString.c_str()); return;}
	
	//calculate whether the character at the given index CAN BE a '0'. 
	if ( (this->stringLength - strIndex) > numOfOnes )	{this->eString[strIndex] = '0'; do_enumeration(strIndex + 1, numOfOnes);}
	
	//first recursive call returns, determine if the character at the given index CAN BE a '1'. 
	if (numOfOnes != 0)	{this->eString[strIndex] = '1'; do_enumeration(strIndex + 1, numOfOnes - 1);}
}

int main(int argc, char *argv[])
{
	//standard argument check --- not really needed since input from files has been confirmed to be in correct format
	if (argc != 3) {printf("usage: enum length ones\n"); return 1;}

	//no error check for data type needed
	enumString s(atoi(argv[1]));
	s.do_enumeration(0, atoi(argv[2]));
	
	return 0;
}
