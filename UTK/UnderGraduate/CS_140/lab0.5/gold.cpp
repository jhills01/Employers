#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

using namespace std;

int main()
{
	string str;
	int totalOunces = 0;

	getline(cin, str, '\0');

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '.' || str[i] == '-'|| str[i] == '\n') {}
		else {totalOunces += (int)str[i] - 64;}
	}

	cout << totalOunces << endl;

	return 0;
}

