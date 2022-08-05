#include <map>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>//NOTE: might not use this

using namespace std;

//STRUCTS--------------------------------------------------------------------------------------------------------------------

struct Song { 
    string title;
    int time;  // could also be a string
    int track;
	
	Song():time(0){}//initializer list
};

struct Album {
    //   int == track # b/c track # is more distinct than song time.
	map <int, Song > songs;
    string name;
    int time;
    int nsongs;  // optional variable but makes it easier
	
	Album():time(0), nsongs(0) {}//initializer list
};

struct Artist {
    map <string, Album > albums;
    string name;
    int time;//this should be total time of all albums in seconds
    int nsongs;
	
	Artist():time(0), nsongs(0) {}//initializer list
};

//NON-MEMBER FUNCTIONS-------------------------------------------------------------------------------------------------------

//removes underscores from provided string
void removeUnderscores (string &str)
{
	for (unsigned int i = 0; i < str.size(); i++) 
	{
		if (str[i] == '_') {str[i] = ' ';}
	}
}


//converts "time" (format "mm:ss") to total number of seconds (int)
int stringToInt_time (string &time)
{
	//NOTE: can't use stoi b/c it's -std=c++11 exclusive.
//	int min = stoi(time.substr(0, time.find(":")), NULL, 10);
//	int sec = stoi(time.substr(time.find(":") + 1, time.size() - 1), NULL, 10);
	
	int min, sec;

	sscanf(time.c_str(), "%d:%d", &min, &sec);
	
	return (sec + min*60);
}

//converts "seconds" into a string of format "mm:ss"
string intToString_time (int sec)
{
	char buff[20];
	string str;
	int min = sec/60;
	sec = sec%60;

	sprintf(buff,"%d:%02d", min, sec);
	str = buff;

	return str;
}

void printArtistsAlbumsTracks (map <string, Artist> &mStrArtist)
{
	//this should print all albums and tracks for a SINGLE artist
	//format is as follows:
//	Coltrane, John: 4, 18:37
//			Giant Steps: 4, 18:37
//					1. Giant Steps: 4:46
//					3. Countdown: 2:25
//					6. Naima: 4:24
//					7. Mr. P.C.: 7:02

	map <string, Artist>::iterator  mStrArtistIt;
	map <string, Album>::iterator  mStrAlbumIt;
	map <int, Song>::iterator  mIntSongIt;

	for (mStrArtistIt = mStrArtist.begin(); mStrArtistIt != mStrArtist.end(); ++mStrArtistIt)
	{
		printf("%s: %d, %s\n", mStrArtistIt->second.name.c_str(), 
				mStrArtistIt->second.nsongs, intToString_time(mStrArtistIt->second.time).c_str());
		
		for (mStrAlbumIt = mStrArtistIt->second.albums.begin(); mStrAlbumIt != mStrArtistIt->second.albums.end(); ++mStrAlbumIt)
		{
			printf("        %s: %d, %s\n", mStrAlbumIt->second.name.c_str(), 
					mStrAlbumIt->second.nsongs, intToString_time(mStrAlbumIt->second.time).c_str());

			for (mIntSongIt = mStrAlbumIt->second.songs.begin(); mIntSongIt != mStrAlbumIt->second.songs.end(); ++mIntSongIt)
			{
				printf("                %d. %s: %s\n", mIntSongIt->first, mIntSongIt->second.title.c_str(), 
						intToString_time(mIntSongIt->second.time).c_str());
			}
		}
	}
}

