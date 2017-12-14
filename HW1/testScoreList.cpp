#include "ScoreList.h"
#include <iostream>

using namespace std;
int main()
{
	ScoreList l;
	l.add(50);
	l.add(100);
	l.add(25);
	l.add(50);
	l.add(-20);
	l.add(250);
	l.remove(50);
	l.remove(250);


	l.maximum();
	l.minimum();
	l.size();

	//I used the debugger to ensure these 'tests'
	//gave the correct values
}