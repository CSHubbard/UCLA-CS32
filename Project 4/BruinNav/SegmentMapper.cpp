#include "provided.h"
#include <vector>
#include "MyMap.h"
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;

private:
	MyMap<GeoCoord, vector<StreetSegment>> m_GeoMap;
	bool checkForDuplicates(vector<StreetSegment> *vec, StreetSegment seg) const;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

///N*A*log(N) + 6*log(N)
void SegmentMapperImpl::init(const MapLoader& ml)
{
	vector<StreetSegment> *streetVecPtr, streetVec;
	///N times N is number of segments in map
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment tempSeg;
		ml.getSegment(i, tempSeg);
	
		streetVecPtr = m_GeoMap.find(tempSeg.segment.start);
		if (streetVecPtr != nullptr)
		{
			if (!checkForDuplicates(streetVecPtr, tempSeg))
				streetVecPtr->push_back(tempSeg);
		}
		else
		{
			m_GeoMap.associate(tempSeg.segment.start, streetVec);	///2*log(N)
			streetVecPtr = m_GeoMap.find(tempSeg.segment.start);	///log(N)
			streetVecPtr->push_back(tempSeg);
		}

		streetVecPtr = m_GeoMap.find(tempSeg.segment.end);
		if (streetVecPtr != nullptr)
		{
			if (!checkForDuplicates(streetVecPtr, tempSeg))
				streetVecPtr->push_back(tempSeg);
		}
		else
		{
			m_GeoMap.associate(tempSeg.segment.end, streetVec);	///2*log(N)
			streetVecPtr = m_GeoMap.find(tempSeg.segment.end);	///log(N)
			streetVecPtr->push_back(tempSeg);
		}
																
		for (int j = 0; j < tempSeg.attractions.size(); j++)	///O(A) A is number of total attractions
		{
			streetVecPtr = m_GeoMap.find(tempSeg.attractions[j].geocoordinates); ///log(N)
			if (streetVecPtr != nullptr)
			{
				if (!checkForDuplicates(streetVecPtr, tempSeg))
				streetVecPtr->push_back(tempSeg);
			}
			else
			{ 
				m_GeoMap.associate(tempSeg.attractions[j].geocoordinates, streetVec);	///2*log(N)
				streetVecPtr = m_GeoMap.find(tempSeg.attractions[j].geocoordinates);	///log(N)
				streetVecPtr->push_back(tempSeg);
			}
				
		}
	}
}

///2*log(N) (average)
vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> const *tempStreetPtr, emptyVec;
	tempStreetPtr = m_GeoMap.find(gc);
	if (tempStreetPtr != nullptr)
		return *(m_GeoMap.find(gc));
	return emptyVec;
}

///Constant number of times, negligible
//checks if the segment is already counted at that location, if it is found return true, else false
bool SegmentMapperImpl::checkForDuplicates(vector<StreetSegment> *vec, StreetSegment seg) const
{
	for (int i = 0; i < vec->size(); i++)
	{
		if ((vec->at(i).segment.start == seg.segment.start)
			&& (vec->at(i).segment.end == seg.segment.end))
		{
			return true;
		}
	}
	return false;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
