#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <queue>


using namespace std;

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:

	string getDirectionProc(GeoSegment seg1) const;
	string getDirectionTurn(GeoSegment seg1, GeoSegment seg2) const;
	StreetSegment findStreet(GeoSegment findMe) const;

	MapLoader m_MainMap;
	AttractionMapper m_AttractionMap;
	SegmentMapper m_SegmentMap;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	if (!m_MainMap.load(mapFile))
		return false;
	m_AttractionMap.init(m_MainMap);
	m_SegmentMap.init(m_MainMap); 
	return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	vector<NavSegment> m_Path;

	GeoCoord startLoc;
	if (!m_AttractionMap.getGeoCoord(start, startLoc))
		return NAV_BAD_SOURCE;
	vector<StreetSegment> startAscSeg(m_SegmentMap.getSegments(startLoc));
	if (startAscSeg.empty())
		return NAV_BAD_SOURCE;

	StreetSegment startStreet;
	for (int i = 0; i < startAscSeg.size(); i++)
	{
		for (int k = 0; k < startAscSeg[i].attractions.size(); k++)
		{
			if (startAscSeg[i].attractions[k].geocoordinates == startLoc)
				startStreet = startAscSeg[i];
		}
	}

	GeoCoord endLoc;
	if (!m_AttractionMap.getGeoCoord(end, endLoc))
		return NAV_BAD_DESTINATION;
	vector<StreetSegment> endAscSeg = m_SegmentMap.getSegments(endLoc);
	if (endAscSeg.empty())
		return NAV_BAD_DESTINATION;

	StreetSegment endStreet;
	for (int i = 0; i < endAscSeg.size(); i++)
	{
		for (int k = 0; k < endAscSeg[i].attractions.size(); k++)
		{
			if (endAscSeg[i].attractions[k].geocoordinates == endLoc)
				endStreet = endAscSeg[i];
	}
		}

	//start of A* pathfinding 
	priority_queue<node*> openList;
	MyMap<GeoCoord, string> closedList;

	bool pathFound = false;
	node pathNode;
	node* pathWatcher = nullptr;

	node *start1 = new node;
	start1->pos = startStreet.segment.start;
	start1->parent = nullptr;
	start1->g = distanceEarthMiles(startLoc, start1->pos);
	start1->h = distanceEarthMiles(start1->pos, endLoc);
	start1->f = start1->g + start1->h;
	openList.push(start1);

	node *start2 = new node;
	start2->pos = startStreet.segment.end;
	start2->parent = nullptr;
	start2->g = distanceEarthMiles(startLoc, start1->pos);
	start2->h = distanceEarthMiles(start2->pos, endLoc);
	start2->f = start2->g + start2->h;
	openList.push(start2);

	while (!openList.empty() && pathFound == false)
	{
		node *cur = openList.top(); 
		if (closedList.find(cur->pos) == nullptr)
			closedList.associate(cur->pos, "vis");
		openList.pop();

		vector<StreetSegment> curAsc(m_SegmentMap.getSegments(cur->pos));

		for (int j = 0; j < curAsc.size(); j++)
		{
			if (curAsc[j].segment == endStreet.segment) 
			{
				pathFound = true;
				pathNode = *cur;
				pathWatcher = cur;
				break;
			}

			if (cur->pos != curAsc[j].segment.start)
			{
				node *tempN = new node;
				tempN->pos = curAsc[j].segment.start;
				tempN->parent = cur;
				tempN->g = distanceEarthMiles(cur->pos, tempN->pos);
				tempN->h = distanceEarthMiles(tempN->pos, endLoc);
				tempN->f = tempN->g + tempN->h;

				if (closedList.find(tempN->pos) == nullptr)
					openList.push(tempN);
				else
					delete tempN;
			}

			if (cur->pos != curAsc[j].segment.end)
			{

				node *tempN = new node;
				tempN->pos = curAsc[j].segment.end;
				tempN->parent = cur;
				tempN->g = distanceEarthMiles(cur->pos, tempN->pos);
				tempN->h = distanceEarthMiles(tempN->pos, endLoc);
				tempN->f = tempN->g + tempN->h;

				if (closedList.find(tempN->pos) == nullptr)
					openList.push(tempN);
				else
					delete tempN;
			}
		}
	} //end of A* pathfinding

	if (pathFound == false)
		return NAV_NO_ROUTE;

	//create direction segments start
	directions.clear();
	node curPos = pathNode;

	GeoSegment toDest;
	toDest.start = pathNode.pos;
	toDest.end = endLoc;
	NavSegment lastSeg(getDirectionProc(toDest), endStreet.streetName, 
		distanceEarthMiles(toDest.start, toDest.end), toDest);
	directions.insert(directions.begin(), lastSeg);

	while (curPos.parent != nullptr)
	{
		GeoSegment tempSeg;
		tempSeg.start = curPos.parent->pos;
		tempSeg.end = curPos.pos;
		StreetSegment myStreet = findStreet(tempSeg);

		if (myStreet.streetName != directions[0].m_streetName)
		{
			NavSegment tempNav(getDirectionTurn(tempSeg, directions[0].m_geoSegment), directions[0].m_streetName);
			directions.insert(directions.begin(), tempNav);
		}

		NavSegment tempNav(getDirectionProc(tempSeg), myStreet.streetName,
			distanceEarthMiles(tempSeg.start, tempSeg.end), tempSeg);
		directions.insert(directions.begin(), tempNav);
		curPos = *curPos.parent;
	}

	GeoSegment startSpot;
	startSpot.start = startLoc;
	startSpot.end = curPos.pos;
	NavSegment startSeg(getDirectionProc(startSpot), startStreet.streetName,
		distanceEarthMiles(startSpot.start, startSpot.end), startSpot);
	directions.insert(directions.begin(), startSeg);


	//housekeeping
	while (!openList.empty())
	{
		node* temp = openList.top();
		openList.pop();
		delete temp;
	}


	while (pathWatcher != nullptr)
	{
		node* temp = pathWatcher->parent;
		delete pathWatcher;
		pathWatcher = temp;
	}
	
	return NAV_SUCCESS;
}	


