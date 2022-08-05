//Tom Hills (jhills)
//Lab 8 - Dlists (part 1)

#include <iostream>
#include <cstdlib>
#include "dlist.h"

using namespace std;

//allocates "sentinel" Dnode object and initalizes all Dlist member variables
Dlist::Dlist()
{
	this->sentinel = new Dnode;
	this->sentinel->s = "";
	this->sentinel->flink = this->sentinel;
	this->sentinel->blink = this->sentinel;
	this->size = 0;
}

//deallocates all Denode objects associated with LIST
Dlist::~Dlist() 
{
	Dnode *i, *temp;
	
	//go through list and deallocate all LIST elements excluding "sentinel"
	for (i = this->Begin(); i != this->End();)
	{
		temp = i;
		i = i->flink;
		delete temp;
	}

	//last, deallocate "sentinel"
	delete this->sentinel;
}

//returns 1 (true) if LIST is empty or 0 (false) if not empty
int Dlist::Empty()
{
	if ( (this->sentinel->blink == this->sentinel) && (this->sentinel->flink == this->sentinel) ) {return 1;}
	return 0;
}

//return LIST size	
int Dlist::Size()
{
	return this->size;
}

//inserts Dnode object containing string s to the front of the LIST 
void Dlist::Push_Front(string s)
{
	//create new Dnode object and initalize member variables such that it is at the beginning of the LIST
	Dnode * newDnode = new Dnode;
	Dnode * previouslyFirstNode = this->sentinel->flink;

	//assigning string and pointers of newDnode
	newDnode->s = s;
	newDnode->flink = previouslyFirstNode;
	newDnode->blink = this->sentinel;	
	
	//re-assigning surrounding Dnodes
	previouslyFirstNode->blink = newDnode;
	this->sentinel->flink = newDnode;

	//update LIST size	
	this->size++;
}

//inserts Dnode object containing string s to the back of the LIST 
void Dlist::Push_Back(string s)
{
	//create new Dnode object and initalize member variables such that it is at the end of the LIST
	Dnode * newDnode = new Dnode;
	Dnode * previouslyLastNode = this->sentinel->blink;

	//assigning string and pointers of newDnode
	newDnode->s = s;
	newDnode->flink = this->sentinel;
	newDnode->blink = previouslyLastNode;
	
	//re-assigning surround Dnodes
	previouslyLastNode->flink = newDnode;
	this->sentinel->blink = newDnode;
	
	//update LIST size	
	this->size++;
}

//removes first element from LIST reducing LIST size by 1
string Dlist::Pop_Front()
{
	if (this->Empty()) {/*do nothing but return empty string as there is nothing to pop off*/ return "";}
	
	else
	{
		//store string of element to be deallocated and erased from LIST
		string str = this->sentinel->flink->s;
		
		this->Erase(this->sentinel->flink);
		
		//return the string from the erased LIST element
		return str;
	}
}

//removes last element from LIST reducing LIST size by 1
string Dlist::Pop_Back()
{
	if (this->Empty()) {/*do nothing but return empty string as there is nothing to pop off*/ return "";}
	
	else
	{
		//store string of element to be deallocated and erased from LIST
		string str = this->sentinel->blink->s;

		this->Erase(this->sentinel->blink);

		//return the string from the erased LIST element
		return str;
	}
}

//returns pointer to first Dnode object in LIST if LIST is not empty
Dnode * Dlist::Begin()
{
	//If list is empty, return pointer to the sentinel node
	if (this->Empty()) {return this->sentinel;}
	else {return this->sentinel->flink;}
}

//returns pointer to the LIST's "sentinel" Dnode
Dnode * Dlist::End()
{
	return this->sentinel;
}

//returns pointer to last Dnode object in LIST, or pointer to "sentinel" Dnode if LIST is empty
Dnode * Dlist::Rbegin() 
{
	//If list is empty, return pointer to the sentinel node
	if (this->Empty()) {return this->sentinel;}
	else {return this->sentinel->blink;}
}

//returns pointer to "sentinel" Dnode
Dnode * Dlist::Rend()
{
	return this->sentinel;
}

//insert new Dnode object BEFORE given Dnode parameter increasing LIST size by 1
void Dlist::Insert_Before(string s, Dnode *n)
{
	//new Dnode goes between "n->blink" and "n" Dnodes
	Dnode * newDnode = new Dnode;
	Dnode * b4NewDnode = n->blink;

	//re-assign pointers of surrounding Dnodes
	b4NewDnode->flink = newDnode;
	n->blink = newDnode;
	
	//assign string and pointers of newDnode
	newDnode->s = s;
	newDnode->flink = n;
	newDnode->blink = b4NewDnode;
	
	//update LIST size
	this->size++;
}

//insert new Dnode object AFTER given Dnode parameter increasing LIST size by 1
void Dlist::Insert_After(string s, Dnode *n)
{
	//new Dnode goes between "n" and "n->flink" Dnodes
	Dnode * newDnode = new Dnode;
	Dnode * afterNewDnode = n->flink;

	//re-assign pointers of surrounding Dnodes
	afterNewDnode->blink = newDnode;
	n->flink = newDnode;
	
	//assign string and pointers of newDnode
	newDnode->s = s;
	newDnode->blink = n;
	newDnode->flink = afterNewDnode;

	//update LIST size
	this->size++;
}

//erase Dnode parameter object from LIST reducing size by 1
void Dlist::Erase(Dnode *n)
{
	if(n == this->sentinel) {/*Do nothing as deleting the sentinel node will destroy the LIST*/return;} 

	else
	{
		Dnode * beforeDnode = n->blink;
		Dnode * afterDnode = n->flink;

		//re-assign blink and flink pointers before deallocating Dnode parameter
		beforeDnode->flink = afterDnode;
		afterDnode->blink = beforeDnode;
		delete n;

		this->size--;
	}
}