int main(int argc, char *argv[])
{
	//OLD ERROR CHECKS
//			printf("BEFORE REMOVE --- songTitle: %s\n", songTitle.c_str());
//			printf("AFTER REMOVE --- songTitle: %s\n", songTitle.c_str());
//			printf("sSong.title: %s\n", sSong.title.c_str());
//			printf("sSong.time == %d. Should be 0.\n", sSong.time);
//			printf("sSong.time == %d. Should be 145.\n", sSong.time);
//			printf("sSong.time == %d. Should be 145.\n", sSong.time);
//			printf("intToString_time(sSong.time) == %s. Should be 2:25.\n", intToString_time(sSong.time).c_str());
	
	
	if (argc != 2) {printf("ERROR: lib_info file");}
	
	Song sSong;
	Album sAlbum;
	Artist sArtist;
	ifstream fs;//NOTE: Had to change from "fstream" to "ifstream" type to get the file path to open correctly!!!
	string songTitle, songTime, artist, album, genre;
	int track;
	map <string, Artist> mStrArtist;
	map <string, Artist>::iterator  mStrArtistIt;
	map <string, Album>::iterator  mStrAlbumIt;
	pair <map <string, Artist>::iterator, bool> pStrArtistBool;
	pair <map <string, Album>::iterator, bool> pStrAlbumBool;

	fs.open(argv[1]);

//	int x = 1;
	
	if (fs.is_open())
	{
		//NOTE: multiple lines from fstream have not been tested yet for while loop condition
		while (fs >> songTitle >> songTime >> artist >> album >> genre >> track)
		{
			
//			cout << x << endl;

//			//NOTE: does not work
//			//clear all struct maps 
//			sArtist.albums.clear();
			sAlbum.songs.clear();
			
			//WORKS
			removeUnderscores(songTitle);
			removeUnderscores(artist);
			removeUnderscores(album);
//			removeUnderscores(genre);//NOTE: Doesn't look like I need "genre" for final print. It's junk.
			
			//WORKS
			//NOTE: I'm assuming that each song from a line of input will be put somewhere in our structures. Hence me automatically creating object.
			//IMPORTANT NOTE: I got confused thinking about how to insert into an existing map. I HAVE TO CREATE THE OBJECT FIRST BEFORE INSERTING!!!
			//build new Song
			sSong.title = songTitle;
			//NOTE: It's looking like songTime is the only place in code I need to use "stringToInt_time" function. If so, think about...
			//...placing the function code here instead since it's just a few lines. However, this might require two new variables....
			//...Can I get away with updating sSong.time directly? Don't think so. Maybe just keep it the way it is. Think on it.
			sSong.time = stringToInt_time(songTime);//stringtoInt_time returns total number of seconds
			sSong.track = track;

			sAlbum.songs.clear();
			sAlbum.songs.insert(make_pair(track, sSong));
			sAlbum.name = album;
			sAlbum.time = sSong.time;
			sAlbum.nsongs = 1;
			
			//NOTE: using INSERT instead of FIND b/c INSERT still has to perform same instructions as FIND initially with the added....
			//...beneifit of inserting the new Artist if not already there 
			//NOTE: Look for repeating code in the two branches to see if we can compact the codde more
			pStrArtistBool = mStrArtist.insert(make_pair(artist, sArtist));
			mStrArtistIt = pStrArtistBool.first;

//			cout << "Artist: " << mStrArtistIt->first << endl;

			if (pStrArtistBool.second)//artist was NOT found in mStrArtist
			{
//				cout << "New Artist/Album" << endl;
				
//				//build new Album object to be inserted into "mStrArtistIt->second", the new Artist object in mStrArtist 
//				sAlbum.songs.clear();
//				sAlbum.songs.insert(make_pair(track, sSong));
//				sAlbum.name = album;
//				sAlbum.time = sSong.time;
//				sAlbum.nsongs = 1;
				
				//update newly inserted Artist in mStrArtist
				//NOTE: don't think I need to do "sArtist.albums.clear()" b/c I only insert blank Artist object and then update copy in mStrArtist map
				//....I think. In other words, sArtist is a blank object inserted into mStrArtist map. This map should now contain a blank Artist...
				//....which I update directly using mStrArtistIt->second to access the map copy. The Artist object inside mStrArtist map should NOT...
				//... update the local sArtist in any way. It should only be a copy.
				mStrArtistIt->second.albums.insert(make_pair(album, sAlbum));
				mStrArtistIt->second.name = artist;
				mStrArtistIt->second.time = sSong.time;
				mStrArtistIt->second.nsongs = 1;
			}
			
			else//artist was found in mStrArtist
			{
				//update existing Artist and Album
//				mStrArtistIt = pStrArtistBool.first;//assigning to more reader-friendly variable
//				NOTE: I think problem lies here b/c I'm inserting sAlbum from OLD read. I believe I need to update album with correct line info...
//				...before trying to insert here. 
				pStrAlbumBool = mStrArtistIt->second.albums.insert(make_pair(album, sAlbum));
				mStrAlbumIt = pStrAlbumBool.first;

				
//				cout << "Album within Artist: " << mStrAlbumIt->first << endl;

				
				if (pStrAlbumBool.second)//album was NOT already in Artist
				{
//					cout << "Existing Artist/ New Album" << endl;

//					//build new Album object to be inserted into "mStrArtistIt->second", the new Artist object in mStrArtist 
//					sAlbum.songs.clear();
//					sAlbum.songs.insert(make_pair(track, sSong));
//					sAlbum.name = album;
//					sAlbum.time = sSong.time;
//					sAlbum.nsongs = 1;

					//update existing Artist
					mStrArtistIt->second.albums.insert(make_pair(album, sAlbum));
					mStrArtistIt->second.time += sAlbum.time;
					mStrArtistIt->second.nsongs += 1;
				}

				else//album was already in map 
				{
//					cout << "Existing Artist/ Existing Album" << endl;
					
					//update existing Album
					mStrAlbumIt->second.songs.insert(make_pair(track, sSong));
					mStrAlbumIt->second.time += sSong.time;
					mStrAlbumIt->second.nsongs += 1;

					//update existing Artist
					mStrArtistIt->second.time += sSong.time;
					mStrArtistIt->second.nsongs += 1;
				}
			}
	
//			cout << "\n";
//			++x;

		}
	}

	else {printf("ERROR: file didn't open successfully");}

	fs.close();
	
	//final print
	printArtistsAlbumsTracks (mStrArtist);

	return 0;
}
