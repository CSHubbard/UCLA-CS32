#include "ScoreList.h"

ScoreList::ScoreList()
{
	//I dont think I need to do anything here 
	//as the size of the scorelist is equal to the size of the sequence so 
	//the sequence will always keep track of empty-ness and size;
}

bool ScoreList::add(unsigned long score)
{
	if (0 <= score && score <= 100)
	{
		m_rec.insert(score);
		return true;
	}
	return false;
}

bool ScoreList::remove(unsigned long score)
{

	if (m_rec.find(score) != -1)
	{
		m_rec.erase(m_rec.find(score));
		return true;
	}
	return false;
}

int ScoreList::size() const
{
	return m_rec.size();
}

unsigned long ScoreList::minimum() const
{
	if (m_rec.size() > 0)
	{
		unsigned long temp, lowest;
		m_rec.get(0, lowest);
		for (int i = 0; i < m_rec.size(); i++)
		{
			m_rec.get(i, temp);
			if (temp <= lowest)
			{
				lowest = temp;
			}
		}
		return lowest;
	}
	return NO_SCORE;

}


unsigned long ScoreList::maximum() const
{
	if (m_rec.size() > 0)
	{
		unsigned long temp, highest;
		m_rec.get(0, highest);
		for (int i = 0; i < m_rec.size(); i++)
		{
			m_rec.get(i, temp);
			if (temp >= highest)
			{
				highest = temp;
			}
		}
		return highest;
	}
	return NO_SCORE;
}
// Return the highest score in the score list.  If the list is
// empty, return NO_SCORE.

