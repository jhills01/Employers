#include "Units.h"
#include <iostream>
using namespace std;


int main(){

	//testing the print function for each unit which will give the range, health, and damage but just health for base
	Sniper s;
	Artillery a;
	Infantry i;
	Cavalry c(35);
	Biker bi;
	Base b;

	Unit *unit1 = &s;
	Unit *unit2 = &a;
	Unit *unit3 = &i;
	Unit *unit4 = &c;
	Unit *unit5 = &bi;
	Unit *unit6 = &b;

	unit1->print();
	cout<<endl;
	unit2->print();
	cout<<endl;
	unit3->print();
	cout<<endl;
	unit4->print();
	cout<<endl;
	unit5->print();
	cout<<endl;
	unit6->print();
	return 0;
}
