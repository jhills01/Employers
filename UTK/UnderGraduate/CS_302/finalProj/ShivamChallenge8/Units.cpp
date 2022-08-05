// Name: Shivam Patel
// Description: Defined the getter functions and print  


using namespace std;
#include "Units.h"
#include <iostream>

//*****UNIT -----> BASE*******

Base::Base(){
	this->health = 20000;
	this->damage = 0;
	this->range = 0;
}

int Base::getHealth(){return health;}

int Base::getDamage(){return damage;}

int Base::getRange(){return range;}

void Base::print(){
	cout<<"Base Status:"<<endl<<"health: "<<health<<endl;
}


//*****UNIT -----> SNIPER*******
Sniper::Sniper(){
	this->health = 2500;
	this->damage = 550;
	this->range = 7;
}

int Sniper::getHealth(){return health;}

int Sniper::getDamage(){return damage;}

int Sniper::getRange(){return range;}

void Sniper::print(){
	cout<<"Sniper Status:"<<endl<<"health: "<<health<<endl<<"damage: "<<damage<<endl<<"range: "<<range<<endl;
}

//*****UNIT -----> ARTILLERY*******
Artillery::Artillery(){
	this->health = 6000;
	this->damage = 250;
	this->range = 2;
}

int Artillery::getHealth(){return health;}

int Artillery::getDamage(){return damage;}

int Artillery::getRange(){return range;}

void Artillery::print(){
	cout<<"Artillery Status:"<<endl<<"health: "<<health<<endl<<"damage: "<<damage<<endl<<"range: "<<range<<endl;
}

//*****UNIT -----> INFANTRY*******
Infantry::Infantry(){
	this->health = 3000;
	this->damage = 1100;
	this->range = 1;
}

int Infantry::getHealth(){return health;}

int Infantry::getDamage(){return damage;}

int Infantry::getRange(){return range;}

void Infantry::print(){
	cout<<"Infantry Status:"<<endl<<"health: "<<health<<endl<<"damage: "<<damage<<endl<<"range: "<<range<<endl;
}

//*****UNIT -----> CAVALRY*******
Cavalry::Cavalry(int rowsize){
	this->health = 4000;
	this->damage = 200;
	this->range = rowsize;
}

int Cavalry::getHealth(){return health;}

int Cavalry::getDamage(){return damage;}

int Cavalry::getRange(){return range;}

void Cavalry::print(){
	cout<<"Cavalry Status:"<<endl<<"health: "<<health<<endl<<"damage: "<<damage<<endl<<"range: "<<range<<endl;
}

//*****UNIT -----> BIKER*******
Biker::Biker(){
	this->health = 1500;
	this->damage = 425;
	this->range = 1;
}

int Biker::getHealth(){return health;}

int Biker::getDamage(){return damage;}

int Biker::getRange(){return range;}

void Biker::print(){
	cout<<"Biker Status:"<<endl<<"health: "<<health<<endl<<"damage: "<<damage<<endl<<"range: "<<range<<endl;
}

