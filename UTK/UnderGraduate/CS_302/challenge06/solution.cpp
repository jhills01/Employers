// Challenge 06: Palindrome Permutation

//Tom Hills (jhills)

//Brief Description:
//This program reads in phrases from std input one per line until eof.
//After each phrase, the code checks if it is a palindrome permutation or not ignoring spaces and punctuation.

#include <unordered_set>//note: inserts, erases, etc are done in O(1) time b/c they use hashes instead of trees
#include <string>
#include <iostream>
#include <cctype>

using namespace std;

//returns true if string s is a palindrome permutation, or false if not. 
bool is_palindrome(string &s)
{
	unordered_set<char> phraseSet;
	unordered_set<char>::iterator usit;
	pair<unordered_set<char>::iterator, bool> pusit;
	
	//read and count string chars into unordered map
	for (unsigned int i = 0; i < s.size(); ++i)
	{
		//checks alphabetic characters from string s ignoring spaces and punctuation
		if ( (s[i] >= 65 && s[i] < 91) || (s[i] >= 97 && s[i] < 123) )
		{
			pusit = phraseSet.insert(tolower(s[i]));//makes all alphabetic characters lower case
			
			if (pusit.second){/*Do nothing since new character was inserted*/}
			else {phraseSet.erase(pusit.first);}
		}
	}

	//check if resulting set indicates a palindrome permutation
	if (phraseSet.size() == 1 || phraseSet.size() == 0) {return true;}
	else {return false;}
}

int main(int argc, char *argv[]) 
{
	string phraseStr;

	//check if string from std in is a palindrome permutation 
	while (getline(cin, phraseStr))
	{
		if(is_palindrome(phraseStr))
		{printf("\"%s\" is a palindrome permutation\n", phraseStr.c_str());}
		else {printf("\"%s\" is not a palindrome permutation\n", phraseStr.c_str());}
	}

	return (0);
}
