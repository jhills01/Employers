// Challenge 8: Preparing for Final Project
//
// Author: Alexander Lambert
//
// Description: My part of the project is tying the project together into
// one cohesive program. To get practice and get started on this, I made a 
// mini version of our project and tied it all together. This file contains the
// actual "game" and is what the user interacts with.
//#include "caseyChallenge8/Map.h"
//#include "Units/Units.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
#include <set>
#include <SDL2/SDL.h>
#include <cstdio>
#include "Tom/SDL_cbc/cbc_library.h"

using namespace std;

const int aSCREEN_WIDTH = 800;
const int aSCREEN_HEIGHT = 600;

string apath;

SDL_Window* agWindow=NULL;

SDL_Renderer* agRenderer = NULL;

SDL_Texture* agTexture = NULL;

SDL_Rect aviewport;

class Player{
	public:
		Player();
		Player(int, int, vector<vector<char> >&);
		string name;
		vector<Unit*> units;
		set<int> deadUnits;
};

Player::Player(int row, int col, vector<vector<char> >& unitGrid){
	units.emplace_back(new Sniper(1,col));
	unitGrid[1][col] = 'S';
	units.emplace_back(new Sniper(13, col));
	unitGrid[13][col] = 'S';
	units.emplace_back(new Artillery(2,col));
	unitGrid[2][col] = 'A';
	units.emplace_back(new Artillery(12,col));
	unitGrid[12][col] = 'A';
	units.emplace_back(new Infantry(3, col));
	unitGrid[3][col] = 'I';
	units.emplace_back(new Infantry(11, col));
	unitGrid[11][col] = 'I';
	units.emplace_back(new Cavalry(4,col,15));
	unitGrid[4][col] = 'C';
	units.emplace_back(new Cavalry(10, col, 15));
	unitGrid[10][col] = 'C';
	units.emplace_back(new Biker(5, col));
	unitGrid[5][col] = 'B';
	units.emplace_back(new Biker(9, col));
	unitGrid[9][col] = 'B';
	unitGrid[row][col] = 'b';
	units.emplace_back(new Base(row,col));
}

//Helper functions to make cleaner code
int startGame(Map& gameboard, Player* players);

