#include "StdAfx.h"
#include "DLine.h"
#define PI (3.1415926)
using namespace AutoLib;
DLine::DLine(void)
{
}

DLine::~DLine(void)
{
}
double DLine::GetAtan()
{
	double dblRet;
	if(PA.x==PB.x)
	{
		dblRet = ((PB.y - PA.y) > 0 ? 1:-1) * PI / 2;
	}else{
		if (PB.y == PA.y)
		{
			if (PB.x - PA.x > 0)
			{
				dblRet =  PI;
			}else{
				dblRet =  -PI;
			}
		}else{
			dblRet = atan((PB.y - PA.y) / (PB.x - PA.x)) + ((PB.x-PA.x>0)?0:PI);
		}
	}
	return (dblRet/PI*180);
}
DLine DLine::Rotation(const double dblS)
{
	DLine dlRet; 
	double dblR,dX,dY,dblCx,dblCy,dblOS;
    dblCx = GetCenter().x;
    dblCy = GetCenter().y;
    dX = PB.x - dblCx;
    dY = PB.y - dblCy;
    if	(dX != 0)
	{
		dblOS = atan(dY / dX) + (dX < 0? PI:0);
	}else{
		if (dY > 0)
		{
			dblOS =  PI/2;
		}else{
			dblOS = -PI/2;
		}
	}
    dblOS = dblOS + (dblS / 180 * PI);
    dblR =sqrt((dX *dX) + (dY*dY));
    dX = dblR * cos(dblOS);
    dY = dblR * sin(dblOS);
    dlRet.PB.x = dblCx + dX;
    dlRet.PB.y = dblCy + dY;
    dlRet.PA.x = dblCx - dX;
    dlRet.PA.y = dblCy - dY;
	return dlRet;
}

// 取得 直線PA,PB 的 斜率.(若是垂直,則m_LineVertical = true)
double DLine::GetLineSlope()
{
	if (PA.x == PB.x)	// 線(PA,PB)是垂直(PA.x==PB.x)
	{
		m_LineVertical = true;
		return 999999999;
	} else {
		m_LineVertical = false;
		return ( (PA.y-PB.y) / (PA.x-PB.x) );
	};
}

// 取得 中垂線 的 斜率.(若是垂直,則m_MiddleLineVertical = true)
double DLine::GetMidLineSlope() 
{
	if (PA.y == PB.y)	// 線(PA,PB)是水平(PA.y==PB.y), 則其中垂線垂直.
	{
		m_MiddleLineVertical = true;
		return 999999999;
	} else {
		m_MiddleLineVertical = false;
		return ( (PB.x-PA.x) / (PA.y-PB.y) );
	};
}

DCoordinate DLine::CalOffset(DLine dlTarget,DCoordinate dcRotationCenter)
{
	DCoordinate dcRet;
	DLine dlTmp;
	double dblSita1,dblSita2,dblR,dblTx,dblTy;
	dcRet.x=0;
	dcRet.y=0;
	dblSita1=dlTarget.GetAtan();
    dblSita2=GetAtan();
	dcRet.s=(dblSita2-dblSita1)*PI/180;
	//-------------------------------------------------------------------------------------
	dlTmp.PA=GetCenter();
	dlTmp.PB=dcRotationCenter;

	dblSita2 = dlTmp.GetAtan()*PI/180;
    dblR = dlTmp.GetLength();
	while(dcRet.s>(PI/2) || dcRet.s<(-PI/2))
	{
		if (dcRet.s>(PI/2))
		{
			dcRet.s=dcRet.s-PI;
		}else{
			if (dcRet.s<(-PI/2))
			{
				dcRet.s=dcRet.s+PI;
			}
		}
	}	

	dblTx = dcRotationCenter.x - (dblR * cos(dblSita2 - dcRet.s));
	dblTy = dcRotationCenter.y - (dblR * sin(dblSita2 - dcRet.s));
	dcRet.x=dlTarget.GetCenter().x-dblTx;
	dcRet.y=dlTarget.GetCenter().y-dblTy;
	dcRet.s=dcRet.s*180/PI;
	return dcRet;
}