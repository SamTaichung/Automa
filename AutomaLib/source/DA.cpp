#include "stdafx.h"
#include "DA.h"

using namespace AutoLib;

std::map<CString,MDA*>	MDA::m_Members;

MDA::MDA(MBase *pB,CString strID,CString strName,int ChannelID,CString LineID,int Max,int Min)
:MBase(pB,strID,strName),m_ChannelID(ChannelID),m_LineID(LineID),m_dblValue(-1),m_dblMax(Max),m_dblMin(Min)
{
	std::map<CString,MDA*>::iterator itDA;
	itDA=m_Members.find(strID);
	if (itDA==m_Members.end())
		m_Members.insert(std::make_pair(strID,this));
	else
		ShowMessage(this,_T("DA Duplicate(")+strID+_T(")"),MMessage::MSGIMG::Warn);
}

MDA::~MDA(void)
{
	std::map<CString,MDA*>::iterator itTmp;
	if (m_ID.Trim()!= _T("") && !m_Members.empty())
	{
		itTmp=m_Members.find(m_ID);
		if (itTmp!=m_Members.end())
		{
			m_Members.erase(m_ID);
		}
	}
}

double MDA::GetValue()
{
	double dblValue=m_dblValue*(m_dblMax-m_dblMin)/100+m_dblMin;
	return dblValue;
}

void MDA::SaveMachineData(CADOConnection * pC,bool bAllChildsSave)
{
	CADORecordset rsTmp;
	_variant_t id,v;
	if (rsTmp.Open(_T("Select * From DAs Where ID='") +m_ID+ _T("'"),pC))
	{
		if (rsTmp.isEOF())
		{
			pC->BeginTrans();
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"),m_ID);
			rsTmp.SetValue(_T("ChannelID"),m_ChannelID);
			rsTmp.SetValue(_T("LineID"),m_LineID);
			rsTmp.Update();
			pC->CommitTrans();
		}
		rsTmp.Close();
	}
	MBase::SaveMachineData(pC, bAllChildsSave);
}

void MDA::LoadMachineData(CADOConnection * pC, bool bAllChildsLoad)
{
	CADORecordset rsTmp;
	_variant_t id,v;
	if (rsTmp.Open(_T("Select * From DAs Where ID='") +m_ID+ _T("'"),pC))
	{
		if (rsTmp.isEOF()){
			pC->BeginTrans();
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"),m_ID);
			rsTmp.SetValue(_T("Name"),m_strName);
			rsTmp.SetValue(_T("ChannelID"),m_ChannelID);
			rsTmp.SetValue(_T("LineID"),m_LineID);
			rsTmp.Update();
			pC->CommitTrans();
			SaveMachineData(pC);
		}else{
			rsTmp.GetValue(_T("ChannelID"),	m_ChannelID);
			rsTmp.GetValue(_T("LineID"),m_LineID);
		}
		rsTmp.Close();
	}
	MBase::LoadMachineData(pC, bAllChildsLoad);
}
