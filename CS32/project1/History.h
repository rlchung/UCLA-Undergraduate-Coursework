#ifndef HISTORY_INCLUDED
#define HISTORY_INCLUDED

#include "globals.h"

class History
{
public:
	History(int nRows, int nCols);//constructor that initializes a History object that corresponds to an Arena with nRows rows and nCols columns
	bool record(int r, int c);
	void display() const;

private:
	int h_rows;//the x-rows of the history grid
	int h_cols;//the y-cols of the history grid
	int recGrid[MAXROWS][MAXCOLS];//total grid that will record/increment each slot that the zombies die at
};

#endif