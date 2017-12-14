// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "MyMap.h"
#include <string>
#include <iostream>
using namespace std;
int main()
{
	Navigator bruinNav;
	bruinNav.loadMapData("mapdata.txt");

	vector<NavSegment> directions;
	bruinNav.navigate("1061 Broxton Avenue", "Headlines!", directions);

}

