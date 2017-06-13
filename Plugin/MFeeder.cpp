// MConveyor.cpp : ¹ê§@ÀÉ
//

#include "stdafx.h"
#include "Plugin.h"
#include "MFeeder.h"


// MConveyor

MFeeder::MFeeder(MUnit *pParent, CString strID, CString strName):
	MUnit(pParent,strID,strName)
{
	m_strComponentID = _T("");
}

MFeeder::~MFeeder()
{
}
void MFeeder::StepCycle(const double dblTime)
{

}
bool MFeeder::GetReadyToFeed()
{
	return true;
}
void MFeeder::LoadMachineData(CADOConnection * pC, bool bAllChildsLoad)
{
	CADORecordset rsTmp;
	CString strSQL;
	_variant_t id, v;
	if (pC->GetTableIndex(_T("Feeder"))<0)
	{
		strSQL = _T("Create Table Feeder(ID nChar(20),");
		strSQL += _T("ComponentID nChar(20),");
		strSQL += _T("XBasePos float not null default 0,");
		strSQL += _T("YBasePos float not null default 0,");
		strSQL += _T("ZBasePos float not null default 0,");
		strSQL += _T("Constraint PKFeeder Primary Key(ID))");
		pC->BeginTrans();
		pC->ExecuteSQL(strSQL);
		pC->CommitTrans();
	}
	if (rsTmp.Open(_T("Select * From Feeder Where ID='") + m_ID + _T("'"), pC))
	{
		if (rsTmp.isEOF()) {
			pC->BeginTrans();
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"), m_ID);
			rsTmp.Update();
			pC->CommitTrans();
		}
		else {
			rsTmp.GetValue(_T("XBasePos"), m_cdBase.x);
			rsTmp.GetValue(_T("YBasePos"), m_cdBase.y);
			rsTmp.GetValue(_T("ZBasePos"), m_cdBase.z);
			rsTmp.GetValue(_T("ComponentID"), m_strComponentID);
			m_strComponentID.Trim();
		}
		rsTmp.Close();
	}
	MUnit::LoadMachineData(pC, bAllChildsLoad);
}
void MFeeder::SaveMachineData(CADOConnection * pC, bool bAllChildsSave)
{
	CADORecordset rsTmp;
	_variant_t id, v;
	if (rsTmp.Open(_T("Select * From Feeder Where ID='") + m_ID + _T("'"), pC))
	{
		pC->BeginTrans();
		if (rsTmp.isEOF()) {
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"), m_ID);
		}
		rsTmp.SetValue(_T("XBasePos"), m_cdBase.x);
		rsTmp.SetValue(_T("YBasePos"), m_cdBase.y);
		rsTmp.SetValue(_T("ZBasePos"), m_cdBase.z);
		rsTmp.SetValue(_T("ComponentID"), m_strComponentID);
		rsTmp.Update();
		pC->CommitTrans();
		rsTmp.Close();
	}
	MUnit::SaveMachineData(pC, bAllChildsSave);
}


