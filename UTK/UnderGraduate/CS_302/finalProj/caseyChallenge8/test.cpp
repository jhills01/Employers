#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include "Map.h"

using namespace std;

int main(){
	srand(time(NULL));
	Map m;
	vector< vector<char> > units;
	vector<char> row;
	for(int i = 0; i < 25; i++){
		for(int j = 0; j < 25; j++){
			row.push_back('_');
		}
		units.push_back(row);
		row.clear();
	}
	m.resize(25, 25);
	m.print(units);
	cout << m.visibility(4, 5) << endl;
	m.setWeather("foggy");
	cout << m.visibility(4, 5) << endl;
	//m.resize(15, 15);
	//m.print();
}