int main(int argc, char* argv[]){
	int choice, curPlayer, unit, unit2;
	unsigned int turnCounter=0, i;
	pair<int, int> des, src;
	vector<vector<char> > unitGrid(15, vector<char>(15, '_'));
	char charChoice;
	const char unitSymbols[6] = {'S', 'A', 'I', 'C', 'B', 'b'};
	const string unitNames[6] = {"Sniper", "Artillery", "Infantry", "Cavalry", "Biker", "Base"};
	set<int>::iterator it;

	if( !init()){
		printf("Failed to initialize!\n");
	}else{

		// Start menu
		/*do{
			cout << "Chess, But Cooler\n  1. Play\n  2. Exit\n";
			cin >> choice;
		}while(choice >2 && choice <1);
		if(choice == 2) return 0;*/

		bool quit = false;

		SDL_Event event;

		Map gameboard;

		Player players[2] = {Player(7,0, unitGrid), Player(7, 14, unitGrid)};

		curPlayer = startGame(gameboard, players);

		//gameboard.print(unitGrid);
		cout << "\e[0m";
		//This is the main game loop. As long as the player does not
		//choose "6" the game will keep running. Eventually this will be
		//tied to victory conditions as well
		while(!quit){
			while(SDL_PollEvent(&event) !=0){
				if(event.type == SDL_QUIT)	{
					quit = true;
				}
			}

//			display(gameboard);
			display(gameboard, unitGrid); //TOM: added this

			cout << '\n' << players[curPlayer].name << "\'s turn\n";
			
			if(players[curPlayer].deadUnits.size() > 0){
				for(it = players[curPlayer].deadUnits.begin(); it != players[curPlayer].deadUnits.end(); it++){
					if(players[curPlayer].units[*it]->deadCount ==5){
						while(true){
							cout << "A new " << unitNames[*it/3] << " is ready to be placed. Enter the cordinates where it should go. The column must be one of the first 3 on your side.\n";
							cin >> des.first >> des.second;

							if(unitGrid[des.first][des.second] != '_')
								cout << "Unit already there.\n";
							else if(((des.second >4|| des.second<0)&&curPlayer==0)||((des.second>14 || des.second<11)&& curPlayer==1))
								cout << "Too far from base.\n";
							else{
								delete players[curPlayer].units[*it];
								switch(*it){
									case 0:
									case 1:
										players[curPlayer].units[*it] = new Sniper(des.first, des.second);
										break;
									case 2:
									case 3:
										players[curPlayer].units[*it] = new Artillery(des.first, des.second);
										break;
									case 4:
									case 5:
										players[curPlayer].units[*it] = new Infantry(des.first, des.second);
										break;
									case 6:
									case 7:
										players[curPlayer].units[*it] = new Cavalry(des.first, des.second, 15);
										break;
									case 8:
									case 9:
										players[curPlayer].units[*it] = new Biker(des.first, des.second);
								}
								unitGrid[des.first][des.second] = unitSymbols[*it/2];
								it = players[curPlayer].deadUnits.erase(it);
								break;
							}
						}
					}else{
						players[curPlayer].units[*it]->deadCount++;
					}
				}
			}
			choice = -1;
			cout << "Options:\n 1. Move Units\n 2. Check tile\n 3. Check unit\n 4. Attack\n 5. End turn\n 6. Reprint Map\n 7. Quit\n Choice: ";
			cin >> choice;

			//Menu
			switch(choice){
				// Move units
				case 1:
					for(i = 0; i < players[curPlayer].units.size()-1; i++){
						if(players[curPlayer].units[i]->getMove()){
							cout << "Unit: "<< i << " has already been moved this turn.\n";
							continue;
						}
						if(players[curPlayer].units[i]->isDead){
							continue;
						}
						cout << "Move unit: " << i << "? (y/n)";
						cin >> charChoice;
						while(charChoice == 'y'){
							cout << "Enter unit " << i << "\'s destination (row, col), or -1 -1 to cancel: ";
							cin >> des.first >> des.second;
							src = players[curPlayer].units[i]->getPosition();
							if(des.first == -1 && des.second == -1){
								break;
							}else if(players[curPlayer].units[i]->move(unitGrid, des, 15, 15)){
								unitGrid[src.first][src.second] = '_';
								unitGrid[des.first][des.second] = unitSymbols[i/2];
								break;
							}
						}
					}
					//gameboard.print(unitGrid);
		//			display(gameboard);
					display(gameboard, unitGrid); //TOM: added this
					break;
				case 2:
					cout << "Which tile (enter rows and columns): ";
					cin >> des.first >> des.second;
					if(des.first >15 || des.second>15){
						cout << "\nInvalid tile, out of bounds\n";
					}else{
						cout << "\n Visibility: "<< gameboard.visibility(des.first, des.first) << endl;
						switch(unitGrid[des.first][des.second]){
							case '_':
								cout << "No unit";
								break;
							case 'I':
								cout << "Infantry";
								break;
							case 'A':
								cout << "Artillery";
								break;
							case 'S':
								cout << "Sniper";
								break;
							case 'C':
								cout << "Cavalry";
								break;
							case 'B':
								cout << "Biker";
								break;
							case 'b':
								cout << "Base";
						}
						cout << " present on this tile\n";
					}
					break;
					//Switches turns, and resets relevant information.
				case 3:
					// Check unit
					cout << "Which unit to check (0-10), or enter 11 for all units";
					cin >> unit;
					if(unit < 11){
						if(players[curPlayer].units[unit]->isDead){
							cout << "This " << unitNames[unit/2] << " is dead, and a new recruit will arrive in " << 5 - players[curPlayer].units[unit]->deadCount << " turns\n";
						}
						cout << "This " << unitNames[unit/2] << " has the following stats:";
						cout << "\nHealth: " << players[curPlayer].units[unit]->getHealth();
						cout << "\nRange: " << players[curPlayer].units[unit]->getRange();
						cout << "\nDamage: " << players[curPlayer].units[unit]->getDamage();
						cout << "\nMoved this turn: " << (players[curPlayer].units[unit]->getMove())?("True\n"):("False\n");
					}else{
						for(i=0; i< 11; i++){
							cout << i << ". ";
							if(players[curPlayer].units[i]->isDead){
								cout << "This " << unitNames[i/2] << " is dead, and a new recruity will in " << 5- players[curPlayer].units[i]->deadCount << " turns\n";
							}else{
								cout << "This " << unitNames[i/2] << " has the following stats:";
								cout << "\nHealth: " << players[curPlayer].units[i]->getHealth();
								cout << "\nRange: " << players[curPlayer].units[i]->getRange();
								cout << "\nDamage: " << players[curPlayer].units[i]->getDamage();
								cout << "\nMoved this turn: " << (players[curPlayer].units[i]->getMove())?("True\n"):("False\n");
							}
						}
					}
					break;
				case 4:
					//Attack
					cout << "Which unit do you want to attack with? (0-9)";
					cin >> unit;
					src = players[curPlayer].units[unit]->getPosition();
					cout << "Which enemy unit do you want to attack? (0-9, 10 for base)";
					cin >> unit2;
					if(players[curPlayer].units[unit]->attack(gameboard.visibility(src.first, src.second),players[curPlayer+1%2].units[unit2])){
						if(players[curPlayer+1%2].units[unit2]->getHealth() <=0){
							if(unit2 == 10){
								cout << players[curPlayer+1%2].name << "'s base has been overrun. Structures burn and no prisoners are taken. " << players[curPlayer].name << " has won the game!\n";
								return 0;
							}
							players[curPlayer+1%2].units[unit2]->isDead = true;
							players[curPlayer+1%2].deadUnits.insert(unit2);
						}
					}
				case 5:
					turnCounter++;
					if(turnCounter%4 ==0){
						gameboard.changeWeather();
					}
					if(turnCounter%5 ==0){
						gameboard.changeTime();
					}
					//gameboard.print(unitGrid);
		//			display(gameboard);
					display(gameboard, unitGrid); //TOM: added this
					for(i=0; i<10; i++){
						//players[curPlayer].units[i]->
					}
					curPlayer = (curPlayer==1)?(0):(1);
					break;
					//Reprints map
				case 6:
					//gameboard.print(unitGrid);
		//			display(gameboard);
					display(gameboard, unitGrid); //TOM: added this
					break;
					//Exits game
				case 7:
					quit = true;
			}
		}
	}
	close();
	return 0;
}

//Initializes board and puts bases into unit maps. Currently only makes a board
//of 15x15, but in the future it will handle varying sizes.
int startGame(Map& gameboard, Player* players){
	unsigned int choice;

	cout << "Input a seed (put 0 for random)?: ";
	cin >> choice;

	if(choice ==0){
		srand(time(NULL));
	}else{
		srand(choice);
	}
	gameboard.resize(15, 15);

	cout << " Input Player 1's Name: ";
	cin >> players[0].name;

	cout << " Input Player 2's Name: ";
	cin >> players[1].name;

	return rand()%2;
}

