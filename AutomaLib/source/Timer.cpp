// Timer.cpp : 實作檔
//
#include "stdafx.h"
#include "Timer.h"

using namespace AutoLib;

MTimer::MTimer(MBase *pB,CString strID,CString strName,double dblTime):
	MBase(pB,strID,strName)
{
	m_bTimerStart=false;
	m_dblInterval=dblTime;
}

MTimer::~MTimer()
{
}
void MTimer::SetInterval(double dblInterval)
{
	m_dblInterval = dblInterval;
}		//設定Timer時間
double MTimer::GetInterval()		//設定Timer時間
{
	return m_dblInterval;
}
void MTimer::Cycle(const double dblTime)
{
	if (m_bTimerStart){
		dblTimeCount+=dblTime;	
		if (dblTimeCount>=m_dblInterval){
			m_bTimerStart=false;
		}
	}
}
double MTimer::GetRemanderTime()
{
	if (m_bTimerStart){
		if (dblTimeCount<m_dblInterval){
			return m_dblInterval-dblTimeCount;
		}
	}
	return 0;
}
void MTimer::Start()
{
	m_bTimerStart=true;
	dblTimeCount=0;
}
void MTimer::Stop()
{
	m_bTimerStart=false;
	MBase::Stop();
}
void MTimer::EStop(const bool isES)
{
	m_bTimerStart=false;
	MBase::EStop(isES);
}
void MTimer::LoadMachineData(CADOConnection * pC, bool bAllChildsLoad)
{
	CADORecordset rsTmp;
	CString strSQL;
	_variant_t id,v;
	if (pC->GetTableIndex(_T("Timers"))<0)
	{
		strSQL = _T("Create Table Timers(ID nChar(10),");
		strSQL += _T("[Interval] float not null default 0,");
		strSQL += _T("Constraint PKTimers Primary Key(ID))");
		pC->BeginTrans();
		pC->ExecuteSQL(strSQL);
		pC->CommitTrans();
	}
	if (rsTmp.Open(_T("Select * From Timers Where ID='") +m_ID+_T("'"),pC))
	{
		if (rsTmp.isEOF()){
			pC->BeginTrans();
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"),m_ID);
			rsTmp.SetValue(_T("Interval"), m_dblInterval);
			rsTmp.Update();
			pC->CommitTrans();
		}else{
			rsTmp.GetValue(_T("Interval"),m_dblInterval);
		}
		rsTmp.Close();
	}
	MBase::LoadMachineData(pC, bAllChildsLoad);
}
void MTimer::SaveMachineData(CADOConnection * pC, bool bAllChildsSave)
{
	CADORecordset rsTmp;
	_variant_t id,v;
	if (rsTmp.Open(_T("Select * From Timers Where ID='") +m_ID+_T("'"),pC))
	{
		pC->BeginTrans();	
		if (rsTmp.isEOF()){
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"),m_ID);
		}
		rsTmp.SetValue(_T("Interval"),m_dblInterval);
		rsTmp.Update();
		pC->CommitTrans();
		rsTmp.Close();
	}
	MBase::SaveMachineData(pC, bAllChildsSave);
}
bool MTimer::isTimeOut(void)
{
	return(!m_bTimerStart);
}


