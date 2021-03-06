#include "stdafx.h"
#include "MotionLibVirtual.h"
#include "Motor.h"
using namespace AutoLib;
MMotionLibVirtual::MMotionLibVirtual(MBase *pB, CString strID, CString strN, int AxisCount):
MMotionLib(pB, strID, strN)
{
	m_intMotorInitCount = 0;
	for (int i = 0; i < AxisCount; i++)
	{
		VirtualMotorData *pVMD;
		pVMD = new VirtualMotorData();
		pVMD->m_dblCalSpeed = 0;
		pVMD->m_dblLastDist = 0;
		pVMD->m_dblTargetPos = 0;
		pVMD->m_dblPos = 0;
		m_AxisArray.Add(pVMD);
	}
}
MMotionLibVirtual::~MMotionLibVirtual()
{
	for (int i = 0; i < m_AxisArray.GetCount(); i++)
	{
		delete m_AxisArray[i];
	}
}
int MMotionLibVirtual::GetMotorIndex(MMotor* pMotor)
{
	int ret;
	map<MMotor*, int>::iterator itr;
	itr = m_mapMotorIndex.find(pMotor);
	if (itr == m_mapMotorIndex.end())
	{
		ret = -1;
	}else{
		ret = (*itr).second;
	}
	return ret;
}
bool MMotionLibVirtual::Init(MMotor* pMotor)
{
	m_mapMotorIndex.insert(map<MMotor*, int>::value_type(pMotor,m_intMotorInitCount));
	m_AxisArray[m_intMotorInitCount]->m_mvStep = stpDone;
	m_intMotorInitCount++;
	return true;
}
bool MMotionLibVirtual::isMotion(MMotor* pMotor)
{
	return (m_AxisArray[GetMotorIndex(pMotor)]->m_mvStep != Step::stpDone);
}
bool MMotionLibVirtual::GetSpeed(MMotor* pMotor, double *pSpeed)
{
	*pSpeed= m_AxisArray[GetMotorIndex(pMotor)]->m_dblCalSpeed;
	return true;
}
bool MMotionLibVirtual::GetPosition(MMotor* pMotor, double *pPos)
{
	*pPos = m_AxisArray[GetMotorIndex(pMotor)]->m_dblPos;
	return true;
}
void MMotionLibVirtual::Cycle(const double dblTime)
{
	double dblPitch, dblDecTime,dblAVGSpeed;
	MMotionLib::Cycle(dblTime);

	for (int AxisID = 0; AxisID < m_AxisArray.GetCount(); AxisID++)
	{
		VirtualMotorData *pVMD = m_AxisArray[AxisID];
		if (pVMD->m_mvStep != Step::stpDone)
		{
			pVMD->m_dblLastDist = fabs(pVMD->m_dblTargetPos - pVMD->m_dblPos);
			switch (pVMD->m_mvStep) {
			case Step::stpAcc:
			{
				//----------------目前的速度所需要的減速時間----------------
				if (pVMD->m_dblCalSpeed > 0)
				{
					dblDecTime = fabs((pVMD->m_dblCalSpeed-pVMD->m_dblStartSpeed)*pVMD->m_dblDesTime/(pVMD->m_dblMaxSpeed-pVMD->m_dblStartSpeed));
					if (dblDecTime > pVMD->m_dblDesTime) //目前剩餘距離要減速不夠
					{
						pVMD->m_mvStep = Step::stpDec;
						break;
					}
				}
				if (pVMD->m_dblCalSpeed < pVMD->m_dblMaxSpeed)
				{
					pVMD->m_dblCalSpeed = pVMD->m_dblCalSpeed +
						(fabs(pVMD->m_dblMaxSpeed -
							pVMD->m_dblStartSpeed)*dblTime /
							pVMD->m_dblAccTime);
					if (pVMD->m_dblCalSpeed > pVMD->m_dblMaxSpeed)
					{
						pVMD->m_dblCalSpeed = pVMD->m_dblMaxSpeed;
						pVMD->m_mvStep = Step::stpHiSpeed;
					}
				}
			}
			break;
			case Step::stpHiSpeed:
			{
				//dblDecTime = pVMD->m_dblLastDist * 2 /
				//	fabs(pVMD->m_dblStartSpeed + pVMD->m_dblCalSpeed);
	  			 dblDecTime = fabs((pVMD->m_dblCalSpeed - pVMD->m_dblStartSpeed)*pVMD->m_dblDesTime / (pVMD->m_dblMaxSpeed - pVMD->m_dblStartSpeed));

				if (dblDecTime < pVMD->m_dblDesTime) //目前剩餘距離要減速不夠
				{
					pVMD->m_mvStep = Step::stpDec;
				}
			}
			break;
			case Step::stpDec:
			{
				if (pVMD->m_dblCalSpeed > pVMD->m_dblStartSpeed)
				{
					pVMD->m_dblCalSpeed = pVMD->m_dblCalSpeed -
						(fabs(pVMD->m_dblMaxSpeed -
							pVMD->m_dblStartSpeed)*dblTime /
							pVMD->m_dblDesTime);
					if (pVMD->m_dblCalSpeed < pVMD->m_dblStartSpeed)
					{
						pVMD->m_dblCalSpeed = pVMD->m_dblStartSpeed;
					}
				}
			}
			break;
			}

			if (pVMD->m_dblCalSpeed < 0.0001)
			{
				pVMD->m_dblCalSpeed = 0.0001;
			}
			dblPitch = dblTime*fabs(pVMD->m_dblCalSpeed);
			if (dblPitch >= fabs(pVMD->m_dblTargetPos - pVMD->m_dblPos))
			{
				dblPitch = fabs(pVMD->m_dblTargetPos - pVMD->m_dblPos);
				pVMD->m_dblCalSpeed = 0;
				pVMD->m_mvStep = Step::stpDone;
			}
			if (pVMD->m_dblTargetPos > pVMD->m_dblPos) {
				pVMD->m_dblPos = pVMD->m_dblPos + dblPitch;
			}
			else {
				pVMD->m_dblPos = pVMD->m_dblPos - dblPitch;
			}
			pVMD->m_dblLastDist = fabs(pVMD->m_dblTargetPos - pVMD->m_dblPos);
		}
	}
}
bool MMotionLibVirtual::AMove(MMotor* pMotor,
	double dblStartSpeed, double dblAccTime,
	double dblMaxSpeed, double dblDesTime,double dblPos)
{
	if (m_AxisArray[GetMotorIndex(pMotor)]->m_mvStep == Step::stpDone)
	{
		m_AxisArray[GetMotorIndex(pMotor)]->m_dblAccTime = dblAccTime;
		if (dblAccTime < 0.001)
		{
			dblAccTime = 0.001;
		}		
		if (dblDesTime < 0.001)
		{
			dblDesTime = 0.001;
		}
		m_AxisArray[GetMotorIndex(pMotor)]->m_dblStartSpeed = dblStartSpeed/1000;
		m_AxisArray[GetMotorIndex(pMotor)]->m_dblAccTime = dblAccTime*1000;
		m_AxisArray[GetMotorIndex(pMotor)]->m_dblDesTime = dblDesTime*1000;
		m_AxisArray[GetMotorIndex(pMotor)]->m_dblMaxSpeed = dblMaxSpeed/1000;

		m_AxisArray[GetMotorIndex(pMotor)]->m_dblTargetPos = dblPos;
		m_AxisArray[GetMotorIndex(pMotor)]->m_mvStep = Step::stpAcc;
		return true;
	}
	return false;
}
bool MMotionLibVirtual::RMove(MMotor* pMotor,
	double dblStartSpeed, double dblAccTime,
	double dblMaxSpeed, double dblDesTime, double dblMove)
{
	if (m_AxisArray[GetMotorIndex(pMotor)]->m_mvStep == Step::stpDone)
	{
		m_AxisArray[GetMotorIndex(pMotor)]->m_dblStartSpeed = dblStartSpeed/1000;
		m_AxisArray[GetMotorIndex(pMotor)]->m_dblAccTime = dblAccTime*1000;
		m_AxisArray[GetMotorIndex(pMotor)]->m_dblDesTime = dblDesTime*1000;
		m_AxisArray[GetMotorIndex(pMotor)]->m_dblMaxSpeed = dblMaxSpeed/1000;
		m_AxisArray[GetMotorIndex(pMotor)]->m_dblTargetPos = dblMove + m_AxisArray[GetMotorIndex(pMotor)]->m_dblTargetPos;
		m_AxisArray[GetMotorIndex(pMotor)]->m_mvStep = Step::stpAcc;
		return true;
	}
	return false;
}
bool MMotionLibVirtual::Stop(MMotor* pMotor, double dblDesTime)
{
	m_AxisArray[GetMotorIndex(pMotor)]->m_mvStep = Step::stpDone;
	m_AxisArray[GetMotorIndex(pMotor)]->m_dblTargetPos = m_AxisArray[GetMotorIndex(pMotor)]->m_dblPos;
	m_AxisArray[GetMotorIndex(pMotor)]->m_dblLastDist = 0;
	return true;
}
bool MMotionLibVirtual::EStop(MMotor* pMotor)
{
	return Stop(pMotor,0);
}
bool MMotionLibVirtual::Home(MMotor* pMotor,
	double dblStartSpeed, double dblAccTime,
	double dblMaxSpeed, double dblOffset)
{
	m_AxisArray[GetMotorIndex(pMotor)]->m_mvStep = Step::stpDone;
	m_AxisArray[GetMotorIndex(pMotor)]->m_dblPos = 0;
	m_AxisArray[GetMotorIndex(pMotor)]->m_dblTargetPos = 0;
	m_AxisArray[GetMotorIndex(pMotor)]->m_dblLastDist = 0;
	return true;
}


