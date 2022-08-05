//Tom Hills (jhills)
//CS 140 - lab3

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

typedef vector <int> IVec;

// Write your code here:

//TEST PRINT - DELETE FOR FINAL ------------------------------------------------------------------------------------------------------
void print2dVec (const vector<IVec> &p)
{
	for (int row = 0; row < p.size(); row++)
	{
		for (int column = 0; column < p[row].size(); column++)
		{
			if (column == (p[row].size() - 1)) {fprintf(stdout, "%4d\n", p[row][column]);}
			else {fprintf(stdout, "%4d", p[row][column]);}
		}
	}
}
//TEST PRINT - DELETE FOR FINAL ------------------------------------------------------------------------------------------------------

//writes pgm file (i.e., 2-d vector of pixels) to stdout
void pgm_write(const vector <IVec> &p)
{
	//"Rows" and "Columns" declared for better readability and to reduce function calls
	int Rows = p.size(), Columns = p[0].size(), count = 0;

	//prints "header" entries to pgm file
	fprintf(stdout, "P2\n%d %d\n255\n", Columns, Rows);

	for (int row = 0; row < Rows; row++)
	{
		//prints pixels 20 PER LINE!!!
		for (int column = 0; column < Columns; column++)
		{
			count++;
			
			if (count == 20) 
			{
				fprintf(stdout, "%4d\n", p[row][column]);
				count = 0;
			}
			else {fprintf(stdout, "%4d", p[row][column]);}
		}

		//prints final newline if not already done in "column" FOR loop
		if ((row == (Rows - 1)) && (count > 0) && (count < 20)) {fprintf(stdout, "\n");}
	}					
}

//return 2-d vector of pixels.
vector<IVec> pgm_create(int r, int c, int pv)
{
	vector<IVec> pgmFile (r, IVec(c, pv));
//	return vector<IVec> pgmFile (r, IVec(c, pv));
	return pgmFile;
}

//Adds "w" pixels around the border of vector "p" 
void pgm_pad(vector<IVec> &p, int w, int pv)
{
	//"Rows" and "Columns" declared for better readability and to reduce function calls
	int Rows = p.size(), Columns = p[0].size();
	
	//resizes 2-d vector, "p", to accomodate "w" pixel thick border around original 2-d vector. Each new element initialized to "pv". 
	for (int row = 0; row < Rows; row++) {p[row].resize(Columns + 2*w, pv);}
	p.resize(Rows + 2*w, IVec(Columns + 2*w, pv));
	
	//relocates original 2-d vector such that it is padded on each side by "w" pixels with "pv" values
	for (int row = (Rows - 1); row >= 0; row--)
	{
		for (int column = (p[row].size() - 1 - 2*w); column >= 0; column--)
		{
			if (row <= (w - 1)) 
			{
				p[row + w][column + w] = p[row][column];
				p[row][column] = pv;
			}
			else if ((column <= (w - 1)) && (row > (w - 1))) 
			{
				p[row + w][column + w] = p[row][column];
				p[row][column] = pv;
			}
			else {p[row + w][column + w] = p[row][column];}
		}
	}
}

//Resizes vector "p" to accomodate r*c copies of pixel table which are laid out in a "r" by "c" grid 
void pgm_panel(vector<IVec> &p, int r, int c)
{
	//declared for better readability and to reduce function calls
	int Rows = p.size();
	int Columns = p[0].size();

	//resizes 2-d vector, "p", based on "r" and "c" parameters. 
	for (int row = 0; row < Rows; row++) {p[row].resize(Columns * c, -1);}
	p.resize(Rows * r, IVec(Columns * c, -1));
	
	for (int row = 0; row < (Rows * r); row++)
	{
		if (row < Rows)
		{
			for (int column = Columns; column < (Columns * c); column++) 
			{
				p[row][column] = p[row][column % Columns];
			}
		}
		else 
		{
			for (int column = 0; column < (Columns * c); column++) 
			{
				p[row][column] = p[row % Rows][column % Columns];
			}
		}
	}
}

