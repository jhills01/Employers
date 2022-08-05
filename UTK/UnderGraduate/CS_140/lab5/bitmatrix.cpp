//Tom Hills (jhills)
//lab 5 - Bit-Matrices

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <algorithm> 
#include "bitmatrix.h"

//non-member functions-------------------------------------------------------------------------------------------
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
//non-member functions-------------------------------------------------------------------------------------------

Bitmatrix::Bitmatrix(int rows, int cols)
{
	if ((rows <= 0) || (cols <= 0))
	{
		fprintf(stderr, "Either rows or cols <= 0 in Bitmatrix(int rows, int cols).");
		M.resize(1);
		M[0] = '0';
	}
	
	else 
	{
		//declaring/initializing string here to avoid un-necessarily processing if "row" or "cols" is bad data.
		string str(cols, '0');
		
		M.resize(rows);
		for (int i = 0; i < rows; i++) {M[i] = str;}
	}
}

int Bitmatrix::Rows()
{
	return M.size();	
}

int Bitmatrix::Cols()
{
	return M[0].size();	
}

void Bitmatrix::Set(int row, int col, char val)
{
	if ((row < 0) || (col < 0)) {return;}
	//converts integers 0 and 1 to chars '0' and '1'
	if ((val == 0) || (val == 1)) {val += '0';}
  
	M[row][col] = val;	
}
  
char Bitmatrix::Val(int row, int col)
{
	if ((row < 0) || (col < 0)) {return -1;}
	//returns char element converted to integer
	return (M[row][col] - '0');
}
  
void Bitmatrix::Print(int w) 
{
  int i, j;

  for (i = 0; i < M.size(); i++) {
    if (w > 0 && i != 0 && i%w == 0) printf("\n");
    if (w <= 0) {
      cout << M[i] << endl;
    } else {
      for (j = 0; j < M[i].size(); j++) {
        if (j % w == 0 && j != 0) printf(" ");
        printf("%c", M[i][j]);
      }
      cout << endl;
    }
  }
}

void Bitmatrix::Write(string fn) 
{
	ofstream outF;

	outF.open(fn.c_str());
	if (outF.fail()) {perror(fn.c_str()); return;}

	for (int row = 0; row < M.size(); row++)
	{
		for (int col = 0; col < M[row].size(); col++)
		{
			if (col == (M[row].size() - 1)) {outF << (M[row][col]) << "\n";}
			else {outF << (M[row][col]);}
		}
	}

	outF.close();
}

void Bitmatrix::Swap_Rows(int r1, int r2)
{
	if ((r1 < 0) || (r2 < 0)) {return;}

	string strTmp;
	
	//Swaps rows r1 and r2 within vector member M
	strTmp = M[r1];
	M[r1] = M[r2];
	M[r2] = strTmp;
}

void Bitmatrix::R1_Plus_Equals_R2(int r1, int r2)
{
	if ((r1 < 0) || (r2 < 0)) {return;}
	//adds rows r1 and r2 and stores result in row r1 within vector member M
	for (int i = 0; i < M[r1].size(); i++) {M[r1][i] = ((M[r1][i] + M[r2][i]) % 2) + '0';}	
}

Bitmatrix::Bitmatrix(string fn) 
{
  ifstream f;
  int i, j;
  string s, row;

  f.open(fn.c_str());
  if (f.fail()) { perror(fn.c_str()); M.resize(1); M[0].resize(1), M[0][0] = '0'; return; }

  while (getline(f, s)) {
    row.clear();
    for (i = 0; i < s.size(); i++) {
      if (s[i] == '0' || s[i] == '1') {
        row.push_back(s[i]);
      } else if (s[i] != ' ') {
        fprintf(stderr, "Bad matrix file %s\n", fn.c_str());
        M.resize(1); M[0].resize(1), M[0][0] = '0'; 
        f.close();
        return;
      }
    }
    if (s.size() > 0 && M.size() > 0 && row.size() != M[0].size()) {
      fprintf(stderr, "Bad matrix file %s\n", fn.c_str());
      M.resize(1); M[0].resize(1), M[0][0] = '0'; 
      f.close();
      return;
    }
    if (s.size() > 0) M.push_back(row);   
  }
  f.close();
}

