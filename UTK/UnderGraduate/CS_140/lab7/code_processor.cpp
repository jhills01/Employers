//Tom Hills (jhills)
//Lab 7 - Code Processing (complete)

#include <set>
#include <deque>
#include <map>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "code_processor.h"

using namespace std;

//creates a new Prize object and stores it in "Prizes" member variable 
int Code_Processor::New_Prize(string id, string description, int points, int quantity)
{
	if ( (this->Prizes.find(id) != Prizes.end()) || (points <= 0) || (quantity <= 0) ) {/*ERROR CHECK*/ return -1;}
	else 
	{		
		//creates new Prize object on heap and initializes with corresponding parameters
		Prize * newPrize = new Prize;
		newPrize->id = id;
		newPrize->description = description;
		newPrize->points = points;
		newPrize->quantity = quantity;

		//adds new Prize object to "Prizes" map member, keyed on "id" parameter
		this->Prizes.insert(pair<string, Prize *> (id, newPrize));
		
		return 0;
	}
}

//Creates a new User object and stores it in "Names" member variable
int Code_Processor::New_User(string username, string realname, int starting_points)
{
	if ( (this->Names.find(username) != Names.end()) || (starting_points < 0) ) {/*ERROR CHECK*/ return -1;}
	else 
	{		
		//creates new User object on heap and initializes with corresponding parameters
		User * newUser = new User;
		newUser->username = username;
		newUser->realname = realname;
		newUser->points = starting_points;

		//adds new User object to "Names" map member, keyed on "username" parameter
		this->Names.insert(pair<string, User *> (username, newUser));
		
		return 0;
	}
}

//returns the value of "points" within User object if "username" exists
int Code_Processor::Balance(string username)
{
	if (this->Names.find(username) == this->Names.end()) {return -1;}
	else {return this->Names[username]->points;}
}

//registers "phone" in "phone_numbers" and "Phones" member variable
int Code_Processor::Add_Phone(string username, string phone)
{
	//checks whether "username" does NOT exist || "phone" is already registered with another User
	if ( (this->Names.find(username) == this->Names.end()) || (this->Phones.find(phone) != this->Phones.end()) ) {return -1;}
	
	//registers "phone"
	else 
	{
		this->Phones.insert(pair<string, User *> (phone, this->Names[username]));
		this->Names[username]->phone_numbers.insert(phone);
		
		return 0;
	}
}

//removes "phone" from "phone_numbers" and "Phones" member variable
int Code_Processor::Remove_Phone(string username, string phone)
{
	//checks whether "username" does NOT exist || "phone" does NOT exist || "phone" is registered with another User
	if ( (this->Names.find(username) == this->Names.end()) || (this->Phones.find(phone) == this->Phones.end()) || 
		 (this->Phones[phone]->username != username) ) {return -1;}
	
	//removes "phone" 
	else 
	{
		this->Phones[phone]->phone_numbers.erase(phone);
		this->Phones.erase(phone);
		
		return 0;
	}
}

//builds and returns a string consisting of a User's "phone_numbers" in lexicographical order
string Code_Processor::Show_Phones(string username)
{
	//checks whether User exists
	if (this->Names.find(username) == this->Names.end()) {return "BAD USER";}
	
	else 
	{
		//return EMPTY string if there are no phone numbers associated with User
		if (this->Names[username]->phone_numbers.empty()) {return "";}

		string phoneNumbers;
		set<string>::iterator pnit;
		
		//builds a string of User phone numbers in lexicographical order
		for (pnit = this->Names[username]->phone_numbers.begin(); pnit != this->Names[username]->phone_numbers.end(); pnit++)
		{
			phoneNumbers += *pnit + "\n";
		}
		
		return phoneNumbers;
	}
}

//saves the server state by writing "PRIZE", "ADD_USER", "ADD_PHONE", & "MARKED_USED" keyword lines from input files
int Code_Processor::Write(const char *file)
{	
	ofstream outFile;

	//if file can't be created or opened, return -1
	outFile.open(file);
	if (outFile.fail()) {return -1;}
	
	//"PRIZE" lines to output "file"
	map<string, Prize *>::iterator pit;
	for (pit = Prizes.begin(); pit != Prizes.end(); pit++)
	{
		outFile << "PRIZE " << pit->second->id << " " << pit->second->points << " ";
		outFile << pit->second->quantity << " " << pit->second->description << "\n";
	}
	
	//"ADD_USER" lines to output "file"
	map<string, User *>::iterator uit;
	for (uit = Names.begin(); uit != Names.end(); uit++)
	{
		outFile << "ADD_USER " << uit->second->username << " " << uit->second->points << " ";
		outFile << uit->second->realname << "\n";
	}
	
	//"ADD_PHONE" lines to output "file" using same iterator from "ADD_USER"
	set<string>::iterator pnit;
	for (uit = Phones.begin(); uit != Phones.end(); uit++)
	{
		for (pnit = uit->second->phone_numbers.begin(); pnit != uit->second->phone_numbers.end(); pnit++)
		{
			outFile << "ADD_PHONE " << uit->second->username << " " << *pnit << "\n";
		}
	}

	//"MARK_USED" lines to output "file" using same set iterator from "ADD_PHONE"
	int count = 0;
	for (pnit = this->Codes.begin(); pnit != this->Codes.end(); pnit++)
	{
		outFile << "MARK_USED " << *pnit << "\n";		
	}
	
	outFile.close();

	return 0;
}

