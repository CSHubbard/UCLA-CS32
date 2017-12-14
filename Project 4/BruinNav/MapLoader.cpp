#include "provided.h"
#include <string>
#include <fstream>
#include <vector>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	ifstream m_reader;
	vector <StreetSegment> my_Streets;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
	m_reader.close();
}

bool MapLoaderImpl::load(string mapFile)
{
	m_reader.open(mapFile);
	if (!m_reader)
		return false;
	
	string line;
	int numAttr;
	while (getline(m_reader, line))
	{
		//sets street name
		StreetSegment tempStreet;
		tempStreet.streetName = line;

		//unefficently sets street's geosegment
		getline(m_reader, line);
		line.erase(line.find(','),1);
		line[line.find(',')] = ' ';
		string latlong[4]; //lat1, long1, lat2, long2
		int j = 0; //index to access latlong[]
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] == ' ')
			{
				j++; i++;
			}
			latlong[j] += line[i];
		}
		GeoCoord tempSCor(latlong[0], latlong[1]);
		GeoCoord tempECor(latlong[2], latlong[3]);
		GeoSegment tempSeg(tempSCor, tempECor);
		tempStreet.segment = tempSeg;

		//sets street attractions
		m_reader >> numAttr;
		m_reader.ignore(10000, '\n');
		for (int k = 0; k < numAttr; k++)
		{
			Attraction tempAttr;
			getline(m_reader, line);
			tempAttr.name = line.substr(0, line.find('|'));
			line = line.substr(line.find('|')+1);
			line.erase(line.find(','), 1);

			string attLatLong[2];
			int h = 0; //index to access latlong[]
			for (int i = 0; i < line.size(); i++)
			{
				if (line[i] == ' ')
				{
					h++; i++;
				}
				attLatLong[h] += line[i];
				
			}
			GeoCoord tempAttCor(attLatLong[0], attLatLong[1]);
			tempAttr.geocoordinates = tempAttCor;
			tempStreet.attractions.push_back(tempAttr);
		}

		my_Streets.push_back(tempStreet);
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return my_Streets.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if ((segNum < 0) || (segNum > my_Streets.size() - 1))
		return false;
	seg = my_Streets[segNum];
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
