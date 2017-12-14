///////////////////////////////////////////////////////////////////////////
//  History implementation
///////////////////////////////////////////////////////////////////////////

#include "History.h"
#include "globals.h"
#include <iostream>

using namespace std;

History::History(int nRows, int nCols)
	: m_rows(nRows), m_cols(nCols)
{
	m_rows = nRows;
	m_cols = nCols;

	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			m_hgrid[r - 1][c - 1] = EMPTY;
}

bool History::record(int r, int c)
{
	if (m_rows <= 0 || m_cols <= 0 || m_rows > MAXROWS || m_cols > MAXCOLS)
	{
		return false;
	}

	else
	{
		m_hgrid[r - 1][c - 1] +=1 ;
		return true;
	}

}

void History::display() const
{

	char hdisplayGrid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill hdisplayGrid 
	for (r = 1; r <= m_rows; r++)
		for (c = 1; c <= m_cols; c++)
		{
			hdisplayGrid[r - 1][c - 1] = (m_hgrid[r - 1][c - 1] == EMPTY ? '.' : m_hgrid[r - 1][c - 1] >=26 ? 'Z' : (m_hgrid[r - 1][c - 1] + 64));
		}

	// draw the grid
	clearScreen();
	for (r = 1; r <= m_rows; r++)
	{
		for (c = 1; c <= m_cols; c++)
			cout << hdisplayGrid[r - 1][c - 1];
		cout << endl << endl;
	}
}