//Tom Hills (jhills)
//cs140: Lab 1 - moonglow

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	double multiple = 1.0, tempScore = 0.0, examScore = 0.0, questionScore = 0.0, sum = 0.0, average = 0.0, averageSum = 0.0, count = 0.0;
	string input, name;

	//priming read from standard input as the while condition check (!cin.eof()) is reactive, not proactive.
	cin >> input;
	
	//loop to check text file inputs as either numbers or having "NAME" and "AVERAGE" keywords and performs appropriate calculations
	while (!cin.eof())
	{
		//checks if input is a number not immediately following the "AVERAGE" keyword and adds to examScore if true.
		if (input[0] >= '0' && input[0] <= '9') 
		{
			//PROBLEMS HERE: My examScore output is NOT rounding to match grading script.
			examScore += strtod(input.c_str(), NULL);
			cin >> input;
		}

		//checks if input is "NAME" and stores it in "name" variable if true
		else if (input == "NAME") 
		{
			cin >> name;
			cin >> input;
		}
	
		//checks if input is "AVERAGE" and, if true, performs appropriate caculations depending on subsequent reads into input
		else if (input == "AVERAGE")
		{
			cin >> input;
			
			//Code to execute if "input" is a number
			while (input[0] >= '0' && input[0] <= '9' && !cin.eof())
			{
				questionScore = strtod(input.c_str(), NULL);
				sum += questionScore;
				count++;
				cin >> input;
			}
			
			if (count > 0.0)
			{
				average = sum/count;
				averageSum += average;
				
				count = 0.0;
				sum = 0.0;
			}
		}

		else {cin >> input;}
	}

	//calculation for final exam score
	examScore += averageSum;

	//final printout of student's name and final exam score
	cout << name << " " << examScore << '\n';
	
	return 0;
}

