// Card.cpp : 實作檔
//

#include "stdafx.h"
#include "MotionLib.h"


using namespace AutoLib;

MMotionLib::MMotionLib(MBase *pB,CString strID,CString strName):
	MBase(pB,strID,strName)
{
}
MMotionLib::~MMotionLib()
{
}
bool MMotionLib::isMotion(MMotor* pMotor)
{
	return false;
}
bool MMotionLib::isHoming(MMotor* pMotor)
{
	return false;
}
bool MMotionLib::isInp(MMotor* pMotor)
{
	return true;
}
bool MMotionLib::isOrg(MMotor* pMotor)
{
	return false;
}
bool MMotionLib::isEMG(MMotor* pMotor)
{
	return false;
}
bool MMotionLib::isEZ(MMotor* pMotor)
{
	return false;
}
bool MMotionLib::isPLim(MMotor* pMotor)
{
	return false;
}
bool MMotionLib::isMLim(MMotor* pMotor)
{
	return false;
}
bool MMotionLib::isSVOn(MMotor* pMotor)
{
	return true;
}
bool MMotionLib::isALM(MMotor* pMotor)
{
	return false;
}

void MMotionLib::SetPosition(MMotor* pMotor, double Pos)	//設定位置
{
	return;
}
bool MMotionLib::SetSVOn(MMotor* pMotor, bool isOn)		//設定是否要Server On
{
	return true;
}
int MMotionLib::GetALMNo(MMotor* pMotor)
{
	return 0;
}
void MMotionLib::ResetALM(MMotor* pMotor)
{
	return ;
}
WORD MMotionLib::GetIOStatus(MMotor* pMotor)
{
	return (0x6000);
}
int MMotionLib::GetStatusCode(MMotor* pMotor)
{
	return 0;
}

// MCard 訊息處理常式

