// IO.cpp : 實作檔
//

#include "stdafx.h"
#include "IO.h"

using namespace AutoLib;

// MIO
MIO::MIO(MBase *pB,CString strID,CString strName,MIOLib *pIOLib,bool bisOut,bool bLogic):
	MBase(pB,strID,strName),m_isOut(bisOut),m_Logic(bLogic),m_pIOLib(pIOLib)
{
	m_CardID = 0;
	m_ConnectID = 0;
	m_StationID = 0;
	m_PinID = 0;
	m_Value=!bLogic;
}

MIO::~MIO()
{
}
bool MIO::GetIsOut()
{
	return m_isOut;
}
void MIO::SetValue(bool bValue)
{
	m_pIOLib->SetValue(this, !(bValue ^ m_Logic));
}
bool MIO::GetValue()
{
	return (!(m_pIOLib->GetValue(this) ^ m_Logic));
}
void MIO::Inverse()
{
	m_pIOLib->SetValue(this,!m_pIOLib->GetValue(this));
}
void MIO::LoadMachineData(CADOConnection * pC,bool bAllChildsLoad)
{
	CADORecordset rsTmp;
	CString strSQL;
	_variant_t id,v;
	if (pC->GetTableIndex(_T("IOs"))<0)
	{
		strSQL = _T("Create Table IOs(ID nChar(10),");
		strSQL += _T("CardID	int not	null default 0,");
		strSQL += _T("ConnectID	int not	null default 0,");
		strSQL += _T("StationID	int not	null default 0,");
		strSQL += _T("PinID	int not	null default 0,");
		strSQL += _T("Logic bit default 1,");
		strSQL += _T("Constraint PKIOs Primary Key(ID))");
		pC->BeginTrans();
		pC->ExecuteSQL(strSQL);
		pC->CommitTrans();
	}
	if (rsTmp.Open(_T("Select * From IOs Where ID='") +m_ID+_T("'"),pC))
	{
		if (rsTmp.isEOF()){
			pC->BeginTrans();
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"), m_ID);
			rsTmp.SetValue(_T("CardID"), m_CardID);
			rsTmp.SetValue(_T("ConnectID"), m_ConnectID);
			rsTmp.SetValue(_T("StationID"), m_StationID);
			rsTmp.SetValue(_T("PinID"), m_PinID);
			rsTmp.SetValue(_T("Logic"), m_Logic);
			rsTmp.Update();
			pC->CommitTrans();
		}else{
			rsTmp.GetValue(_T("CardID"), m_CardID);
			rsTmp.GetValue(_T("ConnectID"), m_ConnectID);
			rsTmp.GetValue(_T("StationID"), m_StationID);
			rsTmp.GetValue(_T("PinID"), m_PinID);
			rsTmp.GetValue(_T("Logic"), m_Logic);
		}
		rsTmp.Close();
	}
	MBase::LoadMachineData(pC, bAllChildsLoad);
}
void MIO::SaveMachineData(CADOConnection * pC,bool bAllChildsSave)
{
	CADORecordset rsTmp;
	_variant_t id,v;

	if (rsTmp.Open(_T("Select * From IOs Where ID='") +m_ID+_T("'"),pC))
	{
		pC->BeginTrans();
		if (rsTmp.isEOF()){
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"),m_ID);
		}
		rsTmp.SetValue(_T("Logic"),m_Logic);
		rsTmp.Update();
		pC->CommitTrans();
		rsTmp.Close();
	}
	MBase::SaveMachineData(pC, bAllChildsSave);
}




// MIO 訊息處理常式