//ALWAYS pass vectors by reference, NOT by value. Declare separate 2-d vector.
void pgm_cw(vector<IVec> &p)
{
	int pRows = p.size(), pColumns = p[0].size();
	
	vector<IVec> cw (pColumns, IVec(pRows,-1));

//	cout << "p BEFORE loop:\n";
//	print2dVec(p);
//
//	cout << "\n\n";
//
//	cout << "cw BEFORE loop:\n";
//	print2dVec(cw);


	for (int row = 0; row < pRows; row++)
	{
		for (int column = 0; column < pColumns; column++)
		{
			cw[column][pRows - 1 - row] = p[row][column];
		}
	}
	
//	cout << "cw AFTER loop:\n";
//	print2dVec(cw);

	//FORGOT THIS!!!  CODE WAS RIGHT BUT I SCREWED UP BY NOT COPYING VECTOR BACK TO P!!!!!!!!!!!!!!!!!!!
	p = cw;
}

//ALWAYS pass vectors by reference, NOT by value. Declare separate 2-d vector.
void pgm_ccw(vector<IVec> &p)
{
	int pRows = p.size(), pColumns = p[0].size();
	
	vector<IVec> ccw (pColumns, IVec(pRows,-1));

//	cout << "p BEFORE loop:\n";
//	print2dVec(p);
//
//	cout << "\n\n";
//
//	cout << "ccw BEFORE loop:\n";
//	print2dVec(ccw);

	for (int row = 0; row < pRows; row++)
	{
		for (int column = 0; column < pColumns; column++)
		{
			ccw[pColumns - 1 - column][row] = p[row][column];
		}
	}
	
//	cout << "ccw AFTER loop:\n";
//	print2dVec(ccw);
	
	//FORGOT THIS!!!  CODE WAS RIGHT BUT I SCREWED UP BY NOT COPYING VECTOR BACK TO P!!!!!!!!!!!!!!!!!!!
	p = ccw;
}

//ALWAYS pass vectors by reference. CANNOT use separate vector of any kind.
void pgm_crop(vector<IVec> &p, int r, int c, int rows, int cols)
{
	int row, column;	
	
	//loop to relocate crop selection such that top-right corner now resides at beginning of 2-d vector...
	//...(i..e, vector[0][0])
	for (row = 0; row < rows; row++)
	{
		for (column = 0; column < cols; column++)
		{
			p[row][column] = p[row + r][column + c];
		}
	}

	//resizes 2-d vector to crop proportions
	p.resize(rows);
	for (row = 0; row < rows; row++) {p[row].resize(cols);}

}

////TEST MAIN (): DELETE THIS///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int main(int argc, char **argv)
//{
//
//	vector <IVec> p(10);
//	int row = 0, column = 0, readCount = 0, tempInt;
//	stringstream ss;
//	string str;
//	int count = 0;
//
////    vector<vector<int> > p (10, vector<int> (11, 235));
//
//	for (int i = 0; i < 10; i++)
//	{
//		for (int j = 0; j < 5; j++)
//		{
//			count++;
//			p[i].push_back(count);
//		}
//	}
//
//	pgm_cw(p);
//
////	cout << "\n\nNEXT\n\n";
////
////	pgm_ccw(p);
//
////	cout << "p BEFORE crop:\n";
////	print2dVec(p);
////
////	cout <<"\n\np AFTER crop:\n";
////
////	pgm_crop(p,2, 1, 3,3);
////
////	print2dVec(p);
//	
//	//reading in and storing pixels from file
////	while (getline(cin, str))
////	{
//////		//TEST
//////		cout << "str:\n" << str << "\n\n";
////		
////		//loop to bypass any "lone" newlines (b/c newline terminates on \n, but leaves it in stream) found by getline
////		while (str == "\n") {cout <<"str == newline\n"; getline(cin, str);}//NOTE TO SELF: With pgm_write.pgm, I never enter this loop. I think newline is getting discarded on last column read.	
////		//This line is specific to "pgm_write.pgm". Other pgm files might not need this check.
////		if (str == "") {cout << "detected blank string\n"; break;}
////
////		ss.clear();
////		ss.str(str);
////
////		column = 0;
////		pgmFile.resize(row + 1);
////		
////		//streams ints from ss into vector and error checks and loop tracks
////		while (1) 
////		{
////			ss >> tempInt;
////		
//////			//TEST
//////			printf("tempInt: %d\treadCount: %d\tcolumn: %d", tempInt, readCount, column);
////
////			if (ss.fail() && !ss.eof()) {fprintf(stderr,"Failed during read %d before end of file.");}
////			pgmFile[row].push_back(tempInt);
////		
//////			//TEST
//////			cout << "\tpgmFile[" << row << "][" << column <<"]: " << pgmFile[row][column] << "\n";
////
////			readCount++;
////			column++;
////
////			if (ss.eof()) {break;}
////		}
////	
//////		//TEST
//////		cout << "\n";
////		
////		cin.clear();//this needs to be here at the end
////		row++;//this needs to be here b/c row = 0 initially
////	}
//
//	
//
//	return 0;
//}
//
////TEST MAIN (): DELETE THIS///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





