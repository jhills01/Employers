#include "Units.h"
#include <iostream>
using namespace std;


int main(){

	Sniper i(5,5);
	Unit *unit1 = &i;

	pair<int,int> des;
	des = make_pair(4,5);
	int row =20;
	int col =20;
	int visibility = 1;
	bool move;
	vector<vector<char> >grid;
	grid.resize(row);
	for(int i=0; i<grid.size(); i++){
		grid[i].resize(col,'_');
	}
	grid[5][5] = 'A';
	move=unit1->move(grid, des, row, col);
	unit1->print();
	return 0;
}