void Bitmatrix::PGM(string fn, int pixels, int border)
{
	ofstream outF;

	outF.open(fn.c_str());
	if (outF.fail()) {perror(fn.c_str()); return;}

	//total output dimensions for PGM file
	int PGMcols = (M[0].size() * pixels) + (border * (M[0].size() + 1));
	int PGMrows = (M.size() * pixels) + (border * (M.size() + 1));
	
	//PGM header info
	outF << "P2\n" << PGMcols << " " << PGMrows << "\n" << "255\n";

	//outputs pixels of PGM file based on "pixels" and "border" parameters 
	for (int PGMrow = 0; PGMrow < PGMrows; PGMrow++)
	{
		for (int PGMcol = 0; PGMcol < PGMcols; PGMcol++)
		{
			//border == 0 conditions
			if (border == 0 && M[PGMrow/pixels][PGMcol/pixels] == '0') {outF << " " << 255;} 
			if (border == 0 && M[PGMrow/pixels][PGMcol/pixels] == '1') {outF << " " << 100;} 
			
			//border != 0 conditions
			//condition for printing rows of only border pixels to PGM file 
			if (((PGMrow % (pixels + border)) < border) && (border != 0)) {outF << " " << 0;}
  		
			//condition for printing border pixels AND pixels from the "pixel by pixel" square
			if (((PGMrow % (pixels + border)) >= border) && (border != 0)) 
			{
				if ((PGMcol % (pixels + border)) < border) {outF << " " << 0;}
				if (((PGMcol % (pixels + border)) >= border) && (M[PGMrow/(pixels + border)][PGMcol/(pixels + border)] == '0')) {outF << " " << 255;}
				if (((PGMcol % (pixels + border)) >= border) && (M[PGMrow/(pixels + border)][PGMcol/(pixels + border)] == '1')) {outF << " " << 100;}
			}
			
			if (PGMcol == (PGMcols - 1)) {outF << "\n";} 
		}
	}

	outF.close();
}

Bitmatrix *Bitmatrix::Copy()
{
	Bitmatrix *newBM = NULL;
	//declared to save on excessive calling of .size() function
	int Rows = M.size(), Cols = M[0].size();

	//creates new Bitmatrix object and initializes the new Matrix vector with the same dimensions, but with all elements == 0
	newBM = new Bitmatrix(Rows, Cols);

	//copies each element of vector M from CURRENT OBJ to the same element of vector M for NEW OBJ
	for (int row = 0; row < Rows; row++)
	{
		for (int col = 0; col < Cols; col++)
		{
			newBM->Set(row, col, M[row][col]);			
		}
	}
	
	return newBM;
}


BM_Hash::BM_Hash(int size)
{
	table.resize(size);	
}

void BM_Hash::Store(string &key, Bitmatrix *bm)
{
	unsigned int hashVal = djbHash(key);
	int tableIndex = hashVal % table.size();
	int tableElementSize = table[tableIndex].size();
	HTE *HTEptr = new HTE;

	HTEptr->key = key;
	HTEptr->bm = bm;
	
	//key not previously stored since element is empty
	if (tableElementSize == 0){table[tableIndex].push_back(HTEptr);}
	//check if key has been previously inserted. If so, replace Bitmatrix obj. If not, add new HTE object
	else 
	{
		
		for (int col = 0; col < tableElementSize; col++)
		{
			if (table[tableIndex][col]->key == key) {table[tableIndex][col]->bm = bm; return;}
		}
		
		//key was not previously stored after checking. Store new HTE object.
		table[tableIndex].push_back(HTEptr);
	}
}
  
Bitmatrix *BM_Hash::Recall(string &key)
{
	unsigned int hashVal = djbHash(key);
	int tableIndex = hashVal % table.size();
	int tableElementSize = table[tableIndex].size();

	//returns bitmatrix from hash table based on key if found. If not found, returns NULL.
	for (int col = 0; col < tableElementSize; col++)
	{
		if (table[tableIndex][col]->key == key) {return table[tableIndex][col]->bm;}
	}
	
	return NULL;
}
  
HTVec BM_Hash::All()
{
	HTVec rv; //HTVec == vector <HTE *>
	int tableSize = table.size();

	//creates a vector which stores all elements from the hash table (element == pointer to HTE object) 
	for (int row = 0; row < tableSize; row++)
	{
		for (int col = 0; col < table[row].size(); col++)
		{
			rv.push_back(table[row][col]);
		}
	}

	return rv;
}


Bitmatrix *Sum(Bitmatrix *m1, Bitmatrix *m2)
{
	int m1Rows = m1->Rows(), m1Cols = m1->Cols(), m2Rows = m2->Rows(), m2Cols = m2->Cols();
	
	//If m1 and m2 are not the same size, return NULL. 
	if ( (m1Rows != m2Rows) || (m1Cols != m2Cols) ) {return NULL;}
	
	//This creates a new bit-matrix which is the sum of m1 and m2. 
	Bitmatrix *m3 = new Bitmatrix(m1Rows, m1Cols);

	//sums the individual elements of m1 and m2 together and stores the result in the corresponding element of m3 
	for (int row = 0; row < m1Rows; row++)
	{
		for (int col = 0; col < m1Cols; col++)
		{
			m3->Set(row, col, (m1->Val(row,col) + m2->Val(row,col)) % 2);
		}
	}

	return m3;
}

