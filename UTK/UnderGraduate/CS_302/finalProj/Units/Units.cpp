// Name: Shivam Patel
// Description: Defined the getter functions and print  

using namespace std;
#include "Units.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


//****UNIT******

int Unit::getHealth(){return health;}

void Unit::setHealth(int newHealth){health = newHealth;}

int Unit::getRange(){return range;}

int Unit::getDamage(){return damage;}

pair<int,int> Unit::getPosition(){return pos;}

bool Unit:: getMove(){return moveCheck;}

void Unit:: setMove(bool set){moveCheck = set;}


void Unit::print(){

	printf("Unit Status: \nHealth: %d\nDamage: %d\nRange: %d\nCurrent Position: %d,%d\nMove: %d\nisDead: %d\nDead Count: %d\n", health, damage, range,pos.first,pos.second, moveCheck, isDead, deadCount);
}
//*****UNIT -----> BASE*******

Base::Base(int row, int col){
	this->health = 20000;
	this->range = 0;
	this->damage = 0;
	this->pos = make_pair(row,col);
	this->moveCheck = false;
	this->deadCount = 0;
	this->isDead=false;
}

bool Base::move(vector<vector<char> >grid,pair<int,int> des, int row, int col){
	printf("The base does not move at any point in the game\n");
	return false;
}

bool Base::attack(int visibility, Unit*){
	printf("The base does not attack at any point in the game\n");
	return false;
}

//*****UNIT -----> SNIPER*******
Sniper::Sniper(int row, int col){
	this->health = 2500;
	this->damage = 550;
	this->range = 7;
	this->pos = make_pair(row,col);
	this->moveCheck = false;
	this->deadCount =0;
	this->isDead=false;
}

bool Sniper::move(vector<vector<char> >grid,pair<int,int>des, int row, int col){
	bool valid = false;
	if(grid[des.first][des.second] != '_'){
		printf("Cannot move! A unit is already there\n");
		return false;
	}
	if(des.first <0 || des.first >=row || des.second<0 || des.second >=col){
		printf("Invalid Move! You went out of bounds!\n");
		return false;
	}

	int rowdiff = abs(des.first - pos.first);
	int coldiff = abs(des.second - pos.second);

	//valid moves
	if(rowdiff ==1 && coldiff ==1) valid = true;
	if(rowdiff ==1 && coldiff ==0) valid =true;
	if(rowdiff ==0 && coldiff ==1) valid =true;

	if(valid == false){
		printf("Sniper cannot move to that destination. Sniper moves 1 tile in any direction\n");
		return false;
	}
	
	//update the unit's current position to the destination postion
	else{
		pos.first = des.first;
		pos.second = des.second;
	}
	moveCheck = true;
	return true;
}

bool Sniper::attack(int visibility, Unit* unit2){
	bool valid = false;
	int newRange = this->range-visibility;

	int rowdiff = abs(unit2->getPosition().first - pos.first);
	int coldiff = abs(unit2->getPosition().second - pos.second);

	//valid moves
	if(rowdiff <=newRange && coldiff <=newRange) valid = true;
	if(rowdiff <=newRange && coldiff ==0) valid =true;
	if(rowdiff ==0 && coldiff <=newRange) valid =true;

	if(valid == false){
		printf("Sniper cannot attack that Unit. Sniper's current range is %d\n", range);
		return false;
	}
	
	else{
		int newHealth = unit2->getHealth() - this->damage;
		unit2->setHealth(newHealth);
	}
	if(unit2->getHealth() <=0) unit2->isDead= true;

	return true;
}


//*****UNIT -----> ARTILLERY*******
Artillery::Artillery(int row, int col){
	this->health = 6000;
	this->damage = 475;
	this->range = 4;
	this->pos = make_pair(row,col);
	this->moveCheck = false;
	this->deadCount =0;
	this->isDead = false;
}

