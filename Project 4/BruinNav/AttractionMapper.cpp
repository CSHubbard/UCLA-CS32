#include "provided.h"
#include <string>
#include "MyMap.h"
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;

private: 
	MyMap<string, Attraction> m_AttrMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

///N+A*log(N)
void AttractionMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < ml.getNumSegments(); i++) ///O(N), N is number of segments in map
	{
		StreetSegment tempSeg;
		ml.getSegment(i, tempSeg);

		//string streetTempString = tempSeg.streetName;
		//for (int i = 0; i < streetTempString.size(); i++)
		//{
		//	streetTempString[i] = tolower(streetTempString[i]);
		//}
		//m_AttrMap.associate(streetTempString, tempSeg); ///log(N)

		for (int j = 0; j < tempSeg.attractions.size(); j++) ///O(A), A is number of attractions in the segment
		{
			string tempString = tempSeg.attractions[j].name;
			for (int i = 0; i < tempString.size(); i++)
			{
				tempString[i] = tolower(tempString[i]);
			}
			m_AttrMap.associate(tempString, tempSeg.attractions[j]); ///log(N)
		}
	}
}

///(log(N) where N is the number of attr in map
bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	string tempString = attraction;
	for (int i = 0; i < tempString.size(); i++) ///O(s) s is size of string (constant& negligible)
	{
		tempString[i] = tolower(tempString[i]);
	}
	const Attraction *tempAttr = m_AttrMap.find(tempString); ///log(N) N number of attractions
	if (tempAttr == nullptr)
		return false;
	gc = tempAttr->geocoordinates;
	return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
