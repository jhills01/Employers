// quick.cpp

#include "volsort.h"

#include <iostream>

// Prototypes

Node *qsort(Node *head, bool numeric);
void  partition(Node *head, Node *pivot, Node *&left, Node *&right, bool numeric);
Node *concatenate(Node *left, Node *right);

// Implementations

void quick_sort(List &l, bool numeric) 
{
	//print list first to make sure
//	std::cout << "l.head:\n";
//	dump_node(l.head);
//	std::cout << "\n\n";
//	std::cout << "l.head->next:\n";
//	dump_node(l.head->next);
//	exit(1);

	l.head = qsort(l.head->next, numeric);//not sure about passing "l.head->next"
}

Node *qsort(Node *head, bool numeric) 
{
	//NOTE: make sure to put "NULL" in pivot's next when pivot is put into final spot

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
				
//				//just swapping variables. No need to rearrange pointers
//				int temp = head->number;
//				head->number = head->next->number;
//				head->next->number = temp;
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

//				//just swapping variables. No need to rearrange pointers
//				std::string temp = head->string;
//				head->string = head->next->string;
//				head->next->string = temp;
			}
		}

		return head;
	}	
	
	Node *left = head->next; //head->next?? 
	Node *right = head->next;//head->next?? 
	Node *pivot = head; //should be first element in list

	//TEST print
	std::cout << "BEFORE PARTITION:\n";
	std::cout << "dump_node(left):\n";
	dump_node(left);
	std::cout << "dump_node(left):\n";
	dump_node(left);
	std::cout << "\n\n";
	//TEST print

	partition(head, pivot, left, right, numeric);

	//TEST print
	std::cout << "AFTER PARTITION:\n";
	std::cout << "dump_node(left):\n";
	dump_node(left);
	std::cout << "dump_node(right):\n";
	dump_node(right);
	exit(1);
	//TEST print

	left = qsort(left, numeric);
	right = qsort(right, numeric);

	
	return concatenate(left, right);
}

void partition(Node *head, Node *pivot, Node *&left, Node *&right, bool numeric) 
{
	//DON'T FORGET TO PUT "NULL" ON LAST ELEMENT OF "LEFT" HAVE...i.e., pivot->next = NULL;
	Node *previousLeft = left; //this pointer will point to the node that left was previously. This way, I possibly have a way of 
							   //...having a pointer to the node that is the end of the 1st half of partition
	Node *previousRight = right;
	//temp variables for swapping!
	std::string stemp;
	int itemp;

	//assign previousLeft AFTER a swap!!

	//"left" pointer seeks values greater than pivot value
	//"right" pointer seeks values less than or equal to pivot value
	if (numeric == 1)
	{
		while (left != NULL && right != NULL)
		{
			//swapping
			if ( (left->number > pivot->number) && (right->number <= pivot->number) )
			{
				itemp = left->number;
				left->number = right->number;
				right->number = itemp;

			//	previousLeft = left; //NOTE: moving the previous pointer stuff to ELSE IF portions
			}	
		
			//MIGHT BE FASTER WITHOUT USING "ELSE-IFS"
			//NOTE: Once left and right pointers are settled on something, only a swap will allow them to start moving again!!!
			//...in this way, I think I cna get rid of "else-if"s and just use ifs to speed things up
			//FURTHER NOTE: Actually, I think I need the ELSE-IF because this limits left moving BEFORE right begins to move!!!!
			//...I think I need this "one pointer move at a time" scheme to ensure ...something

			//progressing left pointer
			else if (left->number <= pivot->number) {previousLeft = left; left = left->next;}

			//progressing right pointer --- MIGHT think about changing this to just "else"
			else if (right->number > pivot->number) {previousRight = right; right = right->next;}
		}
	}

	else 
	{
		while (left != NULL && right != NULL)
		{
			//swapping
			if ( (left->string > pivot->string) && (right->string <= pivot->string) )
			{
				stemp = left->string;
				left->string = right->string;
				right->string = stemp;

//				previousLeft = left;
			}	
		
			//progressing left pointer
			else if (left->number <= pivot->number) {previousLeft = left; left = left->next;}

			//progressing right pointer --- MIGHT think about changing this to just "else"
			else if (right->number > pivot->number) {previousRight = right; right = right->next;}
		}
	}

	//NOTE: revamped
	//finalizing each partition
	if (numeric == 1)
	{
		if (left == NULL)
		{
			//IMPORTANT-----------------------------------------------------------------------------------------------
			//NOTE: I think it's better to do what Alex did, and redirect pointers instead of copying over values. 
			//BUT, in the cast of singly linked list, it's a pain in the ass to get an "in-place" solution. 
			//Alex's solution was to create a new "head" and attach "greater than pivot" values to it
			//he would make that the previous node's next pointer skip that greater value and point to the next node.
			//This way, once he ran out of greater than values, he had one list with greater than (new head, but all pointers were same_
			//...and the skipping code allowed the other list to have just less than or equal values to pivot
			//NOTE NOTE NOTE:  he said that eventually, you could get a list of multiple values equaling the pivot....
			//this would cause infinite recursion and blow up the stack. he said you need to put the one pivot value in
			//greater than list so that the repeat stuff would eventually die of
			//IMPORTANT-----------------------------------------------------------------------------------------------
			
			//swap "pivot" number with "right" number
			itemp = pivot->number;
			pivot->number = right->number;
			right->number = itemp;

			//assign pointers such that "left" heads new list (NULL termianted) with less than or equal to pivot value 
			//...and "right" heads other new list (NULL termianted) with greater than pivot values.
			left = right;
			previousRight->next = NULL;
			right = pivot;
		}

		else if (right == NULL)
		{
			//swap "pivot" number with "previousLeft" number
			itemp = pivot->number;
			pivot->number = previousLeft->number;
			previousLeft->number = itemp;

			//assign pointers such that "left" heads new list (NULL termianted) with less than or equal to pivot value 
			//...and "right" heads other new list (NULL termianted) with greater than pivot values.
			previousLeft->next = NULL;
			right = left;
			left = pivot;
		}
	}

	else
	{
	
	}
	
//	//finalizing each partition 
//	if (numeric == 1) 
//	{
//		itemp = pivot->number;
//		pivot->number = previousLeft->number;
//		previousLeft->number = itemp;
//	}
//	else
//	{
//		stemp = pivot->string;
//		pivot->string = previousLeft->string;
//		previousLeft->string = itemp;
//	}
//	previousLeft->next = NULL;
//	right = left;
//	left = pivot;
}

Node *concatenate(Node *left, Node *right) 
{
	//NOTE: left and right should already be sorted AND be separated such that all members in left are > right or vice versa
	//Thus, I think we just compare the first element of each list and make the smaller one the head.
	
	
	
	
	return left; //TEMPORARY. still need to check and concatenate
}

