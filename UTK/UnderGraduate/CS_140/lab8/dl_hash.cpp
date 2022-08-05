//Tom Hills (jhills)
//Lab 8 - Dlists (part 2)

#include "dl_hash.h"
#include <cstdio>

using namespace std;

//searches Dlist object for Dnode with string member == "s" 
Dnode *findDlistStr(Dlist *dlist, string &s)
{	
	Dnode *iterator;

	for (iterator = dlist->Begin(); iterator != dlist->End();)
	{
		if (iterator->s == s) {/*match for "s" found. */ return iterator;}
		iterator = iterator->flink;
	}

	//no match for "s" found. Return NULL.
	return NULL;
}

//hash function
unsigned int djbHash(string &s)
{
  int i;
  unsigned int h;
  
  h = 5381;

  for (i = 0; i < s.size(); i++) {
    h = (h << 5) + h + s[i];
  }
  return h;
}

//DL_Hash constructor
DL_Hash::DL_Hash(int size)
{
	//allocate and intialize "table" member
	Dlist *newDlistPtr;
	
	for (int i = 0; i < size; i++)
	{
		newDlistPtr = new Dlist;
		this->table.push_back(newDlistPtr);
	}	
}

//DL_Hash deconstructor
DL_Hash::~DL_Hash()
{
	for (int i = 0; i < this->table.size(); i++)
	{
		delete this->table[i];
	}
}

//inserts new Dnode into Dlist object of  hash table at index determined by hash function
void DL_Hash::Insert(string &s)
{
	//check to see if "s" is already in the hash table.
	if (this->Present(s) == 1) {return;}

	//insert "s" into hash table
	unsigned hashValue = djbHash(s);
	int hashIndex = hashValue % this->table.size();
	Dlist *hashListElement = this->table[hashIndex];
	hashListElement->Push_Back(s);
	return;
}

//return 1 (true) if string "s" is already in the hash table, and 0 (false) otherwise
int DL_Hash::Present(string &s)
{
	unsigned hashValue = djbHash(s);
	int hashIndex = hashValue % this->table.size();
	Dlist *hashListElement = this->table[hashIndex];
	
	if (findDlistStr(hashListElement, s) == NULL) {return 0;}
	else {return 1;}
}

//erase Dnode object from Dlist object of hash table at index determined by hash function
void DL_Hash::Erase(string &s)
{
	//check to see if "s" is not in the hash table.
	if (this->Present(s) == 0) {return;}

	//find and erase "s" from hash table
	unsigned int hashValue = djbHash(s);
	int hashIndex = hashValue % this->table.size();
	Dlist *hashListElement = this->table[hashIndex];

	//erase Dnode object with string member == "s"
	hashListElement->Erase(findDlistStr(hashListElement, s));
}

//erases all Dnode objects within hash table that contain substring, s.
void DL_Hash::Strip_All_Substring(string &s)
{
	int tableSize = this->table.size();
	Dlist *hashListElement;
	Dnode *iterator, *temp;

	for (int i = 0; i < tableSize; i++)
	{
		if (this->table[i]->Empty() == 1) {/*do nothing. LIST element is empty*/}
		
		//searches for substring and erases all Dnode objects containing substring
		else
		{
			hashListElement = this->table[i];

			for (iterator = hashListElement->Begin(); iterator != hashListElement->End();)
			{
				if (iterator->s.find(s) != string::npos) 
				{
					temp = iterator; 
					iterator = iterator->flink;
					hashListElement->Erase(temp);
				}
				
				else {iterator = iterator->flink;} 
			}
		}
	}
}

//prints entire contents of hash table 
void DL_Hash::Print()
{
	int tableSize = this->table.size();
	Dlist *hashListElement;
	Dnode *iterator;

	for (int i = 0; i < tableSize; i++)
	{
		if (this->table[i]->Empty() == 1) {/*do nothing. LIST element is empty*/}
		
		//prints Dnode object strings from beginning to end of Dlist, one per line preceded by hash table index 
		else
		{
			hashListElement = this->table[i];

			for (iterator = hashListElement->Begin(); iterator != hashListElement->End();)
			{
				printf("%4d %s\n", i, iterator->s.c_str());
				iterator = iterator->flink;	
			}
		}
	}
}
