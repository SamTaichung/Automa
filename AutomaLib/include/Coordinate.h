#pragma once
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
namespace AutoLib {
	AUTOMALIB_API class DCoordinate
	{
	public:
		AUTOMALIB_API DCoordinate(double px=0,double py=0,double pz=0,double ps=0):x(px),y(py),z(pz),s(ps){};
		AUTOMALIB_API ~DCoordinate(void);
		AUTOMALIB_API DCoordinate operator+(const DCoordinate &p);
		AUTOMALIB_API DCoordinate operator*(const DCoordinate &p);
		AUTOMALIB_API DCoordinate operator/(const DCoordinate &p);
		AUTOMALIB_API DCoordinate operator+=(const DCoordinate &p);
		AUTOMALIB_API DCoordinate operator-(const DCoordinate &p);
		AUTOMALIB_API DCoordinate operator-=(const DCoordinate &p);
		AUTOMALIB_API void Zero(){x=0;y=0;s=0;z=0;};
		double x,y,z,s;
	};
}//namespace MacLib