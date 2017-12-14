#include "Sequence.h"

Sequence::Sequence()
{
	m_size = 0;
}

bool Sequence::empty() const
{
	if (m_size == 0)
		return true;
	else
		return false;
}

int Sequence::size() const
{
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
	if (0 <= pos && pos <= size() && size()+1 <= maxItems) 
	{
		ItemType temp1, temp2 = m_array[pos]; //creates two temps, sets temp2 to the value @pos in m_array
		m_array[pos] = value;
		for (int i = (pos+1); i <= size(); i++)
		{
			temp1 = m_array[i];
			m_array[i] = temp2;
			temp2 = temp1;
		}
		m_size++;
		return true;
	}
	else
		return false;
}



int Sequence::insert(const ItemType& value)
{
	int p = -1;
	int i = 0;
	if ((size() + 1) <= maxItems)
	{
		do
		{
			if ((value <= m_array[i] && p < 0) || (i == size()))
			{
				p = i;
				this->insert(p, value);
			}
			i++;
		} while (p < 0 && i <= size());
	}
	return p;
}


bool Sequence::erase(int pos)
{
	if (0 <= pos && pos < size())
	{
		for (int i = (pos); i < size()-1; i++)
		{
			if (i < size())
			{
				ItemType temp1 = m_array[i + 1];
				m_array[i] = temp1;
			}

		}
		m_size--;
		return true;
	}
	else
		return false;
}


int Sequence::remove(const ItemType& value)
{
	int itemsRemoved = 0;
	for (int i = 0; i < size(); i++)
	{
		if (m_array[i] == value)
		{
			this->erase(i);
			itemsRemoved++;
			if (m_array[i] == value) //this resets i in the case the 
			{						 //element following our value also needs to be removed.
				i = -1;
			}
		}
	}
		return itemsRemoved;

}


bool Sequence::get(int pos, ItemType& value) const
{
	if (0 <= pos && pos < size())
	{
		value = m_array[pos];
		return true;
	}
	else
		return false;
}


bool Sequence::set(int pos, const ItemType& value)
{
	if (0 <= pos && pos < size())
	{
		m_array[pos] = value;
		return true;
	}
	else
		return false;
}


int Sequence::find(const ItemType& value) const
{
	int p = -1, i = 0;
	do
	{
		if (m_array[i] == value)
		{
			p = i;
		}
		i++;
	} while ((p < 0) && (i < size()));
	return p;
}

void Sequence::swap(Sequence& other)
{
	ItemType temp1;
	for (int i = 0; i < other.size(); i++)
	{
		other.get(i, temp1);
		other.set(i, m_array[i]);
		m_array[i] = temp1;
	}
}

