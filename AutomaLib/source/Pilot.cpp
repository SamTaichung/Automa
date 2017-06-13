// Pilot.cpp : 實作檔
//

#include "stdafx.h"
#include "Pilot.h"


// MPilot 三色燈及蜂鳴器之狀態控制
using namespace AutoLib;

IMPLEMENT_DYNAMIC(MPilot, MBase)
MPilot::MPilot(MBase *pB, CString strID, CString strName):
	MBase(pB,strID,strName)
{
	m_pFHGreen=new MFlash(this, strID +_T("G"),_T("Pilot Green Light"));
	AddChild(m_pFHGreen);
	m_pFHRed=new MFlash(this, strID + _T("R"), _T("Pilot Red Light"));
	AddChild(m_pFHRed);
	m_pFHYellow=new MFlash(this, strID + _T("Y"), _T("Pilot Yello Light"));
	AddChild(m_pFHYellow);
	m_pPSBuzz=new MPulseIO(this, strID + _T("B"), _T("Buzz"));
	AddChild(m_pPSBuzz);
	m_OldState = PilotState::UNKNOW;
	m_PilotState = PilotState::UNKNOW;
}

MPilot::~MPilot()
{
}
CString MPilot::GetStateName(PilotState ps)
{
	CString strName[PilotState::UNKNOW] =
	{
		_T("INITIAL"),
		_T("IDLE"),
		_T("HOMING"),
		_T("PAUSE_STOP"),
		_T("ERROR_ON_AUTO"),
		_T("ERROR_ON_HOMING"),
		_T("ERROR_ON_MANUAL"),
		_T("EMERGENCY_STOP"),
		_T("AUTOMATIC_RUN"),
		_T("CYCLE_STOP"),
		_T("DRY_RUN"),
		_T("MAINTENANCE_RUN"),
		_T("MANUAL_RUN")
	};
	CString strRet;
	strRet = strName[ps];
	return strRet;
}
int MPilot::GetRLightSet(PilotState ps)
{
	return m_RLight[ps];
}
int MPilot::GetGLightSet(PilotState ps)
{
	return m_GLight[ps];
}
int MPilot::GetYLightSet(PilotState ps)
{
	return m_YLight[ps];
}
int MPilot::GetBuzzSet(PilotState ps)
{
	return m_Buzz[ps];
}
void MPilot::SetRLightSet(PilotState ps, int intSV)
{
	m_RLight[ps] = intSV;
}
void MPilot::SetGLightSet(PilotState ps, int intSV)
{
	m_GLight[ps] = intSV;
}
void MPilot::SetYLightSet(PilotState ps, int intSV)
{
	m_YLight[ps] = intSV;
}
void MPilot::SetBuzzSet(PilotState ps, int intSV)
{
	m_Buzz[ps] = intSV;
}
void MPilot::SetPrevState()
{
	SetState(m_OldState);
}
MPilot::PilotState MPilot::GetState()
{
	return m_PilotState;
}
CTime MPilot::GetStateStartTime()
{
	return m_ctStateStartTime;
}
void MPilot::SetState(PilotState ps)
{
	if (m_PilotState != ps)
	{
		m_ctStateStartTime = CTime::GetCurrentTime();
		m_OldState=m_PilotState;
		m_PilotState=ps;
	}
	if (m_GLight[ps]==0){
		m_pFHGreen->DoOff();
	}else{
		if (m_GLight[ps]>0){
			m_pFHGreen->DoBlink();
		}else{
			m_pFHGreen->DoOn();
		}
	}
	if (m_RLight[ps]==0){
		m_pFHRed->DoOff();
	}else{
		if (m_RLight[ps]>0){
			m_pFHRed->DoBlink();
		}else{
			m_pFHRed->DoOn();
		}
	}
	if (m_YLight[ps]==0){
		m_pFHYellow->DoOff();
	}else{
		if (m_YLight[ps]>0){
			m_pFHYellow->DoBlink();
		}else{
			m_pFHYellow->DoOn();
		}
	}
	if (m_Buzz[ps]==0){
		m_pPSBuzz->SetIOValue(false);
	}else{
		if (m_Buzz[ps]>0){
			m_pPSBuzz->m_pOnTimer->SetInterval(m_Buzz[ps]);
			m_pPSBuzz->Pulse();
		}else{
			m_pPSBuzz->SetIOValue(true);
		}
	}
}
void MPilot::LoadMachineData(CADOConnection *pC, bool bAllChildsLoad)
{
	CADORecordset rsTmp;
	CString strSQL;
	int v;
	if (pC->GetTableIndex(_T("Pilot"))<0)
	{
		strSQL = _T("Create Table Pilot(ID int,CName nChar(40),EName nChar(40),");
		strSQL += _T("RLight int not null default 0,");
		strSQL += _T("GLight int not null default 0,");
		strSQL += _T("YLight int not null default 0,");
		strSQL += _T("Buzz int not null default 0,");
		strSQL += _T("Constraint PKPilot Primary Key(ID))");
		pC->BeginTrans();
		pC->ExecuteSQL(strSQL);

		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::INITIAL, _T("機臺初始化"), _T("Initial"), 1, 0, 1, 2);
		pC->ExecuteSQL(strSQL);
		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::IDLE, _T("待機停止"), _T("Cycle stop"), 0, 0, 1, 0);
		pC->ExecuteSQL(strSQL);

		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::HOMING, _T("機臺復歸中"), _T("Home run"), 0, 0, 0, 0);
		pC->ExecuteSQL(strSQL);

		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::PAUSE_STOP, _T("動作暫停"), _T("Pause"), 0, 0, 1, 0);
		pC->ExecuteSQL(strSQL);

		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::ERROR_ON_AUTO, _T("自動中異常"), _T("Error on auto"), 1, 0, 1, 2);
		pC->ExecuteSQL(strSQL);

		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::ERROR_ON_HOMING, _T("復歸中異常"), _T("Error on homing"), 1, 0, 1, 2);
		pC->ExecuteSQL(strSQL);

		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::ERROR_ON_MANUAL, _T("手動中異常"), _T("Error on manual"), 1, 0, 1, 2);
		pC->ExecuteSQL(strSQL);

		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::EMERGENCY_STOP,_T("緊急停止"),_T("Emergency stop"), 1, 0, 0, 1);
		pC->ExecuteSQL(strSQL);

		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::AUTOMATIC_RUN, _T("自動生產"), _T("Automatic run"), 0, 1, 0, 0);
		pC->ExecuteSQL(strSQL);
		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::CYCLE_STOP,_T("生產停止"),_T("Cycle stop"), 0, 0, 1, 0);
		pC->ExecuteSQL(strSQL);
		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::DRY_RUN,_T("空跑開始"),_T("Dry run"), 0, 1, 0, 0);
		pC->ExecuteSQL(strSQL);
		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::MAINTENANCE_RUN,_T("維護中"),_T("Maintenance run"), 0, 1, 1, 0);
		pC->ExecuteSQL(strSQL);
		strSQL.Format(_T("Insert into Pilot(ID,CName,EName,Rlight,GLight,YLight,Buzz) Values(%d,'%s','%s',%d,%d,%d,%d)"),
			MPilot::PilotState::MANUAL_RUN,_T("手動執行"),_T("Manual run"), 0, 1, 0, 0);
		pC->ExecuteSQL(strSQL);
		pC->CommitTrans();
	}
	if (rsTmp.Open(_T("Select * From Pilot"),pC))
	{
		while (!rsTmp.isEOF()){
			rsTmp.GetValue(_T("ID"),v);
			rsTmp.GetValue(_T("RLight"),m_RLight[v]);
			rsTmp.GetValue(_T("GLight"),m_GLight[v]);
			rsTmp.GetValue(_T("YLight"),m_YLight[v]);
			rsTmp.GetValue(_T("Buzz"),m_Buzz[v]);
			rsTmp.MoveNext();
		}
		rsTmp.Close();
	}
	MBase::LoadMachineData(pC);
}
void MPilot::SaveMachineData(CADOConnection *pC, bool bAllChildsLoad)
{
	CADORecordset rsTmp;
	CString strSQL;
	int v;
	if (rsTmp.Open(_T("Select * From Pilot"), pC))
	{
		while (!rsTmp.isEOF()) {
			rsTmp.GetValue(_T("ID"), v);
			if (v >= 0 && v < PilotState::UNKNOW)
			{
				pC->BeginTrans();
				rsTmp.SetValue(_T("RLight"), m_RLight[v]);
				rsTmp.SetValue(_T("GLight"), m_GLight[v]);
				rsTmp.SetValue(_T("YLight"), m_YLight[v]);
				rsTmp.SetValue(_T("Buzz"), m_Buzz[v]);
				rsTmp.Update();
				pC->CommitTrans();
			}
			rsTmp.MoveNext();
		}
		rsTmp.Close();
	}
	MBase::SaveMachineData(pC);
}
BEGIN_MESSAGE_MAP(MPilot, MBase)
END_MESSAGE_MAP()



// MPilot 訊息處理常式
