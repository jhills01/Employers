//Header file for map library. Stores gameboard and relevant information.

#include <vector>
#include <string>
using namespace std;

class Map{
	
	int r, c, w;
	const string weather[3]= {"clear", "rain", "foggy"};
	const char terrains[3] = {'F', 'P', 'M'};
public:
	Map();
	vector<vector<char> > grid;
	void generateMap();
	void changeWeather();
	int rows() const {return r;}
	int cols() const {return c;}
	int size() const {return r*c;}

	string getWeather() const {return weather[w];}
	int visibility(int row, int col) const;
};
