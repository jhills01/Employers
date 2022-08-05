// Tom Hills (NETID: jhills, GIT: jhills01)

// Challenge 02: Adding List-Based Integers

// Overview: 
// This program reads a pair of numbers from standard input. Each number has each of its digits assigned to a ...
// ... single node of a doubly linked list such that the 1st node == 1s digit, 2nd node == 10s digit, 3rd == 100s, and so on.
// The two "number lists" are then added together and the result stored in a separate "number list" which is printed from the ...
// ... most significant digit until the least significant to standard output.

#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

//CLASS DEFINITIONS/IMPLEMENTATIONS-------------------------------------------------------------------------------------------

template<typename T>
class Dnode 
{
	public:
		T s;
		Dnode<T> *nextNode;
		Dnode<T> *prevNode;
};

template<typename T>
class Dlist 
{
	public:
		Dlist();
		~Dlist();
		int Empty(); 
		int Size(); 
		void Push_Front(T s);
		void Push_Back(T s);
		T Pop_Front();
		T Pop_Back();
		Dnode<T> *Begin(); 
		Dnode<T> *End();
		Dnode<T> *Rbegin();
		Dnode<T> *Rend();
		void Insert_Before(T s, Dnode<T> *n);
		void Insert_After(T s, Dnode<T> *n); 
		void Erase(Dnode<T> *n);
  
	protected:
		Dnode<T> *sentinel;
		int size;
};

//allocates "sentinel" Dnode object and initalizes all Dlist member variables
template<typename T>
Dlist<T>::Dlist()
{
    this->sentinel = new Dnode<T>;
    this->sentinel->nextNode = this->sentinel;
    this->sentinel->prevNode = this->sentinel;
    this->size = 0;
}

//deallocates all Denode objects associated with Dlist
template<typename T>
Dlist<T>::~Dlist()
{
    Dnode<T> *i, *temp;

    //go through list and deallocate all Dlist elements excluding "sentinel"
    for (i = this->Begin(); i != this->End();)
    {
        temp = i;
        i = i->nextNode;
        delete temp;
    }

    //last, deallocate "sentinel"
    delete this->sentinel;
}

//returns 1 (true) if Dlist is empty or 0 (false) if not empty
template<typename T>
int Dlist<T>::Empty()
{
    if ( (this->sentinel->prevNode == this->sentinel) && (this->sentinel->nextNode == this->sentinel) ) {return 1;}
    return 0;
}

//return Dlist size  
template<typename T>
int Dlist<T>::Size()
{
    return this->size;
}

//inserts Dnode object containing string s to the front of the Dlist 
template<typename T>
void Dlist<T>::Push_Front(T s) 
{
    //create new Dnode object and initalize member variables such that it is at the beginning of the Dlist
    Dnode<T> * newDnode = new Dnode<T>;
    Dnode<T> * previouslyFirstNode = this->sentinel->nextNode;

    //assigning string and pointers of newDnode
    newDnode->s = s;
    newDnode->nextNode = previouslyFirstNode;
    newDnode->prevNode = this->sentinel;

    //re-assigning surrounding Dnodes
    previouslyFirstNode->prevNode = newDnode;
    this->sentinel->nextNode = newDnode;

    //update Dlist size  
    this->size++;
}

//inserts Dnode object containing string s to the back of the Dlist 
template<typename T>
void Dlist<T>::Push_Back(T s) 
{
    //create new Dnode object and initalize member variables such that it is at the end of the Dlist
    Dnode<T> * newDnode = new Dnode<T>;
    Dnode<T> * previouslyLastNode = this->sentinel->prevNode;

    //assigning string and pointers of newDnode
    newDnode->s = s;
    newDnode->nextNode = this->sentinel;
    newDnode->prevNode = previouslyLastNode;

    //re-assigning surround Dnodes
    previouslyLastNode->nextNode = newDnode;
    this->sentinel->prevNode = newDnode;

    //update Dlist size  
    this->size++;
}

