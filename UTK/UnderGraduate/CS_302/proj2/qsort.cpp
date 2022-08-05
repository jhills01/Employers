// qsort.cpp

#include "volsort.h"
#include <cstdlib>
#include <array>
#include <iostream>

void qsort_sort(List &l, bool numeric) 
{
    Node *nodePtr = l.head->next;
	int i = 0;
	Node *nodePtrArr[l.size];

	//load "nodePtrArr" with pointers to all nodes from input
	while (nodePtr != NULL)
	{
		nodePtrArr[i] = nodePtr;
		++i;
		nodePtr = nodePtr->next;
	}

	//sort "nodePtrArr" elements either by "string" or "number" Node members
	if (numeric == 1) {qsort(nodePtrArr, l.size, sizeof(void*), node_qnumber_compare);}
	else {qsort(nodePtrArr, l.size, sizeof(void*), node_qstring_compare);}

	//update pointers of original list to reflect sorted order
	i = l.size - 1;
	l.head = nodePtrArr[i];

	while (i > 0)
	{
		nodePtrArr[i]->next = nodePtrArr[i-1];
		--i;
		if (i == 0) {nodePtrArr[i]->next = NULL;}	
	}
}

