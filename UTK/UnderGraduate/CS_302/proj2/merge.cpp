// merge.cpp

#include "volsort.h"
#include <cstring>
#include <iostream>

// Prototypes

Node *msort(Node *head, bool numeric);
void  split(Node *head, Node *&left, Node *&right);
Node *merge(Node *left, Node *right, bool numeric);

// Implementations

void merge_sort(List &l, bool numeric) 
{
	Node *head = l.head->next;
	l.head = msort(head, numeric);
}

Node *msort(Node *head, bool numeric) 
{
	//base case
	if (head->next == NULL) {return head;}
	if (head->next->next == NULL)
	{
		if (numeric == 1) 
		{
			if (head->number > head->next->number)
			{
				//swap nodes
				Node *newHead = head->next;
				head->next = head->next->next;
				newHead->next = head;
				
				return newHead;
			}
		}
		else 
		{
			if (head->string > head->next->string)
			{
				//swap nodes
				Node *newHead = head->next;
				head->next = head->next->next;
				newHead->next = head;
				
				return newHead;
			}
		}

		return head;
	}

	Node *left = head;
	Node *right = head;

	//divide ---- "left" will head left-most portion, and "right" will head right-most portion of split
	split (head, left, right);
	
	//recursive calls for further dividing and merging
	left = msort(left, numeric);
	right = msort(right, numeric);
	
	//conquer
	return merge(left, right, numeric);
}


void split(Node *head, Node *&left, Node *&right) 
{
	//"right != NULL" accounts for a list with an even number of nodes
	//"right->next != NULL" accounts for a list with an odd number of nodes
	while (right != NULL && right->next != NULL) 
	{
		left = left->next;
		right = right->next->next;
	}

	//rearrangment pointers to create two NULL termianted lists ("left" heads left-most, "right" heads right-most)
	right = left->next;
	left->next = NULL;
	left = head;
}

Node *merge(Node *left, Node *right, bool numeric) 
{
	Node head;
	Node *headPtr = &head;

	//use "node_number_compare"
	if (numeric == 1)
	{
		while (left != NULL && right != NULL)
		{
			//"left" greater than "right"
			if (node_number_compare(left, right))	
			{
				headPtr->next = right;
				headPtr = right;
				right = right->next;
			}
			//"left" less than or equal "right"
			else
			{
				headPtr->next = left;
				headPtr = left;
				left = left->next;
			}
		}
	}

	//use "node_string_compare"
	else 
	{
		while (left != NULL && right != NULL)
		{
			//"left" greater than "right"
			if (node_string_compare(left, right))	
			{
				headPtr->next = right;
				headPtr = right;
				right = right->next;
			}
			//"left" less than or equal "right"
			else
			{
				headPtr->next = left;
				headPtr = left;
				left = left->next;
			}
		}
	}

	//tack on leftover Nodes
	headPtr->next = (left == NULL) ? right : left;

	return head.next;
}
