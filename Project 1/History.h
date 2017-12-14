#ifndef HISTORY_H
#define HISTORY_H

#include "globals.h"


class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;

private:

	int     m_hgrid[MAXROWS][MAXCOLS];
	int     m_rows;
	int     m_cols;

};
#endif // !HISTORY_H
