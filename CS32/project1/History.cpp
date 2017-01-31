#include <iostream>
#include "History.h"

using namespace std;

History::History(int nRows, int nCols) : h_rows(nRows), h_cols(nCols)
{
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//Initialize each relevant point in recGrid to a 0 to be incremented later by the record function
	///////////////////////////////////////////////////////////////////////////////////////////////////
	for (int x = 1; x <= h_rows; x++)
	for (int y = 1; y <= h_cols; y++)
		recGrid[x-1][y-1] = 0;
}

bool History::record(int r, int c)
{
	if (!(r >= 1 && r <= h_rows  &&  c >= 1 && c <= h_cols))
		return false;
	recGrid[r-1][c-1]++;
	return true;
}
void History::display() const
{
	char letters[26] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

	clearScreen();
	for (int x = 1; x <= h_rows; x++)
	{
		for (int y = 1; y <= h_cols; y++)
		{
			if (recGrid[x - 1][y - 1] >= 26)//If 26 of more zombies have died at a grid point, cout "Z"
				cout << "Z";
			else if (recGrid[x - 1][y - 1] == 0)
				cout << ".";
			else
				cout << letters[recGrid[x - 1][y - 1]-1];
		}
		cout << endl;
	}
	cout << endl;
}