bool Artillery::move(vector<vector<char> >grid,pair<int,int> des, int row, int col){
	bool valid = false;
	if(grid[des.first][des.second] != '_'){
		printf("Cannot move! A unit is already there\n");
		return false;
	}
	
	if(des.first <0 || des.first >=row || des.second<0 || des.second >=col){
		printf("Invalid Move! You went out of bounds!\n");
		return false;
	}


	int rowdiff = abs(des.first - pos.first);
	int coldiff = abs(des.second - pos.second);
	
	if(rowdiff == 0 && coldiff == 3) valid = true;
	if(rowdiff == 3 && coldiff == 0) valid = true;

	if(valid ==false){
		printf("Invalid move! Artillery moves 3 tiles at time in any adjacent direction to current position.\n");
		return false;
	}
	else{
		pos.first = des.first;
		pos.second = des.second;
	}
	moveCheck = true;
	return true;
}

bool Artillery::attack(int visibility,Unit* unit2){
	bool valid = false;
	int newRange = this->range-visibility;
	if(newRange !=4 && newRange != 3){
		printf("Due to visibility, this unit cannot be used because range isnt 4 or 3\nCurrent range: %d\n",newRange);
		return false;
	}
	int rowdiff = abs(unit2->getPosition().first - pos.first);
	int coldiff = abs(unit2->getPosition().second - pos.second);

	//valid moves
	if((rowdiff <=newRange  && rowdiff>2) && (coldiff <=newRange && coldiff >2)) valid = true;
	if((rowdiff <=newRange && rowdiff>2) && coldiff ==0) valid =true;
	if(rowdiff ==0 && (coldiff <=newRange && coldiff >2)) valid =true;

	if(valid == false){
		printf("Artillery cannot attack that Unit. Artillery's current range is %d\n", range);
		return false;
	}
	
	else{
		int newHealth = unit2->getHealth() - this->damage;
		unit2->setHealth(newHealth);
	}
	if(unit2->getHealth() <=0) unit2->isDead= true;

	return true;
}
	

//*****UNIT -----> INFANTRY*******
Infantry::Infantry(int row, int col){
	this->health = 3000;
	this->damage = 1100;
	this->range = 1;
	this->pos = make_pair(row,col);
	this->moveCheck = false;
	this->deadCount =0;
	this->isDead =false;
}

bool Infantry::move(vector<vector<char> >grid,pair<int,int>des, int row, int col){
	bool valid = false;
	if(grid[des.first][des.second] != '_'){
		printf("Cannot move! A unit is already there\n");
		return false;
	}
	if(des.first <0 || des.first >=row || des.second<0 || des.second >=col){
		printf("Invalid Move! You went out of bounds!\n");
		return false;
	}

	int rowdiff = abs(des.first - pos.first);
	int coldiff = abs(des.second - pos.second);

	//valid moves
	if(rowdiff ==1 && coldiff ==0) valid = true;
	if(rowdiff ==0 && coldiff ==1) valid =true;

	if(valid == false){
		printf("Invalid destination. Infantry only goes 1 tiles adjacent to its current position\n");
		return false;
	}
	
	//update the unit's current position to the destination postion
	else{
		pos.first = des.first;
		pos.second = des.second;
	}
		moveCheck = true;
		return true;
}

bool Infantry::attack(int visibility,Unit* unit2){
	bool valid = false;
	int newRange = this->range-visibility;
	if(newRange != 1){
		printf("Cant attack due to visibility\n");
		return false;
	}
	int rowdiff = abs(unit2->getPosition().first - pos.first);
	int coldiff = abs(unit2->getPosition().second - pos.second);

	//valid moves
	if(rowdiff ==newRange && coldiff ==newRange) valid = true;

	if(valid == false){
		printf("Infantry cannot attack that Unit. Infantry's current range is %d\n", range);
		return false;
	}
	
	else{
		int newHealth = unit2->getHealth() - this->damage;
		unit2->setHealth(newHealth);
	}
	if(unit2->getHealth() <=0) unit2->isDead= true;

	return true;

}


