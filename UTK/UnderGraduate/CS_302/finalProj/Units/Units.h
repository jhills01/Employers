/* Shivam Patel
 * Description: used inhertitance for each unit and virtual functions 
*/

using namespace std;
#include <iostream>
#include <vector>
class Unit{
	public:
		int getHealth();
		void setHealth(int);
		int getRange();
		int getDamage();
		bool getMove();
		void setMove(bool);
		pair<int,int> getPosition();
		void print();
		virtual bool move(vector<vector<char> >,pair<int,int>,int,int) =0; //grid vector, destination position, rowsize, column size
		virtual bool attack(int, Unit*)   =0; //visibility, the unit being attacked
		int deadCount;
		bool isDead;
	protected:
		pair<int,int>pos;
		int health;
		int damage;
		int range;
		bool moveCheck;

};

class Base : public Unit{
	public:
		bool move(vector<vector<char> >,pair<int,int>, int, int);
		bool attack(int, Unit*);
		Base(int,int);
};

class Sniper : public Unit{
	public:	
		bool move(vector<vector<char> >,pair<int,int>, int, int);
		bool attack(int, Unit*);
		Sniper(int, int);
};

class Artillery : public Unit{
	public:	
		bool move(vector<vector<char> >,pair<int,int>, int , int);
		bool attack(int, Unit*);
		Artillery(int,int);
};

class Infantry : public Unit{
	public:	
		bool move(vector<vector<char> >,pair<int,int>, int, int);
		bool attack(int,Unit*);
		Infantry(int, int);
};

class Cavalry : public Unit{
	public:	
		bool move(vector<vector<char> >,pair<int,int>, int, int);
		bool attack(int,Unit*);
		Cavalry(int, int, int);
};

class Biker : public Unit{
	public:
		bool move(vector<vector<char> >,pair<int,int>, int, int);
		bool attack(int,Unit*);
		Biker(int, int);
};



