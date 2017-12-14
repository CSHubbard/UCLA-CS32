// Sequence.cpp
#include <string>
#include <iostream>
#include "Sequence.h"

int Sequence::size() const //
{
	return m_size;
}

Sequence::Sequence() //
	:headN(nullptr), tailN(nullptr), m_size(0)
{
}

bool Sequence::empty() const //
{
	return size() == 0;
}

bool Sequence::insert(int pos, const ItemType& value) //
{
	if (pos < 0 || pos > size())
		return false;
	uncheckedInsert(pos, value);	
	return true;
}

int Sequence::insert(const ItemType& value) //
{
	Node *searchN = headN;		//node to search for smallest pos with value less than given value
	int pos;
	for (pos = 0; pos < size() && value > searchN->m_value; pos++)
		searchN = searchN->nextN;
	uncheckedInsert(pos, value);
	return pos;
}

bool Sequence::erase(int pos) //
{
	if (pos < 0 || pos >= size())
		return false;
	else
	{
		uncheckedRemove(pos);	//removes the value at position pos
		return true;
	}
}

int Sequence::remove(const ItemType& value)  //
{
	int count = 0;
	int keepPos = find(value);
	while (keepPos != -1)					//searches for a the value and will continually 
	{										//remove them as long as thevalue is found
		this->uncheckedRemove(keepPos);
		count++;
		keepPos = find(value);
	}	
		return count;
}

bool Sequence::get(int pos, ItemType& value) const	//
{
	Node *searchN = headN; //used to traverse list

	if (pos < 0 || pos >= size())
		return false;
	for (int i = 0; i < pos; i++)
		searchN = searchN->nextN;
	value = searchN->m_value;  //sets value passed to value in the node
	return true;
}

bool Sequence::set(int pos, const ItemType& value) //
{
	Node *searchN = headN; //used to traverse list

	if (pos < 0 || pos >= size())
		return false;
	for (int i = 0; i < pos; i++)	//loops until at correct position
		searchN = searchN->nextN;
	searchN->m_value = value;  //sets value to value at position in the list
	return true;
}

int Sequence::find(const ItemType& value) const //
{
	int pos;
	Node *searchN = headN; //pointer used to search the list

	for (pos = 0; pos < size(); pos++)
	{
		if (searchN->m_value == value)
			return pos;
		searchN = searchN->nextN;
	}
	return -1;
}

void Sequence::swap(Sequence& other) 
{
	// Swap elements.  Since the all functions operate based on
	//head an tail pointers those are the only values which need
	//to be switched (and sizes)

	Node *tempN = nullptr; //temp pointer for transferring pointer values

	// Swap sizes
	int tempSize = m_size;
	m_size = other.m_size;
	other.m_size = tempSize;

	//swap head nodes
	tempN = headN;
	this->headN = other.headN;
	other.headN = tempN;

	//swap tail nodes
	tempN = this->tailN;
	this->tailN = other.tailN;
	other.tailN = tempN;	
}

void Sequence::uncheckedInsert(int pos, const ItemType& value)
{


	Node *newN;				//pointer for our new node 
	newN = new Node;		//initalizes node
	newN->m_value = value;	

	if (empty())			//sets first and last to new node if the list is empty
	{
		headN = tailN = newN;
	}

	else if (pos == 0) //will insert at beggining
	{
		newN->nextN = headN;
		std::cerr << m_size;
		headN->prevN = newN;
		headN = newN;
	}

	else if (pos == size())	//will insert at the end
	{
		newN->prevN = tailN;
		tailN->nextN = newN;
		tailN = newN;
	}

	else //insert for all other positions
	{
		Node *tempN = headN;
		for (int i = 0; i < pos; i++)		//find correct position
		{
			tempN = tempN->nextN;
		}
			
		newN->nextN = tempN;				//new nodes next and prev pointers
		newN->prevN = tempN->prevN;
		tempN->prevN->nextN = newN;			//prev node's next ptr
		tempN->prevN = newN;				//follwing nods prev ptr
	}

	m_size++;
}