//removes first element from Dlist reducing Dlist size by 1
template<typename T>
T Dlist<T>::Pop_Front() 
{
    if (this->Empty()) {/*do nothing but return empty string as there is nothing to pop off*/ return "";}

    else
    {
        //store string of element to be deallocated and erased from Dlist
        T str = this->sentinel->nextNode->s; //originally: string str;

        this->Erase(this->sentinel->nextNode);

        //return the string from the erased Dlist element
        return str;
    }
}

//removes last element from Dlist reducing Dlist size by 1
template<typename T>
T Dlist<T>::Pop_Back() 
{
    if (this->Empty()) {/*do nothing but return empty string as there is nothing to pop off*/ return "";}

    else
    {
        //store string of element to be deallocated and erased from Dlist
        T str = this->sentinel->prevNode->s; //originally: string str;

        this->Erase(this->sentinel->prevNode);

        //return the string from the erased Dlist element
        return str;
    }
}

//returns pointer to first Dnode object in Dlist if Dlist is not empty
template<typename T>
Dnode<T> * Dlist<T>::Begin()
{
    //If list is empty, return pointer to the sentinel node
    if (this->Empty()) {return this->sentinel;}
    else {return this->sentinel->nextNode;}
}

//returns pointer to the Dlist's "sentinel" Dnode
template<typename T>
Dnode<T> * Dlist<T>::End()
{
    return this->sentinel;
}

//returns pointer to last Dnode object in Dlist, or pointer to "sentinel" Dnode if Dlist is empty
template<typename T>
Dnode<T> * Dlist<T>::Rbegin()
{
    //If list is empty, return pointer to the sentinel node
    if (this->Empty()) {return this->sentinel;}
    else {return this->sentinel->prevNode;}
}

//returns pointer to "sentinel" Dnode
template<typename T>
Dnode<T> * Dlist<T>::Rend()
{
    return this->sentinel;
}

//insert new Dnode object BEFORE given Dnode parameter increasing Dlist size by 1
template<typename T>
void Dlist<T>::Insert_Before(T s, Dnode<T> *n)
{
    //new Dnode goes between "n->prevNode" and "n" Dnodes
    Dnode<T> * newDnode = new Dnode<T>;
    Dnode<T> * b4NewDnode = n->prevNode;

    //re-assign pointers of surrounding Dnodes
    b4NewDnode->nextNode = newDnode;
    n->prevNode = newDnode;

    //assign string and pointers of newDnode
    newDnode->s = s;
    newDnode->nextNode = n;
    newDnode->prevNode = b4NewDnode;

    //update Dlist size
    this->size++;
}

//insert new Dnode object AFTER given Dnode parameter increasing Dlist size by 1
template<typename T>
void Dlist<T>::Insert_After(T s, Dnode<T> *n)
{
    //new Dnode goes between "n" and "n->nextNode" Dnodes
    Dnode<T> * newDnode = new Dnode<T>;
    Dnode<T> * afterNewDnode = n->nextNode;

    //re-assign pointers of surrounding Dnodes
    afterNewDnode->prevNode = newDnode;
    n->nextNode = newDnode;

    //assign string and pointers of newDnode
    newDnode->s = s;
    newDnode->prevNode = n;
    newDnode->nextNode = afterNewDnode;

    //update Dlist size
    this->size++;
}

//erase Dnode parameter object from Dlist reducing size by 1
template<typename T>
void Dlist<T>::Erase(Dnode<T> *n) 
{
    if(n == this->sentinel) {/*Do nothing as deleting the sentinel node will destroy the Dlist*/return;}

    else
    {
        Dnode<T> * beforeDnode = n->prevNode;
        Dnode<T> * afterDnode = n->nextNode;

        //re-assign prevNode and nextNode pointers before deallocating Dnode parameter
        beforeDnode->nextNode = afterDnode;
        afterDnode->prevNode = beforeDnode;
        delete n;

        this->size--;
    }
}

//FUNCTIONS-----------------------------------------------------------------------------------------------------------------