//private helpers
StreetSegment NavigatorImpl::findStreet(GeoSegment findMe) const
{
	vector<StreetSegment> segsWStart(m_SegmentMap.getSegments(findMe.start));
	vector<StreetSegment> segsWEnd(m_SegmentMap.getSegments(findMe.end));
	for (int i = 0; i < segsWStart.size(); i++)
	{
		for (int k = 0; k < segsWEnd.size(); k++)
		{
			if (segsWStart[i].segment == segsWEnd[k].segment)
				return segsWEnd[k];
		}
	}
}

string NavigatorImpl::getDirectionTurn(GeoSegment seg1, GeoSegment seg2) const
{
	double angle = angleBetween2Lines(seg1, seg2);
	if (angle < 180)
		return "left";
	if (angle >= 180)
		return "right";
}

string NavigatorImpl::getDirectionProc(GeoSegment seg1) const
{
	double travelAngle = angleOfLine(seg1);
	if (0 <= travelAngle && travelAngle <= 22.5)
		return  "east";
	if (22.5 < travelAngle && travelAngle <= 67.5)
		return  "northeast";
	if (67.5 < travelAngle && travelAngle <= 112.5)
		return  "north";
	if (112.5 < travelAngle && travelAngle <= 157.5)
		return  "northwest";
	if (157.5 < travelAngle && travelAngle <= 202.5)
		return  "west";
	if (202.5 < travelAngle && travelAngle <= 247.5)
		return  "southwest";
	if (247.5 < travelAngle && travelAngle <= 292.5)
		return  "south";
	if (292.5 < travelAngle && travelAngle <= 337.5)
		return  "southeast";
	if (337.5 < travelAngle && travelAngle < 360)
		return  "east";
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
