#include "volsort.h"
#include <cstring>

List::List() 
{
	this->head = new Node();
	this->head->string = "";
	this->head->number = 0;
	this->head->next = NULL;
	this->size = 0;
}

List::~List() 
{
	Node *nodePtr = this->head->next;//1st node after head
	Node *tempNodePtr;

	//deletes all List Nodes except the "head"
	while(nodePtr != NULL)
	{
		tempNodePtr = nodePtr->next;
		delete nodePtr;
		nodePtr = tempNodePtr;
	}

	//deletes "head" which completely frees List object memory
	delete this->head;
}

//From volsort.h file to avoid "multiple definition" error
void List::push_front(const std::string &s)
{
	//create new Node and store node that will be replaced
	Node *newNode = new Node();
	Node *previous1stNode = this->head->next;

	//assign members of newNode
	newNode->string = s;
	newNode->number = stoi(s);//TEMPORARY
//		//NOTE: going to comment this try/catch out until I talk with TAs
//		try
//		{
//			newNode->number = std::stoi(s);
//		}
//		catch (const std::invalid_argument& ia)
//		{
//			//default to 0
//			number = 0;
//		}
	newNode->next = previous1stNode;

	//reassign head's next ptr
	this->head->next = newNode;
	
	//update list size
	this->size++;
}

bool node_number_compare(const Node *a, const Node *b) 
{
	return (a->number > b->number);
}

int node_qnumber_compare(const void *a, const void *b) 
{
	const Node *aN = *(const Node**)a;
	const Node *bN = *(const Node**)b;
	return (aN->number < bN->number);
}

bool node_string_compare(const Node *a, const Node *b)
{
	return (strcmp(a->string.c_str(), b->string.c_str()) > 0); 
//	return (a->string > b->string);	
}

int node_qstring_compare(const void *a, const void *b)
{
	const Node *aN = *(const Node**)a;
	const Node *bN = *(const Node**)b;
	return (strcmp(aN->string.c_str(), bN->string.c_str()) < 0); 
}

void dump_node(Node *n)			
{
	while (n != NULL)
	{
		printf("string: %s\tnumber: %d\n",n->string.c_str(), n->number);
		n = n->next;
	}
}
