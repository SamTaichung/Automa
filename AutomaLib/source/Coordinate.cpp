#include "stdafx.h"
#include "coordinate.h"

using namespace AutoLib;

DCoordinate::~DCoordinate(void)
{
};
DCoordinate	DCoordinate::operator+(const DCoordinate &p)
{
	return (DCoordinate(x+p.x,y+p.y,z+p.z,s+p.s)) ;
};
DCoordinate	DCoordinate::operator*(const DCoordinate &p)
{
	return (DCoordinate(x*p.x,y*p.y,z*p.z,s*p.s)) ;
};
DCoordinate	DCoordinate::operator/(const DCoordinate &p)
{
	return (DCoordinate(x/p.x,y/p.y,z/p.z,s/p.s)) ;
};
DCoordinate	DCoordinate::operator+=(const DCoordinate &p)
{
	return (DCoordinate(x+=p.x,y+=p.y,z+=p.z,s+=p.s)) ;
};
DCoordinate	DCoordinate::operator-(const DCoordinate &p)
{
	return (DCoordinate(x-p.x,y-p.y,z-p.z,s-p.s)) ;
};
DCoordinate	DCoordinate::operator-=(const DCoordinate &p)
{
	return (DCoordinate(x-=p.x,y-=p.y,z-=p.z,s-=p.s)) ;
};