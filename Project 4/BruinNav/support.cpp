#include "support.h"

bool operator < (const GeoCoord& lhs, const GeoCoord& rhs)
{
	if (lhs.latitude < rhs.latitude)
		return true;
	else if (lhs.longitude < rhs.longitude)
		return true;
	return false;
}

bool operator > (const GeoCoord& lhs, const GeoCoord& rhs)
{
	return (rhs < lhs);
}

bool operator == (const GeoCoord& lhs, const GeoCoord& rhs)
{
	if ((lhs.latitude == rhs.latitude) && (lhs.longitude == rhs.longitude))
		return true;
	return false;
}

bool operator != (const GeoCoord& lhs, const GeoCoord& rhs)
{
	if (operator == (lhs, rhs))
		return false;
	return true;
}

bool operator == (const GeoSegment& lhs, const GeoSegment& rhs)
{
	if (lhs.start == rhs.start)
		if (lhs.end == rhs.end)
			return true;
	if (lhs.start == rhs.end)
		if (lhs.end == rhs.start)
			return true;
	return false;
}
bool operator != (const GeoSegment& lhs, const GeoSegment& rhs)
{
	if (lhs == rhs)
		return false;
	return true;
}

//this is backwards for priority queue use
bool operator < (const node& lhs, const node& rhs)
{
	if (lhs.f > rhs.f)
		return true;
	return false;
}