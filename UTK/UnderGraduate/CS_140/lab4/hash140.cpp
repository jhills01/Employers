//Tom Hills (jhills)
//lab4 - hash table

#include "hash140.h"
#include <string>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

//non-member functions-------------------------------------------------------------------------------------------------------------------

//calculates and returns the hash value (i.e., vector index) for "keys" and "vals" vectors using "Last7" hashing technique
unsigned int Last7(string &key, int tableSize)
{
	unsigned int hash = 0;						
	unsigned int strHexToInt = 0;			
	unsigned int keySize = key.size();
	istringstream iss;
	string Last7;

	iss.clear();
	iss.str(key);
	
	if (keySize <= 7)
	{
		//get hash value using entire string
		iss.clear();
		if (!(iss >> hex >> strHexToInt)) {fprintf (stderr, "iss >> hex >> strHexToInt -- DID NOT WORK!!!\n");}
		hash = strHexToInt % tableSize;
	}
	else 
	{
		//get hash value using last 7 characters of key string
		Last7 = key.substr(keySize - 7); 
		iss.clear();
		iss.str(Last7);
		if (!(iss >> hex >> strHexToInt)) {fprintf (stderr, "iss >> hex >> strHexToInt -- DID NOT WORK!!!\n");} 
		hash = strHexToInt % tableSize;
	}
	
	return hash;
}

//calculates and returns the hash value (i.e., vector index) for "keys" and "vals" vectors using "XOR" hashing technique
unsigned int XOR(string &key, int tableSize)
{
	unsigned int hash = 0;						
	unsigned int strHexToInt = 0;			
	unsigned int keySize = key.size();
	istringstream iss;
	string temp7;

	iss.clear();
	iss.str(key);
	
	if (keySize <= 7)
	{
		//get hash value using entire string
		iss.clear();
		if (!(iss >> hex >> strHexToInt)) {fprintf (stderr, "iss >> hex >> strHexToInt -- DID NOT WORK!!!\n");}
		hash = strHexToInt % tableSize;
	}
	else 
	{
		//get hash value by XOR'ing 7 character substrings together 
		for (int i = 0; i < keySize; i += 7)
		{
			temp7 = key.substr(i, 7);
			iss.clear();
			iss.str(temp7);
			if (!(iss >> hex >> strHexToInt)) {fprintf (stderr, "iss >> hex >> strHexToInt -- DID NOT WORK!!!\n");} 
			hash = (hash ^ strHexToInt);
		}
		
		hash = hash % tableSize;
	}
	
	return hash;
}
//non-member functions-------------------------------------------------------------------------------------------------------------------

//constructor 
HashTable::HashTable(int table_size, string function, string collision)
{
	//resize and initalize to blank strings (NULL terminated)
	keys.resize(table_size, "");
	vals.resize(table_size, "");

	//numbers arbitrarily set 
	if (function == "Last7") {Fxn = 1;}
	if (function == "XOR") {Fxn = 2;}
	
	if (collision == "Linear") {Coll = 3;}
	if (collision == "Double") {Coll = 4;}
}

