// Challenge 8: Preparing for Final Project
//
// Author: Alexander Lambert
//
// Description: My part of the project is tying the project together into
// one cohesive program. To get practice and get started on this, I made a 
// mini version of our project and tied it all together. This file contains the
// actual "game" and is what the user interacts with.
#include "map.h"
#include "Unit.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unordered_map>

using namespace std;

//Helper functions to make cleaner code
bool startGame(Map& gameboard, unordered_map<int, Unit*>& p1Units, unordered_map<int, Unit*>& p2Units);
void printMap(vector<vector<char> >& unitGrid, Map& gameboard);
void moveUnit(unordered_map<int, Unit*>& units, vector<vector<char> >& unitGrid);

int main(int argc, char* argv[]){
	int choice, idP1=1, idP2=1;
	unsigned int turnCounter=0, row, col;
	bool curTurn = false;
	bool unitBought=false;
	//bool unitsmoved =false;
	string theGrid;
	
	// Start menu
	do{
		cout << "BOARD OF STRATEGY AND DOOOM AND WIT AND MUCH MORE!!\n  1. Play\n  2. Exit\n";
		cin >> choice;
	}while(choice >2 && choice <1);
	if(choice == 2) return 0;
	
	// All of thse are declared after it is decided the game will be played
	// since they take more memory than the others.
	Map gameboard;
	//Units are stored in a map so they can be accessed by ID in close to constant time.
	unordered_map<int,Unit*> p1Units;
	unordered_map<int,Unit*> p2Units;

	//This unit grid allows for quick printing of the units on the map
	//alongside the terrain tiles.
	vector<vector<char> > unitGrid(15, vector<char>(15, '_'));
	
	//There is only one map size for testing. Here the bases are added to the map
	unitGrid[7][0] = 'b';
	unitGrid[7][14] ='b';

	curTurn = startGame(gameboard, p1Units, p2Units);
	printMap(unitGrid, gameboard);

	//This is the main game loop. As long as the player does not
	//choose "6" the game will keep running. Eventually this will be
	//tied to victory conditions as well
	while(true){
		if(curTurn){
			cout << "Player 1\'s turn\n";
		}else{
			cout << "Player 2\'s turn\n";
		}
		cout << "Options: 1. Move Units\n 2. Buy unit\n 3. Check tile\n 4. End turn\n 5. Reprint Map\n 6. Quit\n Choice: ";
		cin >> choice;

		//Menu
		switch(choice){
			//Attempts to move unit, this is commented out because it does not work
			//right now
		/*	case 1:
				if(!unitsMoved){
					if(curTurn){
						moveUnit(p2Units, unitGrid);
					}else{
						moveUnit(p1Units, unitGrid);
					}
					printMap(unitGrid, gameboard);
					unitsMoved=true;
				}else{
					cout << "All units already moved this turn\n";
				}
				break;*/
			// Buys a new unit. Currently player can only
			// do this once as they cannot move the new unit away
			// from the spawn point
			case 2:
				if(!unitBought){
					//Figure out which base and unit map to add to
					if(curTurn){
						if(unitGrid[7][13] == '_'){
							p2Units.emplace(idP2,new Infantry(7,13));
							unitGrid[7][13] = 'i';
							idP2++;
						}else{
							cout << "Space in front of base already occupied. Please move unit first\n";
						}
					}else{
						if(unitGrid[7][1] == '_'){
							p1Units.emplace(idP1, new Infantry(7,13));
							unitGrid[7][1]= 'i';
							idP1++;
						}else{
							cout << "Space in front of base already occupied. Please move unit first\n";
						}
					}
					unitBought = true;
				}else{
					cout << "Unit already bought this turn.\n";
				}
				break;
			//Returns information about selected tile, like visibility
			//and presence of unit
			case 3:
				cout << "Which tile (enter rows and columns): ";
				cin >> row >> col;
				if(row >15 || col>15){
					cout << "\nInvalid move, out of bounds\n";
				}else{
					cout << "\n Visibility: "<< gameboard.visibility(row, col) << endl;
					if(unitGrid[row][col] == 'i'){
						cout << "Infantry present on this tile\n";
					}else if(unitGrid[row][col] == '_'){
						cout << "Base present on this tile\n";
					}
				}
				break;
			//Switches turns, and resets relevant information.
			case 4:
	//			unitsMoved=false;
				unitBought=false;
				turnCounter++;
				if(turnCounter%4 ==0){
					gameboard.changeWeather();
				}
				printMap(unitGrid, gameboard);
				curTurn = !curTurn;
				break;
			//Reprints map
			case 5:
				printMap(unitGrid, gameboard);
				break;
			//Exits game
			case 6:
				return 0;
		}
		
	}
}

//Initializes board and puts bases into unit maps. Currently only makes a board
//of 15x15, but in the future it will handle varying sizes.
bool startGame(Map& gameboard, unordered_map<int, Unit*>& p1Units, unordered_map<int,Unit*>& p2Units){
	unsigned int choice;
	string size;

	cout << "Input a seed (put 0 for random)?: ";
	cin >> choice;
	
	if(choice ==0){
		srand(time(NULL));
	}else{
		srand(choice);
	}
	gameboard.generateMap();
	p1Units.emplace(0, new Base(7, 0));
	p2Units.emplace(0, new Base(7, 14));
	return rand()%2;
}

// Runs through the map and prints it
void printMap(vector<vector<char> >& unitGrid, Map& gameboard){
	int i, j;
	cout << gameboard.getWeather() << endl;
	for(i=0;i< gameboard.rows(); i++){
		for(j=0; j<gameboard.cols(); j++){
			cout << unitGrid[i][j] << gameboard.grid[i][j] << " ";
		}
		cout << endl;
	}
}

// Basic attempt at moving units. Will need much more work in order to actually
// function properly with correct error checking.

/*void moveUnit(unordered_map<int, Unit*>& units, vector<vector<char> >& unitGrid){
	unsigned int id, row, col;
	cout << "Current units:\n";
	unordered_map<int, Unit*>::iterator it;

	for(it = units.begin(); it != units.end(); it++){
		cout << it->first << '.' << (it->second)->name << endl;
	}
	cin >> id;
	if(id > units.size()){
		cout<<"Invalid id\n";
		return;
	}
	cout<< "Enter new row and col: ";
	cin >> row >> col;

	if(units[id]->move(row, col)){
		
	}

}*/

