// MConveyor.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "MConveyor.h"

MConveyor::MConveyor(MUnit *pParent, CString strID, CString strName):
	MUnit(pParent,strID,strName)
{

}

MConveyor::~MConveyor()
{
}
void MConveyor::LoadRecipeData(CADOConnection * pC, bool bAllChildsLoad)
{
	CADORecordset rsTmp;
	CString strSQL,strField;
	_variant_t id, v;
	if (pC->GetTableIndex(_T("Recipe")) < 0)
	{
		strSQL = _T("Create Table Recipe(");
		strSQL += _T("XBasePos float not null default 0,");
		strSQL += _T("YBasePos float not null default 0,");
		strSQL += _T("ZBasePos float not null default 0,");
		strSQL += _T("NumOfX int not null default 1,");
		strSQL += _T("NumOfY int not null default 1,");
		strSQL += _T("XPitch float not null default 0,");
		strSQL += _T("YPitch float not null default 0,");
		strSQL += _T("XofMark0 float not null default 0,");
		strSQL += _T("YofMark0 float not null default 0,");
		strSQL += _T("XofMark1 float not null default 0,");
		strSQL += _T("YofMark1 float not null default 0,");
		strSQL += _T("XofMark2 float not null default 0,");
		strSQL += _T("YofMark2 float not null default 0,");
		strSQL += _T("XofMark3 float not null default 0,");
		strSQL += _T("YofMark3 float not null default 0)");
		pC->BeginTrans();
		pC->ExecuteSQL(strSQL);
		pC->CommitTrans();

	}
	if (rsTmp.Open(_T("Select * From Recipe"), pC))
	{
		if (!rsTmp.isEOF()) {
			rsTmp.GetValue(_T("XBasePos"), m_PCB.m_cdBase.x);
			rsTmp.GetValue(_T("YBasePos"), m_PCB.m_cdBase.y);
			rsTmp.GetValue(_T("ZBasePos"), m_PCB.m_cdBase.z);
			rsTmp.GetValue(_T("NumOfX"), m_PCB.m_intNumOfX);
			rsTmp.GetValue(_T("NumOfY"), m_PCB.m_intNumOfY);
			rsTmp.GetValue(_T("XPitch"), m_PCB.m_dblXPitch);
			rsTmp.GetValue(_T("YPitch"), m_PCB.m_dblYPitch);
		}
		for (int i = 0; i < 4; i++)
		{
			strField.Format(_T("XofMark%d"), i);
			rsTmp.GetValue(strField, m_PCB.m_cdPCBMark[i].x);
			strField.Format(_T("YofMark%d"), i);
			rsTmp.GetValue(strField, m_PCB.m_cdPCBMark[i].y);
		}
		rsTmp.Close();
	}
	if (pC->GetTableIndex(_T("RecipeData"))<0)
	{
		strSQL = _T("Create Table RecipeData(ID Int,");
		strSQL += _T("ComponentID nChar(20),");
		strSQL += _T("PlugPosX float not null default 0,");
		strSQL += _T("PlugPosY float not null default 0,");
		strSQL += _T("PlugPosZ float not null default 0,");
		strSQL += _T("PlugPosS float not null default 0,");
		strSQL += _T("PlugOffsetX float not null default 0,");
		strSQL += _T("PlugOffsetY float not null default 0,");
		strSQL += _T("PlugOffsetZ float not null default 0,");
		strSQL += _T("PlugOffsetS float not null default 0,");
		strSQL += _T("Constraint PKRecipeData Primary Key(ID))");
		pC->BeginTrans();
		pC->ExecuteSQL(strSQL);
		pC->CommitTrans();
	}
	if (rsTmp.Open(_T("Select * From RecipeData Order by ID"), pC))
	{
	//	m_PCB.RemoveAll();
		int count = 0;
		CString strComponentID;
		while (!rsTmp.isEOF()) {
			PluginPoint *pPP;
			count++;
			if (m_PCB.GetPlugPointCount() < count)
			{
				pPP = new PluginPoint();
				m_PCB.AddPlugPoint(pPP);
			}
			else {
				pPP = m_PCB.GetPlugPoint(count - 1);
			}
			rsTmp.GetValue(_T("ComponentID"), strComponentID);
			strComponentID.Trim();
			if (strComponentID.Compare(pPP->m_strComponentID) != 0)
			{
				pPP->m_strComponentID = strComponentID;
				pPP->m_bPlugFinish = false;
			}
			rsTmp.GetValue(_T("PlugPosX"), pPP->m_cdPlugPosition.x);
			rsTmp.GetValue(_T("PlugPosY"), pPP->m_cdPlugPosition.y);
			rsTmp.GetValue(_T("PlugPosZ"), pPP->m_cdPlugPosition.z);
			rsTmp.GetValue(_T("PlugPosS"), pPP->m_cdPlugPosition.s);
			rsTmp.GetValue(_T("PlugOffsetX"), pPP->m_cdPlugOffset.x);
			rsTmp.GetValue(_T("PlugOffsetY"), pPP->m_cdPlugOffset.y);
			rsTmp.GetValue(_T("PlugOffsetZ"), pPP->m_cdPlugOffset.z);
			rsTmp.GetValue(_T("PlugOffsetS"), pPP->m_cdPlugOffset.s);
			rsTmp.MoveNext();
		}
		rsTmp.Close();
		while (m_PCB.GetPlugPointCount() > count)
		{
			m_PCB.RemovePlugPoint(count);
		}
	}
	MUnit::LoadRecipeData(pC, bAllChildsLoad);
}
void MConveyor::SaveRecipeData(CADOConnection * pC, bool bAllChildsSave)
{
	CADORecordset rsTmp;
	CString strField;
	_variant_t id, v;
	if (rsTmp.Open(_T("Select * From Recipe"), pC))
	{
		pC->BeginTrans();
		if (rsTmp.isEOF())
		{
			rsTmp.AddNew();
		}
		rsTmp.SetValue(_T("XBasePos"), m_PCB.m_cdBase.x);
		rsTmp.SetValue(_T("YBasePos"), m_PCB.m_cdBase.y);
		rsTmp.SetValue(_T("ZBasePos"), m_PCB.m_cdBase.z);
		rsTmp.SetValue(_T("NumOfX"), m_PCB.m_intNumOfX);
		rsTmp.SetValue(_T("NumOfY"), m_PCB.m_intNumOfY);
		rsTmp.SetValue(_T("XPitch"), m_PCB.m_dblXPitch);
		rsTmp.SetValue(_T("YPitch"), m_PCB.m_dblYPitch);
		for (int i = 0; i < 4; i++)
		{
			strField.Format(_T("XofMark%d"), i);
			rsTmp.SetValue(strField, m_PCB.m_cdPCBMark[i].x);
			strField.Format(_T("YofMark%d"), i);
			rsTmp.SetValue(strField, m_PCB.m_cdPCBMark[i].y);
		}
		rsTmp.Update();
		pC->CommitTrans();
		rsTmp.Close();
	}
	pC->BeginTrans();
	pC->ExecuteSQL(_T("Delete * from RecipeData"));
	pC->CommitTrans();
	if (rsTmp.Open(_T("Select * From RecipeData Order by ID"), pC))
	{
		struct PluginPoint *pPoint;
		for (int i = 0; i < m_PCB.GetPlugPointCount(); i++)
		{
			pPoint = m_PCB.GetPlugPoint(i);
			pC->BeginTrans();
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"), i);
			rsTmp.SetValue(_T("ComponentID"), pPoint->m_strComponentID);
			rsTmp.SetValue(_T("PlugPosX"), pPoint->m_cdPlugPosition.x);
			rsTmp.SetValue(_T("PlugPosY"), pPoint->m_cdPlugPosition.y);
			rsTmp.SetValue(_T("PlugPosZ"), pPoint->m_cdPlugPosition.z);
			rsTmp.SetValue(_T("PlugPosS"), pPoint->m_cdPlugPosition.s);
			rsTmp.SetValue(_T("PlugOffsetX"), pPoint->m_cdPlugOffset.x);
			rsTmp.SetValue(_T("PlugOffsetY"), pPoint->m_cdPlugOffset.y);
			rsTmp.SetValue(_T("PlugOffsetZ"), pPoint->m_cdPlugOffset.z);
			rsTmp.SetValue(_T("PlugOffsetS"), pPoint->m_cdPlugOffset.s);
			rsTmp.Update();
			pC->CommitTrans();
		}
		rsTmp.Close();
	}
	MUnit::SaveRecipeData(pC, bAllChildsSave);
}

void MConveyor::StepCycle(const double dblTime)
{
	switch (m_Step)
	{
	case STEP::StartHome:
		m_strStepName = _T("Start Home");

		break;
	}
}
// MConveyor 訊息處理常式