void HashTable::Add_Hash(string &key, string &val)
{
	int tableSize = keys.size(); //saves on calling .size() function repeatedly
	unsigned int hash = 0, h2 = 0;
	
	//LINEAR COLLISSION RESOLUTION----------------------------------------------------------------------------------
	if (Coll == 3)
	{
		//"Last7" hash function
		if (Fxn == 1) {hash = Last7(key, tableSize);}
		//"XOR" hash function 
		else if (Fxn == 2) {hash = XOR(key, tableSize);}
		else {printf("\n\nBAD FXN VALUE\n\n");}

		//initial check for empty element. This avoids performing the more intensive calcuation in the FOR un-necessarily 
		if (keys[hash] == "") {keys[hash] = key; vals[hash] = val; return;}
		
		//loop that inserts "keys" and "values" into the hash table if an empty element is encountered
		for (int i = 1; i < tableSize; i++)
		{
			if (keys[(hash + i) % tableSize] == "") {keys[(hash + i) % tableSize] = key; vals[(hash + i) % tableSize] = val; return;}
		}

		//If code reaches this line, we weren't able to insert "key" and "value" into hash table because it was full
		fprintf(stderr, "Hash Table Full\n");
	}
	
	//DOUBLE HASHING COLLISION RESOLUTION----------------------------------------------------------------------------------
	if (Coll == 4)
	{
		//"Last7" == 1st hash function 
		if (Fxn == 1) {hash = Last7(key, tableSize);}
		//"XOR" == 1st hash function 
		else if (Fxn == 2) {hash = XOR(key, tableSize);}
		else {printf("\n\nBAD FXN VALUE\n\n");}
		
		//initial check for empty element. This avoids prematurely calculating h2 and performing the more intensive FOR loop calcuations 
		if (keys[hash] == "") {keys[hash] = key; vals[hash] = val; return;}
		
		//"XOR" == 2nd hash function 
		if (Fxn == 1) {h2 = XOR(key, tableSize);}
		//"Last7" == 2nd hash function 
		if (Fxn == 2) {h2 = Last7(key, tableSize);}
		//Checks to make sure 2nd hash value isn't zero
		if (h2 == 0) {h2 = 1;} 
		
		//Loop starts at i == 1 since we already checked the case where i == 0 above 
		for (int i = 1; i < tableSize; i++) 
		{
			if (keys[(hash + i*h2) % tableSize] == "") {keys[(hash + i*h2) % tableSize] = key; vals[(hash + i*h2) % tableSize] = val; return;}
		}
		
		//If code reaches this FOR loop, then "key" and "val" were not able to be inserted. 
		for (int i = 0; i < tableSize; i++)
		{
			if (keys[i] == "") {fprintf(stderr, "Couldn't put %s into the table\n", key.c_str()); return;}
		}
		
		fprintf(stderr, "Hash Table Full\n"); return;
	}		
}

string HashTable::Find(string &key)
{
	int tableSize = keys.size();//saves on calling .size() function repeatedly
	unsigned int hash = 0, h2 = 0;
	
	tmp = 0;
	
	//LINEAR COLLISION RESOLUTION-----------------------------------------------------------------------------------------
	if (Coll == 3)
	{	
		//"Last7" hash function
		if (Fxn == 1) {hash = Last7(key, tableSize);}
		//"XOR" hash function 
		else if (Fxn == 2) {hash = XOR(key, tableSize);}
		else {printf("\n\nBAD FXN VALUE\n\n");}

		//initial check for empty element. This avoids performing the more intensive calcuation in the FOR un-necessarily 
		if (keys[hash] == key) {return vals[hash];}
		
		//Loop starts at i == 1 since we already checked the case where i == 0 above 
		for (int i = 1; i < tableSize; i++)
		{
			tmp++;
			if (keys[(hash + i) % tableSize] == key) {return vals[(hash + i) % tableSize];}
		}
	}
	
	//DOUBLE HASHING COLLISION RESOLUTION----------------------------------------------------------------------------------
	if (Coll == 4)
	{
		//"Last7" == 1st hash function 
		if (Fxn == 1) {hash = Last7(key, tableSize);}
		//"XOR" == 1st hash function 
		else if (Fxn == 2) {hash = XOR(key, tableSize);}
		else {printf("\n\nBAD FXN VALUE\n\n");}
		
		//initial check for empty element. This avoids performing the more intensive calcuation in the FOR un-necessarily 
		if (keys[hash] == key) {return vals[hash];}
		
		//"XOR" == 2nd hash function 
		if (Fxn == 1) {h2 = XOR(key, tableSize);}
		//"Last7" == 2nd hash function 
		if (Fxn == 2) {h2 = Last7(key, tableSize);}
		//Checks to make sure 2nd hash value isn't zero
		if (h2 == 0) {h2 = 1;} 
		
		//Loop starts at i == 1 since we already checked the case where i == 0 above 
		for (int i = 1; i < tableSize; i++)
		{
			tmp++;
			if (keys[(hash + i*h2) % tableSize] == key) {return vals[(hash + i*h2) % tableSize];}
		}
	}		

	return "";
}

void HashTable::Print()
{
	for (int i = 0; i < keys.size(); i++)
	{
		if (keys[i] != "") {printf("%5d %s %s\n",i, keys[i].c_str(), vals[i].c_str());}
	}
}
    
int HashTable::Total_Probes()
{
	int tmp2 = 0;
	string value;

	for (int i = 0; i < keys.size(); i++)
	{
		if (keys[i] != "") 
		{
			value = Find(keys[i]);
			tmp2 += tmp;
		}
	}

	return tmp2;
}
