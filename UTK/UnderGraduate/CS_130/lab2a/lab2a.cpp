//John Hills
//Lab2a, WK2, MUDs

#include <iostream>
#include <fstream> 
#include <string>

using namespace std;

struct Room
{
	string title, description;
	int n, e, s, w;
};

int main (int argc, char*argv[])
{
	if (argc != 2)
	{
	        printf("argc != 2. Please re-enter correct number of command line arguments.");	
		return -1;
	}

        ifstream infile;
		
	//opens the filename provided in the command line.
	infile.open(argv[1]);

	if (infile.is_open() != true)
	{
		printf("file %s will not open", argv[1]);
		return -1;
	}

	string line;
	int roomCounter = 0;	
	int lineCounter = 0;
	
	//Gets line and room counts from text file. Results are then used to dynamically allocate required memory. 
	while (getline(infile, line))
	{
		lineCounter++;
		if (static_cast<int>(line.find("Room #"))!= -1) {roomCounter++;}
	}

	//dymanic allocation
	Room *rooms = new Room[roomCounter];
	//Had to add "+ 1" to string[lineCounter] because I kept getting a segmentation fault after "while(getline(infile, roomsFile[i]))" loop. 
	string *roomsFile = new string [lineCounter + 1];

	//Prepares the file to read again into "roomsFile"
	infile.close();
        infile.open(argv[1]);	

	int i = 0;
	line = "";

	//Copies entire room file contents into roomsFile array line by line.
	//I tried coping entire infile to one string, but when I printf'ed the string I was missing characters on the end. I must have reached a character limit. Had to change to string array..
	while(getline(infile, roomsFile[i]))
	{
		i++;
	}

	infile.close();

	int roomNum = 0;
	int tilda = 0;

	//populating each Room object member variables with the appropriate information from rooms file. 
	for (int i = 0; i < (lineCounter); i++)
	{
		if (static_cast<int>(roomsFile[i].find("~")) != -1) {tilda++;}
		if (static_cast<int>(roomsFile[i].find("Room #")) != -1 && roomNum <= roomCounter) {rooms[roomNum].title = roomsFile[i];} 
		if (tilda == 1 && static_cast<int>(roomsFile[i].find("~")) == -1) {rooms[roomNum].description += " " + roomsFile[i];}
		if (tilda == 2 && static_cast<int>(roomsFile[i].find("n")) != -1) {rooms[roomNum].n = stoi(roomsFile[i].erase(0,2));}
		if (tilda == 2 && static_cast<int>(roomsFile[i].find("s")) != -1) {rooms[roomNum].s = stoi(roomsFile[i].erase(0,2));}
		if (tilda == 2 && static_cast<int>(roomsFile[i].find("e")) != -1) {rooms[roomNum].e = stoi(roomsFile[i].erase(0,2));}
		if (tilda == 2 && static_cast<int>(roomsFile[i].find("w")) != -1) {rooms[roomNum].w = stoi(roomsFile[i].erase(0,2));}
		if (tilda == 3) {roomNum++; tilda = 0;}				
	}	
	
	char op, previousOp;
	int currentRoomNum = 0;
	int previousRoomNum = 1;
	bool north, south, east, west;
	int northToRoom0 = -1, southToRoom0 = -1, eastToRoom0 = -1, westToRoom0 = -1;
	bool firstTimeLeavingRoom0_n = true, firstTimeLeavingRoom0_s = true, firstTimeLeavingRoom0_e = true, firstTimeLeavingRoom0_w = true; 

	while (1)
	{
		// Since the rooms after Room 0 have 0 in the n, s, e, & w members variables, Room 0 exit wasn't printing out during the next if statement block. Added this to fix the problem. 
		if (previousRoomNum == 0 && previousOp == 'n' && firstTimeLeavingRoom0_n == true) {southToRoom0 = currentRoomNum; firstTimeLeavingRoom0_n = false;}
		if (previousRoomNum == 0 && previousOp == 's' && firstTimeLeavingRoom0_s == true) {northToRoom0 = currentRoomNum; firstTimeLeavingRoom0_s = false;}
		if (previousRoomNum == 0 && previousOp == 'e' && firstTimeLeavingRoom0_e == true) {westToRoom0 = currentRoomNum; firstTimeLeavingRoom0_e = false;}
		if (previousRoomNum == 0 && previousOp == 'w' && firstTimeLeavingRoom0_w == true) {eastToRoom0 = currentRoomNum; firstTimeLeavingRoom0_w = false;}

		north = false, south = false, east = false, west = false;
		
		//again, first room having index 0 made Room 0 unavailable in rooms just past Room 0, i.e., you couldn't return to start after leaving. The if block above fixed that. 
		printf("%s\n%s\n\nExits: ", rooms[currentRoomNum].title.c_str(), rooms[currentRoomNum].description.c_str());
		if (rooms[currentRoomNum].n != 0 || currentRoomNum == northToRoom0) {printf("n "); north = true;}
		if (rooms[currentRoomNum].s != 0 || currentRoomNum == southToRoom0) {printf("s "); south = true;} 
		if (rooms[currentRoomNum].e != 0 || currentRoomNum == eastToRoom0) {printf("e "); east = true;}
		if (rooms[currentRoomNum].w != 0 || currentRoomNum == westToRoom0) {printf("w "); west = true;}

		redo:

		printf("\n\nPlease input 'l' for look, 'n' for north, 'e' for east, 'w' for west, 's' for south, or 'q' to quit.\n>");		
		scanf(" %c", &op);

		if (op == 'n' && north  == false) {printf("\nYou can't go NORTH!"); goto redo;}
		if (op == 's' && south  == false) {printf("\nYou can't go SOUTH!"); goto redo;}
		if (op == 'e' && east  == false) {printf("\nYou can't go EAST!"); goto redo;}
		if (op == 'w' && west  == false) {printf("\nYou can't go WEST!"); goto redo;}
		

		switch (op)
		{
		    case 'l':
			    break;
		    case 'n':
			    printf("\nYou moved NORTH!");
			    previousOp = 'n';
			    previousRoomNum = currentRoomNum;
			    currentRoomNum = rooms[currentRoomNum].n; 
		            goto redo;
			    break;
		    case 'e':
			    printf("\nYou moved EAST!");
			    previousOp = 'e';
			    previousRoomNum = currentRoomNum;
			    currentRoomNum = rooms[currentRoomNum].e; 
		            goto redo;
			    break;
		    case 'w':
			    printf("\nYou moved WEST!");
			    previousOp = 'w';
			    previousRoomNum = currentRoomNum;
			    currentRoomNum = rooms[currentRoomNum].w; 
		            goto redo;
			    break;
		    case 's':
			    printf("\nYou moved SOUTH!");
			    previousOp = 's';
			    previousRoomNum = currentRoomNum;
			    currentRoomNum = rooms[currentRoomNum].s; 
		            goto redo;
			    break;
		    case 'q':
			    delete [] rooms;
			    delete [] roomsFile;
			    return 0;
		    default:
			    printf("\nERROR: The character entered is not recognized. Try again!");
			    goto redo;
			    break;
		}
	
	}

	//Safety net. 	
	delete [] rooms;
	delete [] roomsFile;
	
	return 0;
}
