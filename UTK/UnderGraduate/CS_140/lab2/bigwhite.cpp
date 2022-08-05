//Tom Hills (jhills)
//lab 2 --  bigwhite

#include <cstdlib>
#include <cstdio>

using namespace std;

int main(int argc, char * argv[])
{

	int rows, columns, row, column;

	//ERROR CHECK: ensures correct number of arguments provided
	if (argc != 3) {fprintf(stderr, "usage: bigwhite rows cols\n"); return 1;}

	//ERROR CHECK: ensures "rows" variable is given non-negative integers 
	for (int i = 0; i < 50; i++) //50 chosen arbitrarily as upperbound assuming noone would provide a number over 50 characters long (null terminator included)
	{
		if (argv[1][i] == '\0') {break;}
		if (argv[1][i] == '-') {fprintf(stderr, "usage: bigwhite rows cols\n"); return 1;}
		if (argv[1][i] < '0' || argv[1][i] > '9') {fprintf(stderr, "usage: bigwhite rows cols\n"); return 1;}
	}

	//ERROR CHECK: ensures "columns" variable is given non-negative integers 
	for (int i = 0; i < 50; i++)
	{
		if (argv[2][i] == '\0') {break;}
		if (argv[2][i] == '-') {fprintf(stderr, "usage: bigwhite rows cols\n"); return 1;}
		if (argv[2][i] < '0' || argv[2][i] > '9') {fprintf(stderr, "usage: bigwhite rows cols\n"); return 1;}
	}
	
	rows = atoi(argv[1]);
	columns = atoi(argv[2]);
	
	//ERROR CHECK: ensures "rows" and "columns" are not equal to integer ZERO.
	if (rows == 0) {fprintf(stderr, "usage: bigwhite rows cols\n"); return 1;}	
	if (columns == 0) {fprintf(stderr, "usage: bigwhite rows cols\n"); return 1;}	

	//prints "header" entries to pgm file
	fprintf(stdout, "P2\n%d %d\n255\n", columns, rows);

	//prints pixel information (all white, i.e., 255) to pgm file
	for (row = 0; row < rows; row++)
	{
		for (column = 0; column < columns; column++)
		{
			if (column == (columns - 1)) {fprintf(stdout, "255\n");}
			if (column < (columns - 1)) {fprintf(stdout, "255 ");}
		}
	}
	
	return 0;
}