Bitmatrix *Product(Bitmatrix *m1, Bitmatrix *m2)
{
	int m1Rows = m1->Rows(), m1Cols = m1->Cols(), m2Rows = m2->Rows(), m2Cols = m2->Cols();
	int mProduct = 0;
	
	//if m1 cols and m2 rows are not equal, then dot product is not possible. Return NULL.
	if (m1Cols != m2Rows) {return NULL;}
	
	Bitmatrix *m3 = new Bitmatrix(m1Rows, m2Cols);

	//Performs matrix multiplication between m1 and m2 and stores result in m3  	
	for (int m1Row = 0; m1Row < m1Rows; m1Row++)
	{
		for (int m2Col = 0; m2Col < m2Cols; m2Col++)
		{
			for (int m1Col = 0; m1Col < m1Cols; m1Col++)
			{
				mProduct += m1->Val(m1Row, m1Col) * m2->Val(m1Col, m2Col);
			}
			
			m3->Set( m1Row, m2Col, (mProduct) % 2);
			mProduct = 0;
		}
	}
	
	return m3;
}

Bitmatrix *Sub_Matrix(Bitmatrix *m, vector <int> &rows)
{
	int mRows = m->Rows(), mCols = m->Cols(), vRows = rows.size();
	
	//return NULL if given empty vector as "rows"
	if (vRows == 0) {return NULL;}

	Bitmatrix * m2 = new Bitmatrix(vRows, mCols);

	//set elements of new bitmatrix m2 to the rows specified in vector "rows"
	for (int vRow = 0; vRow < vRows; vRow++)
	{
		for (int mCol = 0; mCol < mCols; mCol++)
		{
			m2->Set(vRow, mCol, m->Val(rows[vRow], mCol));
		}
	
	}

	return m2;
}

Bitmatrix *Inverse(Bitmatrix *m)
{
	//Create copy of Bitmatrix m and allocate space for the inverse matrix.
	Bitmatrix *mCopy = m->Copy();
	Bitmatrix *mInv = new Bitmatrix(m->Rows(),m->Cols());
	bool notInvertible = true;

	//create identity matrix
	for (int mRow = 0; mRow < m->Rows(); mRow++) {mInv->Set(mRow, mRow, '1');}

	//1st PASS in creating inverted Bitmatrix. This should convert "mCopy" bitmatrix to be in upper-triangular form unless "m" is not invertible.
	for (int mRow = 0; mRow < (mCopy->Rows() - 1); mRow++)
	{
		//Swapping rows
		if ((mCopy->Val(mRow, mRow) != 1))
		{  
			notInvertible = true;
			
			for (int mRow2 = (mRow + 1); mRow2 < mCopy->Rows(); mRow2++)
			{
				if (mCopy->Val(mRow2, mRow) == 1) 
				{
					mCopy->Swap_Rows(mRow, mRow2);
					mInv->Swap_Rows(mRow, mRow2);
					notInvertible = false;
					break;
				}
			}

			//Bitmatrix "m" is not invertible
			if (notInvertible) {delete mInv; delete mCopy; return NULL;}
		}

		//Setting rows
		if ((mCopy->Val(mRow, mRow) == 1)) 
		{  
			for (int mRow2 = (mRow + 1); mRow2 < mCopy->Rows(); mRow2++)
			{
				if (mCopy->Val(mRow2, mRow) == 1) 
				{
					mCopy->R1_Plus_Equals_R2(mRow2, mRow);
					mInv->R1_Plus_Equals_R2(mRow2, mRow);
				}
			}
		}
	}

	//Checking mCopy to make sure it is in "upper-triangular" form
	for (int mRow = 0; mRow < mCopy->Rows(); mRow++)
	{
		if (mCopy->Val(mRow, mRow) != 1) {delete mInv; delete mCopy; return NULL;}
	}
	
	//2nd PASS in creating inverted Bitmatrix. At this point, bitmatrix "m" has been determined to be invertible.  
	for (int mRow = (mCopy->Rows() - 2); mRow >=0; mRow--)
	{
		//Setting rows
		for (int mRow2 = (mRow + 1); mRow2 < mCopy->Cols(); mRow2++)
		{
			if (mCopy->Val(mRow, mRow2) == 1)
			{
				mCopy->R1_Plus_Equals_R2(mRow, mRow2);
				mInv->R1_Plus_Equals_R2(mRow, mRow2);
			}
		}
	}
	
	//"mCopy" is no longer needed since "mInv" holds the inverse of "m". Delete it.
	delete mCopy;
	
	return mInv;
}
