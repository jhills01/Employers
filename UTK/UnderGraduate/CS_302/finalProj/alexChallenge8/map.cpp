//Definitions for map libary

#include<vector>
#include<string>
#include<cstdlib>
#include "map.h"
using namespace std;


// Creates a game board that will have 15 rows and 15 columns. In the 
// future this size will be variable, but for this test keeping it consistent
// was simpler. Also sets up weather.
Map::Map(){
	r=15;
	c=15;
	w=rand()%3;
	grid.resize(15, vector<char>(15));
}


//Actually creates the map
void Map::generateMap(){
	int i,j;
	for(i=0; i< r; i++){
		for(j=0; j< c; j++){
			grid[i][j] = terrains[rand()%3];
		}
	}
}

void Map::changeWeather(){
	w=rand()%3;
}


// Returns the visibility of a tile. Not currently useful, but once attacking is added
// will be more important.
int Map::visibility(int row, int col) const{
	int vis = 3;
	switch(grid[row][col]){
		case 'F':
			vis-=1;
			break;
		case 'M':
			vis+=3;
			break;
	}
	switch(w){
		case 1:
			vis -=2;
			break;
		case 2:
			vis -=3;
			break;
	}
	return (vis <=0)?(1):(vis);
}
