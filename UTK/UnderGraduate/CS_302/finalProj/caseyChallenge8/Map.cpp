#include "Map.h"
#include <iostream>

using namespace std;
void Map::generateMap(PerlinNoise *pn){
	double value = 0.0;
	for(int i = 0; i < r; i++){
		for(int j = 0; j < c; j++){
			x = (double)j/((double)c) + offset;
			y = (double)i/((double)r) + offset;
			value = pn->noise(x*3.0, y*3.0, z);
			/*if (value < 0.3)
				map[i][j] = terrains[0];
			else if (value < 0.4)
				map[i][j] = terrains[1];
			else if (value < 0.5)
				map[i][j] = terrains[2];
			else if (value < 0.7)
				map[i][j] = terrains[3];
			else
				map[i][j] = terrains[4];
			*/
			if(value < 0.2)
				map[i][j] = terrains[4];
			else if(value < 0.3)
				map[i][j] = terrains[3];
			else if (value < 0.4)
				map[i][j] = terrains[2];
			else if (value < 0.45)
				map[i][j] = terrains[1];
			else if (value < 0.5)
				map[i][j] = terrains[0];
			else if (value < 0.55)
				map[i][j] = terrains[1];
			else if (value < 0.6)
				map[i][j] = terrains[2];
			else if (value < 0.7)
				map[i][j] = terrains[3];
			else
				map[i][j] = terrains[4];
		}
	}
}
int Map::visibility(int r, int c) const{
	int range = 5;
	float tMod = 0;
	float wMod = 0;
	switch(map[r][c]){
		case 'O': tMod = 0.5; break;
		case 'D': tMod = 0.5; break;
		case 'P': tMod = 0.75; break;
		case 'F': tMod = 0.25; break;
		case 'M': tMod = 1.5; break;
	}
	switch(w){
		case 0: wMod = 1.0; break;
		case 1: wMod = 0.9; break;
		case 2: wMod = 0.9; break;
		case 3: wMod = 0.7; break;
		case 4: wMod = 0.5; break;
		case 5: wMod = 0.4; break;
		case 6: wMod = 0.6; break;
	}
	range *= (tMod * wMod);
	if(range < 1) range = 1;
	return range;
}
Map::Map(){
	r = 0;
	c = 0;
	w = 0;
	t = 0;
	x = 0;
	y = 0;
	z = drand48();
	offset = drand48();
	pn = new PerlinNoise(rand());
}
Map::~Map(){
	delete pn;
}
void Map::resize(int r, int c){
	this->r = r;
	this->c = c;
	map.resize(r);
	for(int i = 0; i < r; i++){
		map[i].resize(c);
	}
	generateMap(pn);
}
int Map::setWeather(string weather){
	for(int i = 0; i < 7; i++){
		if(weathers[i] == weather){
			w = i;
			return 0;
		}
	}
	cout << "Not a possible weather to set" << endl;
	return -1;
}
void Map::changeWeather(){
	w = rand()%7;
}
int Map::setTime(string time){
	if(time == times[0]){
		t = 0;
		return 0;
	}
	else if(time == times[1]){
		t = 1;
		return 0;
	}
	cout << "Not a possible time to set" << endl;
	return -1;
}
void Map::changeTime(){
	t = !t;
}
void Map::print(vector< vector<char> > &units){
	cout << "Weather: " << weathers[w] << endl;
	cout << "Time: " << times[t] << endl;
	string str;
	for(int i = 0; i < r; i++){
		for(int j = 0; j < c; j++){
			switch(map[i][j]){
				case 'O': str = "\e[34m"; break;
				case 'D': str = "\e[93m"; break;
				case 'P': str = "\e[92m"; break;
				case 'F': str = "\e[32m"; break;
				case 'M': str = "\e[90m"; break;
			}
			cout << str + map[i][j] + units[i][j] << " ";
		}
		cout << endl;
	}
	cout << "\e[0" << endl;
}