void Sequence::uncheckedRemove(int pos)
{
	Node *delMe = nullptr; //ptr to node to be deleted

	if (pos == 0) //delete node at beg
	{
		if (size() == 1) //special case list has only 1 element
		{
			delMe = headN;				//readies deletion
			headN = tailN = nullptr;	//unlinks pointers
			delete delMe;				//deletes
		}
			
		else            
		{								
			delMe = headN;
			headN = headN->nextN;		//sets head node to next in list
			headN->prevN = nullptr;		//unlinks old node at pos 0
			delete delMe;				//deletes node
		}

	}

	else if (pos == size()-1)// delete node at end
	{
		delMe = tailN;
		tailN = tailN->prevN;		//sets tail node to prev in list
		tailN->nextN = nullptr;		//unlinks old node at last pos
		delete delMe;
	}

	else                            //delete any other node
	{
		Node *tempN = headN;				//temp node to travel the list
		for (int i = 0; i < pos; i++)		
		{
			tempN = tempN->nextN;
		}

		delMe = tempN;							//sets target for deleteion
		tempN->nextN->prevN = tempN->prevN;		//unlinks next node from tempnode
		tempN->prevN->nextN = tempN->nextN;		//unlinks prev node from tempnode
		delete delMe;
	}
	m_size--;
}

//housekeeping implementations

Sequence::~Sequence()
{

	if (m_size > 0) //extreaneous case seq is empty
	{
		Node *delMe = headN;
		Node *tempN = nullptr;
		while (delMe != nullptr && delMe->nextN != nullptr)
		{
			tempN = delMe->nextN;
			delete delMe;
			delMe = tempN;
		}
		delete delMe;
	}

}


Sequence::Sequence(const Sequence& other)
{
	if (!other.empty()) //extreaneous case that the passed seq is empty
	{
		Node *tempN = other.headN;	//temporary headnode to traverse passed list

		for (int i = 0; i < other.size(); i++)
		{
			if (tempN == nullptr) //checks if we are at end of the list
				break;
			insert(i, tempN->m_value);
			tempN = tempN->nextN;
		}
	}
}

Sequence& Sequence::operator=(const Sequence& rhs)	//just uses the copy constructor to set them equal
{													//and the destructor to ensure there are no memory leaks  
	if (this != &rhs)
	{
		Sequence temp(rhs);
		this->swap(temp);
	}
	return *this;
}

//Node Functions
Sequence::Node::Node()
	:prevN(nullptr), nextN(nullptr)
{
}

//NonMember Functions

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	if ((seq2.size() > seq1.size()) || seq2.size() == 0 || seq1.size() == 0)	//checks if the subsequence is too big
		return -1;															//or if the subsequence is empty

	int firstPos = -1;				//tracks the earliest position that the subsequence starts

	//variables need for loop
	ItemType sq1Temp, sq2Temp;		//temporary variables used to comapre seq1 and seq2 values
	int cele = 0;					//tracks position of subsequence being compared and 
									//tracks the number of consecutive elements found identical 

	for (int pos = 0; pos < seq1.size(); pos++)	//will go through the elements for seq1 and seq2
	{
		seq1.get(pos, sq1Temp);		//gets the current values to be compared
		seq2.get(cele, sq2Temp);

		if (cele == seq2.size())	//checks if we are at the end of the subSeq
		{
			if (cele != 0)
				firstPos = cele;
			break;
		}
		else if (sq1Temp == sq2Temp)	//compares values of seq and the subseq, iterates the 
		{								//number of elements found and subseq pos if they are equal
			cele++;
		}
		else                            //resets counters if not equal
		{
			cele = 0;
		}
	}
	return firstPos;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)   // Inserts values from seq1 and seq2 into result one by one.
{
	Sequence tempSeq; //sequence which will hold our result, avoids alaising

	int sq1Pos = 0, sq2Pos = 0, resPos = 0; //position counter for 3 seq passed
	ItemType sq1Temp, sq2Temp;				//temporary variables used to insert seq1 and seq2 values
											//into temp sequence
	bool modified = false;

	//while (tempSeq.empty() == false)//clears our temp
	//{
	//tempSeq.erase(0);
	//}

	while ((sq1Pos != seq1.size()) || (sq2Pos != seq2.size()))
	{
		if (sq1Pos < seq1.size())
		{
			seq1.get(sq1Pos, sq1Temp);
			sq1Pos++;
			tempSeq.insert(resPos,sq1Temp);
			resPos++;
		}
		if (sq2Pos < seq2.size())
		{
			seq2.get(sq2Pos, sq2Temp);
			sq2Pos++;
			tempSeq.insert(resPos, sq2Temp);
			resPos++;
		}
		else if (resPos < tempSeq.size())
		{
			tempSeq.erase(resPos);
			resPos++;
		}
		modified = true;
	}
	if (modified == true)
		result = tempSeq;
}
