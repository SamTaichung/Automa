// Card.cpp : ��@��
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
double MMotionLib::GetPosition(MMotor* pMotor)			//�ثe��m
{
	return 0;
}
double MMotionLib::GetSpeed(MMotor* pMotor)				//�ثe�t��
{
	return 0;
}
void MMotionLib::SetPosition(MMotor* pMotor, double Pos)	//�]�w��m
{
	return;
}
bool MMotionLib::SetSVOn(MMotor* pMotor, bool isOn)		//�]�w�O�_�nServer On
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
WORD MMotionLib::GetStatus(MMotor* pMotor)
{
	return (0x6000);
}



// MCard �T���B�z�`��

