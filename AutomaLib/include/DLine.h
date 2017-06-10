#pragma once
#include "Coordinate.h"
#include <Math.h>
namespace AutoLib {
class DLine
{
public:
	DLine(void);
	~DLine(void);
	DCoordinate PA,PB;
	double GetAtan();
	DLine Rotation(const double dblS);
	double GetLength(){return sqrt((PA.x-PB.x)*(PA.x-PB.x)+(PA.y-PB.y)*(PA.y-PB.y));};
	DCoordinate GetCenter(){return DCoordinate((PA.x+PB.x)/2,(PA.y+PB.y)/2,0,0);};
	double GetLineSlope();
	double GetMidLineSlope();
	bool IsMidLineSlopeInfinite(){return (m_MiddleLineVertical);};
	bool IsLineSlopeInfinite(){return (m_LineVertical);};
	DCoordinate CalOffset(DLine dlTarget,DCoordinate dcRotationCenter);
private:
	bool m_LineVertical;	// 線PA,PB垂直.
	bool m_MiddleLineVertical;	// 中垂線垂直.
};
}//namespace MacLib