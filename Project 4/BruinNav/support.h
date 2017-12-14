#ifndef SUPPORT_H
#define SUPPORT_H
#include "provided.h"

bool operator < (const GeoCoord& lhs, const GeoCoord& rhs);
bool operator > (const GeoCoord& lhs, const GeoCoord& rhs);
bool operator == (const GeoCoord& lhs, const GeoCoord& rhs);
bool operator != (const GeoCoord& lhs, const GeoCoord& rhs);

bool operator == (const GeoSegment& lhs, const GeoSegment& rhs);
bool operator != (const GeoSegment& lhs, const GeoSegment& rhs);

struct node {
	node *parent;
	GeoCoord pos;
	double f, g, h;
};

//this is backwards for priority queue use
bool operator < (const node& lhs, const node& rhs);


#endif // !SUPPORT_H
