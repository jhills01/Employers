/* Shivam Patel
 * Description: used inhertitance for each unit and virtual functions 
*/

using namespace std;
#include <iostream>

class Unit{
	public:
		virtual int getHealth() =0;
		virtual int getRange()  =0;
		virtual int getDamage() =0;
		virtual void print()    =0;
//		virtual void move(vector<vector<char>>&grid, int numtiles)     =0;
//		virtual void attack(int visibility)   =0;
	

};

class Sniper : public Unit{
	public:	
		int getHealth();	
		int getRange();
		int getDamage();
		void print();
		void move();
		void attack();
		Sniper();
	private:
		int health;
		int damage;
		int range;
};

class Base : public Unit{
	public:
		int getHealth();
		int getRange();
		int getDamage();
		void print();
		void move();
		void attack();
		Base();
	private:
		int health;
		int damage;
		int range;
};

class Artillery : public Unit{
	public:	
		int getHealth();
		int getRange();
		int getDamage();
		void print();
		void move();
		void attack();
		Artillery();
	private:
		int health;
		int damage;
		int range;
};

class Infantry : public Unit{
	public:	
		int getHealth();
		int getRange();
		int getDamage();
		void print();
		void move();
		void attack();
		Infantry();
	private:
		int health;
		int damage;
		int range;	

};

class Cavalry : public Unit{
	public:	
		int getHealth();
		int getRange();
		int getDamage();
		void print();
		void move();
		void attack();
		Cavalry(int rowsize);
	private:
		int health;
		int damage;
		int range;
};

class Biker : public Unit{
	public:	
		int getHealth();
		int getRange();
		int getDamage();
		void print();
		void move();
		void attack();
		Biker();
	private:
		int health;
		int damage;
		int range;
};