//prints the node values of type T from a Dlist object
//Starts on the 1st node AFTER sentinel and ends on the 1st node BEFORE sentinel
template<typename T>
void printDlist(Dlist<T> *dlist) 
{
	for (Dnode<T> *node = dlist->Begin(); node != dlist->End(); node = node->nextNode)
	{
		printf("%d", node->s); 
	}

	printf("\n");
}

//deletes all nodes of a Dlist oject except the sentinel node
template<typename T>
void clearDlist(Dlist<T> *dlist) 
{
	Dnode<T> *nextNode;
	
	for (Dnode<T> *node = dlist->Begin(); node != dlist->End();)
	{
		nextNode = node->nextNode;
		dlist->Erase(node);
		node  = nextNode;
	}
}

//adds each digit (0-9) from corresponding nodes of two Dlist objects and returns a Dlist object containing the result
//NOTE: since only two Dlist are being added, no single addition between node values will exceed 18 (i.e., 9+9)...
//... so "carry" will be either 0 or 1.
template<typename T>
Dlist<T>* addDlists(Dlist<T> *dlNum1, Dlist<T> *dlNum2)
{
	Dlist<T>* dlResult = new Dlist<T>();
	Dnode<T>* dlNum1Node = dlNum1->Begin();
	Dnode<T>* dlNum2Node = dlNum2->Begin();
	int added, carry = 0;
	
	//loop to add up nodes that have a corresponding node with the other Dlist. 
	for (; dlNum1Node != dlNum1->End() && dlNum2Node != dlNum2->End(); dlNum1Node = dlNum1Node->nextNode, dlNum2Node = dlNum2Node->nextNode)
	{
		added = dlNum1Node->s + dlNum2Node->s + carry;
		if (added > 9) {dlResult->Push_Front(added % 10); carry = 1;}
		else {dlResult->Push_Front(added); carry = 0;}
	}

	//if Dlists are both the same size, we can return the result with only checking the "carry". 
	if (dlNum1->Size() == dlNum2->Size() && carry == 1) {dlResult->Push_Front(1); return dlResult;}
	else if (dlNum1->Size() == dlNum2->Size() /* && carry == 0 */) {return dlResult;}
	
	//1st Dlist object was larger. After accounting for "carry", we simply copy remaining/lone node values to "dlResult". 
	while (dlNum1Node != dlNum1->End())
	{
		dlResult->Push_Front(dlNum1Node->s + carry);
		if (carry == 1) {carry = 0;}
		dlNum1Node = dlNum1Node->nextNode;
	}

	//2nd Dlist object was larger. After accounting for "carry", we simply copy remaining/lone node values to "dlResult". 
	while (dlNum2Node != dlNum2->End())
	{
		dlResult->Push_Front(dlNum2Node->s + carry);
		if (carry == 1) {carry = 0;}
		dlNum2Node = dlNum2Node->nextNode;
	}

	return dlResult;
}


// MAIN---------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) 
{
	Dlist<int> dlNum1, dlNum2; 
	Dlist<int>* dlResultPtr;
	string num1, num2;

	//reads in 1 file line at a time which contains two numbers separated by a space
	while (cin >> num1 >> num2)
	{
		//creates Dlist object, "dlNum1"
		for (unsigned int i = 0; i < num1.size(); ++i)
		{
			dlNum1.Push_Front(num1[i] - '0');	
		}

		//creates Dlist object, "dlNum2"
		for (unsigned int i = 0; i < num2.size(); ++i)
		{
			dlNum2.Push_Front(num2[i] - '0');
		}

		//adds two Dlist objects and returns the result to "dlResultPtr" which is printed, then deleted to avoid memory leaks
		dlResultPtr = addDlists(&dlNum1, &dlNum2);
		printDlist(dlResultPtr);
		delete dlResultPtr;

		//deletes all nodes except sentinel within Dlist objects in preparation for next input, if any.
		clearDlist(&dlNum1);		
		clearDlist(&dlNum2);		
	}

	return 0;
}