//*****UNIT -----> CAVALRY*******
Cavalry::Cavalry(int row, int col,int columnsize){
	this->health = 4000;
	this->damage = 200;
	this->range = columnsize;
	this->pos = make_pair(row,col);
	this->moveCheck = false;
	this->deadCount =0;
	this->isDead=false;
}

bool Cavalry::move(vector<vector<char> >grid,pair<int,int>des, int row, int col){
	bool valid = false;
	if(grid[des.first][des.second] != '_'){
		printf("Cannot move! A unit is already there\n");
		return false;
	}

	if(des.first <0 || des.first >=row || des.second<0 || des.second >=col){
		printf("Invalid Move! You went out of bounds!\n");
		return false;
	}

	int rowdiff = abs(des.first - pos.first);
	int coldiff = abs(des.second - pos.second);
	//valid moves
	if(rowdiff ==0 && coldiff ==1) valid = true;

	if(valid == false){
		printf("Invalid destination. Cavalry only goes 1 tile up or down its current position\n");
		return false;
	}
	
	//update the unit's current position to the destination postion
	else{
		pos.first = des.first;
		pos.second = des.second;
	}
		moveCheck = true;
		return true;
}

bool Cavalry::attack(int visibility,Unit* unit2){
	bool valid = false;
	int newRange = this->range-visibility;
	int rowdiff = abs(unit2->getPosition().first - pos.first);
	int coldiff = abs(unit2->getPosition().second - pos.second);

	//valid moves
	if(rowdiff ==0 && coldiff <=newRange) valid = true;

	if(valid == false){
		printf("Cavalry cannot attack that Unit. Cavalry current range is %d\n", range);
		return false;
	}
	
	else{
		int newHealth = unit2->getHealth() - this->damage;
		unit2->setHealth(newHealth);
	}
	if(unit2->getHealth() <=0) unit2->isDead= true;

	return true;
}

//*****UNIT -----> BIKER*******
Biker::Biker(int row, int col){
	this->health = 1500;
	this->damage = 425;
	this->range = 1;
	this->pos = make_pair(row,col);
	this->moveCheck = false;
	this->deadCount =0;
	this->isDead =false;
}

bool Biker::move(vector<vector<char> >grid,pair<int,int>des, int row, int col){
	bool valid = false;
	if(grid[des.first][des.second] != '_'){
		printf("Cannot move! A unit is already there\n");
		return false;
	}
	
	if(des.first <0 || des.first >=row || des.second<0 || des.second >=col){
		printf("Invalid Move! You went out of bounds!\n");
		return false;
	}

	int rowdiff = abs(des.first - pos.first);
	int coldiff = abs(des.second - pos.second);

	if(rowdiff ==0 && coldiff ==0){
		printf("Destination is same as current position\n");
		return false;
	}
	
	//valid moves
	if(rowdiff ==0 && coldiff <=8) valid = true;
	if(rowdiff <=8 && coldiff ==0) valid = true;
	if(rowdiff <=8 && coldiff <=8) valid = true;
	
	if(valid == false){
		printf("Invalid destination. Biker goes up to 8 tiles in any direction from its current position\n");
		return false;
	}
	
	//update the unit's current position to the destination postion
	else{
		pos.first = des.first;
		pos.second = des.second;
	}
		moveCheck = true;
		return true;
}

bool Biker::attack(int visibility, Unit* unit2){
	bool valid = false;
	int newRange = this->range-visibility;
	if(newRange != 1){
		printf("Cant attack due to visibility\n");
		return false;
	}
	int rowdiff = abs(unit2->getPosition().first - pos.first);
	int coldiff = abs(unit2->getPosition().second - pos.second);

	//valid moves
	if(rowdiff ==0 && coldiff ==newRange) valid = true;
	if(rowdiff ==newRange &&coldiff ==0) valid = true;
	if(valid == false){
		printf("Biker cannot attack that Unit. Biker's current range is %d\n", range);
		return false;
	}
	
	else{
		int newHealth = unit2->getHealth() - this->damage;
		unit2->setHealth(newHealth);
	}
	if(unit2->getHealth() <=0) unit2->isDead= true;

	return true;


	
	return true;
}

