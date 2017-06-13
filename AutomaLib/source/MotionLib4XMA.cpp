#include "stdafx.h"
#include "MotionLib4XMA.h"
#include "Motor.h"
#include "SamNet.h"
#ifdef _M_X64
#pragma comment(lib, "SamNetDLL_X64D.lib") 
#else	
#pragma comment(lib, "SamNetDLL_W32D.lib") 
#endif
using namespace AutoLib;
MMotionLib4XMA::MMotionLib4XMA(MBase *pB, CString strID, CString strN):
MMotionLib(pB, strID, strN)
{

}
MMotionLib4XMA::~MMotionLib4XMA()
{

}
bool MMotionLib4XMA::Init(MMotor* pMotor)
{
	bool ret;
	if (_4XMA_Init(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID, pMotor->m_AxisID) != 0)
	{
		return false;
	}
	ret = _4XMA_set_inp(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID, pMotor->m_AxisID, pMotor->m_INPEnable, pMotor->m_INPLogic);
	ret = _4XMA_set_alm(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID, pMotor->m_AxisID, (pMotor->m_ALMLogic ? 1 : 0), 1);
	ret = _4XMA_set_el_logic(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID, pMotor->m_AxisID, (pMotor->m_PLimLogic ? 1 : 0));
	ret = _4XMA_set_emg_logic(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID, 0);
	ret = _4XMA_set_rst(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID, pMotor->m_AxisID, 1);
	if (ret == 0)
	{
		ret = _4XMA_set_pls_iptmode(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID, 
			pMotor->m_AxisID, pMotor->m_EncoderMode, pMotor->m_EncoderLogic);
		ret = _4XMA_set_pls_outmode(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID, 
			pMotor->m_AxisID, pMotor->m_PulseMode);
		ret = _4XMA_set_feedback_src(pMotor->m_CardID, pMotor->m_CardID, pMotor->m_StationID,
			pMotor->m_AxisID, pMotor->m_CountSource);
		if (ret == 0)
		{
			ret = _4XMA_set_home_config(pMotor->m_CardID, pMotor->m_CardID, pMotor->m_StationID,
				pMotor->m_AxisID, pMotor->m_HomeMode, pMotor->m_OrgLogic, pMotor->m_EZLogic, pMotor->m_EZCount, 0);
			if (ret == 0)
			{
				MMotionLib::Init();
			}
		}
	}	
	return true;
}
bool MMotionLib4XMA::isALM(MMotor *pMotor)
{
	U16 status;
	CString str;
	I16 ret;
	ret = _4XMA_get_io_status(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, &status);
	if (ret != 0) {
		str.Format(_T("_4XMA_get_io_status isALM return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return (bool(status & bit_ALM) ^ (!pMotor->m_ALMLogic));
}
bool MMotionLib4XMA::isEZ(MMotor *pMotor)
{
	U16 status;
	I16 ret;
	CString str;

	ret = _4XMA_get_io_status(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, &status);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_get_io_status isIndex return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return (bool(status & bit_EZ));
}
bool MMotionLib4XMA::isInp(MMotor *pMotor)
{
	U16 status;
	I16 ret;
	CString str;

	ret = _4XMA_get_io_status(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID,&status);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_get_io_status isInp return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return (bool(status & bit_INP));
}
bool MMotionLib4XMA::isMLim(MMotor* pMotor)
{
	U16 status;
	I16 ret;
	CString str;

	ret = _4XMA_get_io_status(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, &status);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_get_io_status isMLim return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return (bool(status & bit_MEL));
}
bool MMotionLib4XMA::isPLim(MMotor* pMotor)
{
	U16 status;
	I16 ret;
	CString str;

	ret = _4XMA_get_io_status(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, &status);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_get_io_status isPLim return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return (bool(status & bit_PEL));
}

bool MMotionLib4XMA::isMotion(MMotor* pMotor)
{
	I16 ret;
	CString str;

	ret = _4XMA_motion_done(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID);
	return (ret != 0);
}

bool MMotionLib4XMA::isOrg(MMotor* pMotor)
{
	U16 status;
	I16 ret;
	CString str;

	ret = _4XMA_get_io_status(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, &status);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_get_io_status isOrg return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return (bool(status & bit_ORG));
}

bool MMotionLib4XMA::isEMG(MMotor* pMotor)
{
	U16 status;
	I16 ret;
	CString str;

	ret = _4XMA_get_io_status(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, &status);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_get_io_status isEMG return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return (bool(status & bit_EMG));
}

bool MMotionLib4XMA::isSVOn(MMotor* pMotor)
{
	CString str;
	U16 status;
	I16 ret;

	ret = _4XMA_get_io_status(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, &status);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_get_io_status isSVOn return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return (bool(status & bit_SVON) ^ (!pMotor->m_SVOnLogic));
}
bool MMotionLib4XMA::GetSpeed(MMotor* pMotor,double *pSpeed)
{
	I16 ret;
	ret=_4XMA_get_current_speed(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, pSpeed);
	return(ret == 0);
}
bool MMotionLib4XMA::GetPosition(MMotor* pMotor, double *pPosition)
{
	I16 ret;
	//若m_CountSource=1.則將從EA,EB信號腳讀回位置信號(通常只能用在伺服馬達的Driver).
	ret = _4XMA_get_position(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, pPosition);
	return(ret == 0);
}
void MMotionLib4XMA::SetPosition(MMotor* pMotor, double Pos)
{
	I16 ret;
	ret = _4XMA_set_position(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID,Pos);
}
bool MMotionLib4XMA::Home(MMotor* pMotor,
	double dblStartSpeed, double dblAccTime,
	double dblMaxSpeed, double dblOffset)
{
	CString str;
	I16 ret;
	ret = _4XMA_home_search(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, dblStartSpeed, dblMaxSpeed, dblAccTime,dblOffset);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_home_search return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return(ret == 0);
}
bool MMotionLib4XMA::AMove(MMotor* pMotor,
	double dblStartSpeed, double dblAccTime,
	double dblMaxSpeed, double dblDesTime,double dblPos)
{
	CString str;
	I16 ret;
	ret = _4XMA_start_sa_move(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, dblPos,dblStartSpeed, dblMaxSpeed, dblAccTime, dblDesTime,
		dblMaxSpeed / 5, dblMaxSpeed / 5);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_start_sa_move return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return(ret == 0);
}
bool MMotionLib4XMA::RMove(MMotor* pMotor,
	double dblStartSpeed, double dblAccTime,
	double dblMaxSpeed, double dblDesTime, double dblDistance)
{
	CString str;
	I16 ret;
	ret = _4XMA_start_tr_move(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, dblDistance, dblStartSpeed, dblMaxSpeed, dblAccTime, dblDesTime);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_start_tr_move return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return(ret == 0);
}
bool MMotionLib4XMA::VMove(MMotor* pMotor,
	double dblStartSpeed, double dblAccTime,
	double dblMaxSpeed)
{
	CString str;
	I16 ret;
	ret = _4XMA_tv_move(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID,dblStartSpeed, dblMaxSpeed, dblAccTime);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_start_tv_move return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return(ret == 0);
}

bool MMotionLib4XMA::Stop(MMotor* pMotor,double dblDesTime)
{
	CString str;
	I16 ret;
	ret = _4XMA_sd_stop(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, dblDesTime);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_sd_stop return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return(ret == 0);
}
bool MMotionLib4XMA::EStop(MMotor* pMotor)
{
	CString str;
	I16 ret;
	ret = _4XMA_emg_stop(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID);
	if (ret != 0)
	{
		str.Format(_T("_4XMA_emg_stop return error(%d)"), ret);
		ShowMessage(this, str);
	}
	return(ret == 0);
}
void MMotionLib4XMA::ResetALM(MMotor* pMotor,bool bValue)
{
	I16 ret;
	ret = _4XMA_set_rst(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, 0);
	::Sleep(100);
	ret = _4XMA_set_rst(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID, 1);
	return;
}

WORD MMotionLib4XMA::GetIOStatus(MMotor* pMotor)
{
	I16 ret; 
	U16 sts;
	ret = _4XMA_get_io_status(pMotor->m_CardID, pMotor->m_ConnectID, pMotor->m_StationID,
		pMotor->m_AxisID,&sts);
	return sts;
}