//hash function to convert "codes" into "points" 
unsigned int djbHash(string &s)
{
  int i;
  unsigned int h;
  
  h = 5381;

  for (i = 0; i < s.size(); i++) 
  {
    h = (h << 5) + h + s[i];
  }
  
  return h;
}


//Erases User from "Names" member and all associated phone numbers in "Phones" member
int Code_Processor::Delete_User(string username)
{
	if (this->Names.find(username) == this->Names.end()) {return -1;}

	else
	{
		User * tempUser = this->Names.find(username)->second;
		
		//erase "username" from "Names" member
		this->Names.erase(this->Names.find(username));

		//erase phone numbers associated with "username" from "Phones" member
		//NOTE TO GRADER: The double iterator method was written to be compliant with c++98
		map <string, User *>::iterator msuIt, msuIt_TEMP;
		for (msuIt = this->Phones.begin(); msuIt != this->Phones.end();)
		{
			if (msuIt->second->username == username) {msuIt_TEMP = msuIt; msuIt++; this->Phones.erase(msuIt_TEMP);}
			else {msuIt++;}
		}
		
		//deallocate User memory
		delete tempUser;

		return 0;
	}
}

//Checks and inserts "code" into "Codes" member if valid using User's "username" 
int Code_Processor::Enter_Code(string username, string code)
{
	unsigned int hashVal = 0;
	int points = 0;

	//checks if "code" was previously entered OR if the User does not exist
	if ( (this->Codes.find(code) != this->Codes.end()) || (this->Names.find(username) == this->Names.end()) ) {return -1;}

	//determines "code" validity (must be divisible by 17 or 13) and inserts/awards points accordingly.
	else
	{
		hashVal = djbHash(code);

		if (hashVal % 17 == 0) 
		{
			points = 10; 
			this->Names.find(username)->second->points += points; 
			this->Codes.insert(code);
			return points;
		}
		else if (hashVal % 13 == 0) 
		{
			points = 3; 
			this->Names.find(username)->second->points += points; 
			this->Codes.insert(code);
			return points;
		}
		else {/*Code is invalid*/ return 0;}
	}
}

//Checks and inserts "code" into "Codes" member if valid using User's "phone"
int Code_Processor::Text_Code(string phone, string code)
{
	unsigned int hashVal = 0;
	int points = 0;

	//checks if "code" was previously entered OR if "phone" does not exist
	if ( (this->Codes.find(code) != this->Codes.end()) || (this->Phones.find(phone) == this->Phones.end()) ) {return -1;}

	//determines "code" validity (must be divisible by 17 or 13) and inserts/awards points accordingly.
	else
	{
		hashVal = djbHash(code);

		if (hashVal % 17 == 0) 
		{
			points = 10; 
			this->Phones.find(phone)->second->points += points; 
			this->Codes.insert(code);
			return points;
		}
		else if (hashVal % 13 == 0) 
		{
			points = 3; 
			this->Phones.find(phone)->second->points += points; 
			this->Codes.insert(code);
			return points;
		}
		else {/*Code is invalid*/ return 0;}
	}
}

//marks provided "code" as used to help rebuild server state   
int Code_Processor::Mark_Code_Used(string code)
{
	unsigned int hashVal = djbHash(code);

	//checks "code" and inserts it if valid
	if ( ((hashVal % 17) == 0) && this->Codes.insert(code).second ) {return 0;}
	if ( ((hashVal % 13) == 0) && this->Codes.insert(code).second ) {return 0;}
	return -1;
}

//redeems "prize" for User provided the "prize" exists and User has enough points
int Code_Processor::Redeem_Prize(string username, string prize)
{
	//checks if "username" and "prize" are valid
	if ( (this->Prizes.find(prize) == this->Prizes.end()) || (this->Names.find(username) == this->Names.end()) ) {return -1;}
	
	//checks if User has enough points for the "prize"
	if (this->Prizes.find(prize)->second->points > this->Names.find(username)->second->points) {return -1;}

	//decrements the points of the "prize" from the User's points
	this->Names.find(username)->second->points -= this->Prizes.find(prize)->second->points;

	//decreases the "quantity" of the "prize" by 1
	this->Prizes.find(prize)->second->quantity--;

	//if "prize" "quantity" is reduced to 0, remove from system
	if (this->Prizes.find(prize)->second->quantity == 0) {delete this->Prizes.find(prize)->second; this->Prizes.erase(prize);}

	return 0;
}

//deletes all heap allocated memory associated with User and Prize objects
Code_Processor::~Code_Processor()
{
	map <string, User *>::iterator msuIt;
	map <string, Prize *>:: iterator mspIt;

	for (msuIt = this->Names.begin(); msuIt != this->Names.end(); msuIt++) {delete msuIt->second;}
	for (mspIt = this->Prizes.begin(); mspIt != this->Prizes.end(); mspIt++) {delete mspIt->second;}
}
