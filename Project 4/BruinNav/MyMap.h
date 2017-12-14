// MyMap.h
#include "support.h"
// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap() :rootPtr(nullptr) {}

	~MyMap()
	{
		freeTree(rootPtr);
		rootPtr = nullptr;
	}

	void clear()
	{
		freeTree(rootPtr);
		rootPtr = nullptr;
	}

	int size() const
	{
		return getCount(rootPtr);
	}

	///2*log(N) (total - average)
	void associate(const KeyType& key, const ValueType& value)
	{
		if (rootPtr == nullptr)
		{
			rootPtr = new BSTNode(key, value);
			return;
		}

		///log(N) (average)
		if (find(key) != nullptr)
		{

			*find(key) = value; return;
		}

		///& at worst +log(N) 
		BSTNode *cur = rootPtr;
		for (;;)
		{
			if (key < cur->m_key)
			{
				if (cur->left != nullptr)
					cur = cur->left;
				else
				{
					cur->left = new BSTNode(key, value);
					return;
				}
			}
			else if (key > cur->m_key)
			{
				if (cur->right != nullptr)
					cur = cur->right;
				else
				{
					cur->right = new BSTNode(key, value);
					return;
				}
			}
		}
	}

	// for a map that can't be modified, return a pointer to const ValueType
	/// (average) log(N) where N is the number of nodes
	const ValueType* find(const KeyType& key) const
	{
		BSTNode *searchPtr = rootPtr;
		while (searchPtr != nullptr)
		{
			if (key == searchPtr->m_key)
				return &searchPtr->m_value;
			else if (key < searchPtr->m_key)
				searchPtr = searchPtr->left;
			else if (key > searchPtr->m_key)
				searchPtr = searchPtr->right;
		}
		return nullptr;
	}


	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct BSTNode
	{
		KeyType m_key;				//key
		ValueType m_value;			//data
		BSTNode *left, *right;		//pointers

		BSTNode(KeyType key, ValueType value)
			:m_key(key), m_value(value), left(nullptr), right(nullptr)
		{}
	};

	BSTNode* rootPtr;	//pointer to root of the classes BST

	//recursively deletes the trees nodes
	void freeTree(BSTNode *cur)
	{
		if (cur == nullptr)
			return;
		freeTree(cur->left);
		freeTree(cur->right);
		delete cur;
	}

	//recursively counts nodes in the tree
	int getCount(const BSTNode *cur) const
	{
		int temp = 0;
		if (cur == nullptr)
			return temp;
		temp++;
		temp += getCount(cur->left);
		temp += getCount(cur->right);
		return temp;
	}
};