// DO NOT CHANGE ANYTHING BELOW THIS LINE

void bad_pgm(string s)
{
  cerr << "Bad PGM file: " << s << endl;
  exit(1);

}

vector <IVec> pgm_read()
{
  string s;
  int r, c, i, j, v;
  vector <IVec> p;

  if (!(cin >> s)) bad_pgm("Empty file.");
  if (s != "P2") bad_pgm("First word is not P2.");
  if (!(cin >> c) || c <= 0) bad_pgm("Bad column spec.");
  if (!(cin >> r) || r <= 0) bad_pgm("Bad column spec.");
  if (!(cin >> i) || i != 255) bad_pgm("Bad spec of 255.");
  p.resize(r);
  for (i = 0; i < r; i++) for (j = 0; j < c; j++) {
    if (!(cin >> v) || v < 0 || v > 255) bad_pgm("Bad pixel.");
    p[i].push_back(v);
  }
  if (cin >> s) bad_pgm("Extra stuff at the end of the file.");
  return p;
}

void usage()
{
  cerr << "usage: pgm_editor command....\n\n";
  cerr << "        CREATE rows cols pixvalue\n";
  cerr << "        CW\n";
  cerr << "        CCW\n";
  cerr << "        PAD pixels pixvalue\n";
  cerr << "        PANEL r c\n";
  cerr << "        CROP r c rows cols\n";
  exit(1);
}

main(int argc, char **argv)
{
  istringstream ss;
  int r, c, i, j, p, w, rows, cols;
  vector <IVec> pgmf;
  string a1;

  if (argc < 2) usage();
  a1 = argv[1];

  if (a1 == "CREATE") {
    if (argc != 5) usage();
    ss.clear(); ss.str(argv[2]); if (!(ss >> r) || r <= 0) usage();
    ss.clear(); ss.str(argv[3]); if (!(ss >> c) || c <= 0) usage();
    ss.clear(); ss.str(argv[4]); if (!(ss >> p) || p < 0 || p > 255) usage();
    pgmf = pgm_create(r, c, p);
  } else if (a1 == "PAD") {
    if (argc != 4) usage();
    ss.clear(); ss.str(argv[2]); if (!(ss >> w) || w <= 0) usage();
    ss.clear(); ss.str(argv[3]); if (!(ss >> p) || p < 0 || p > 255) usage();
    pgmf = pgm_read();
    pgm_pad(pgmf, w, p);
  } else if (a1 == "CCW") {
    if (argc != 2) usage();
    pgmf = pgm_read();
    pgm_ccw(pgmf);
  } else if (a1 == "CW") {
    if (argc != 2) usage();
    pgmf = pgm_read();
    pgm_cw(pgmf);
  } else if (a1 == "PANEL") {
    if (argc != 4) usage();
    ss.clear(); ss.str(argv[2]); if (!(ss >> r) || r <= 0) usage();
    ss.clear(); ss.str(argv[3]); if (!(ss >> c) || c <= 0) usage();
    pgmf = pgm_read();
    pgm_panel(pgmf, r, c);
  } else if (a1 == "CROP") {
    if (argc != 6) usage();
    ss.clear(); ss.str(argv[2]); if (!(ss >> r) || r < 0) usage();
    ss.clear(); ss.str(argv[3]); if (!(ss >> c) || c < 0) usage();
    ss.clear(); ss.str(argv[4]); if (!(ss >> rows) || rows <= 0) usage();
    ss.clear(); ss.str(argv[5]); if (!(ss >> cols) || cols <= 0) usage();
    pgmf = pgm_read();
    if (r + rows > pgmf.size() || c + cols > pgmf[0].size()) {
      fprintf(stderr, "CROP - Bad params for the pictures size (r=%d, c=%d)\n",
           (int) pgmf.size(), (int) pgmf[0].size());
      exit(1);
    }
    pgm_crop(pgmf, r, c, rows, cols);
  } else {
    usage();
  }
  pgm_write(pgmf);
}
