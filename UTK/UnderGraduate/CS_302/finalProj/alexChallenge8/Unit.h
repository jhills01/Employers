//Header file for unit library

#include <string>
using namespace std;

//All units are derived from a basic class, that way they all
//share similar functions and can be easily stored together in the same container using
//pointers.
class Unit{
public:
	virtual int getMovesLeft() =0;
	virtual int getMaxMoves()=0;
	virtual int getUsedMoves()=0;
	virtual void setUsedMoves(int usedMoves)=0;
	virtual bool move(int row, int col) =0;
};

// Main Base class. Cannot move, but can attack. Future versions
// will add a health to this, which will decide who wins and loses.
class Base : public Unit{
public:
	Base(int r, int c);
	int getMovesLeft() {return maxMoves-usedMoves;}
	int getMaxMoves() {return maxMoves;}
	int getUsedMoves() {return usedMoves;}
	void setUsedMoves(int usedMoves) {this->usedMoves=usedMoves;}


	bool move(int row, int col);
	int r;
	int c;
	string name;
private:
	int maxMoves;
	int usedMoves;
};


// Basic attack unit. Can attack and move 1 tile. Future versions
// will allow this to move, and attack other tiles.
class Infantry : public Unit{
public:
	Infantry(int r, int c);
	int getMovesLeft() {return maxMoves-usedMoves;}
	int getMaxMoves() {return maxMoves;}
	int getUsedMoves() {return usedMoves;}
	void setUsedMoves(int usedMoves) {this->usedMoves=usedMoves;}

	bool move(int row, int col);
	int r;
	int c;
	string name;
private:
	int maxMoves;
	int usedMoves;
};
