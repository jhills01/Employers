//Defintions for the larger functions in the Unit library.

#include "Unit.h"
#include <string>
#include <cmath>
using namespace std;

// Constructs the Base at the proper location, with the proper
// Move count and name.
Base::Base(int r, int c){
	this->r = r;
	this->c = c;
	maxMoves =0;
	usedMoves=0;
	this->name = "Base";
}


// Since the Base cannot be moved, this action will always fail. The bool
// returned lets the main program tell the user if a move is valid.
bool Base::move(int row, int col){
	return false;
}


//Constructs an infantry at the proper location with the
//proper move count and name.
Infantry::Infantry(int r, int c){
	this->r=r;
	this->c=c;
	maxMoves = 2;
	usedMoves = 0;
	name = "Infantry";
}


// This function does not have much meat to it yet, as I am still working out how to handled
// movement checking in the main function. Most likely the checking will be done
// simply in the main function, but extra checking may be done here. 
bool Infantry::move(int row, int col){
	if(maxMoves <= (usedMoves + abs(row-r))){
		return false;
	}
	return true;
}
