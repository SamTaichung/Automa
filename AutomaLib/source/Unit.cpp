// Card.cpp : 實作檔
//

#include "stdafx.h"
#include "Unit.h"

using namespace AutoLib;
MUnit::MUnit(MUnit *pParent,CString strID,CString strName):
	MBase(pParent,strID,strName)
{
	//加自己到父單元的管理單元物件表中
	if (pParent != NULL)
	{
		pParent->m_Units.Add(this);
	}
	m_AutoRunMode = AutoRunMode::DryRun;
	m_bAutoEnd = true;
}
MUnit::~MUnit()
{
}
void MUnit::SetAutoRunMode(AutoRunMode mode)
{
	m_AutoRunMode = mode;
	for (int i = 0; i < m_Units.GetCount(); i++)
	{
		m_Units[i]->SetAutoRunMode(mode);
	}
}
bool MUnit::AutoStart()
{
	bool bRet = true;
	m_bAutoEnd = false;
	for (int i = 0; i < m_Units.GetCount(); i++)
	{
		bRet = m_Units[i]->AutoStart();
		if (!bRet)
		{
			break;
		}
	}
	return bRet;
}
void MUnit::AutoStop()
{
	m_bAutoEnd = true;
	for (int i = 0; i < m_Units.GetCount(); i++)
	{
		m_Units[i]->AutoStop();
	}
}

