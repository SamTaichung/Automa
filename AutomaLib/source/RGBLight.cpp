#include "StdAfx.h"
#include "RGBLight.h"

using namespace AutoLib;
// M6308VLight 燈箱的亮度控制
// 用到map來取得燈箱控制器的集合,所以下一行不可省略
std::map<CString,MRGBLight*>	MRGBLight::m_Members;


IMPLEMENT_DYNAMIC(MRGBLight, MBase)

MRGBLight::MRGBLight(MBase *pB,CString strID,CString strName,int intMin,int intMax)
	:MBase(pB,strID,strName),m_Min(intMin),m_Max(intMax)
{
	std::map<CString,MRGBLight*>::iterator itLighter;
	m_isOn = false;
	itLighter=m_Members.find(strID);
	if (itLighter==m_Members.end())
	{
		m_Members.insert(std::map<CString,MRGBLight*>::value_type(strID,this));
	}else{
		ShowMessage(this,_T("Lighter Duplicate(")+strID+_T(")"),MMessage::MSGIMG::Warn);
	}
	m_csDataID=_T("Light");
	m_csItemIndex=strID;
	m_RValue=0;
	m_GValue=0;
	m_BValue=0;
}
MRGBLight::~MRGBLight()
{
}
void MRGBLight::AllLightsLoad()
{
	std::map<CString,MRGBLight*>::iterator itLighter;
	for (itLighter=m_Members.begin();itLighter!=m_Members.end();itLighter++)
	{
		(*itLighter).second->Load();
	}
}

MRGBLight * MRGBLight::GetLightControl(CString ID)
{
	std::map<CString,MRGBLight*>::iterator itLighter;
	itLighter=m_Members.find(ID);
	if (itLighter==m_Members.end())
	{
		return(NULL);
	}else{
		return((*itLighter).second);
	}
}

bool MRGBLight::SetValue(int R,int G,int B)
{
	if (R>m_Max) R=m_Max;
	if (R<m_Min) R=m_Min;
	m_RValue=R;
	if (G>m_Max) G=m_Max;
	if (G<m_Min) G=m_Min;
	m_GValue=G;
	if (B>m_Max) B=m_Max;
	if (B<m_Min) B=m_Min;
	m_BValue=B;
	return true;
}

bool MRGBLight::IncreaseValue(int R,int G,int B)
{
	return SetValue(m_RValue+R,m_GValue+G,m_BValue+B);
}

bool MRGBLight::DescreaseValue(int R,int G,int B)
{
	return SetValue(m_RValue-R,m_GValue-G,m_BValue-B);
}
void MRGBLight::SetDataIDIndex(CString strDataID,CString strItemIndex)
{
	m_csDataID=strDataID;
	m_csItemIndex=strItemIndex;
}
void MRGBLight::GetDataIDIndex(CString *pstrDataID,CString *pstrItemIndex) //設定在Table中要存取的ID及ItemIndex
{
	*pstrDataID=m_csDataID;
	*pstrItemIndex=m_csItemIndex;
}
void MRGBLight::LoadRecipeData(CADOConnection * pC)
{
	m_pADOcn=pC;
	Load();
	MBase::LoadRecipeData(pC);
}

void MRGBLight::SaveRecipeData(CADOConnection * pC)
{
	m_pADOcn=pC;
	Save();
	MBase::SaveRecipeData(pC);
}

bool MRGBLight::Save()
{
	CADORecordset rsTmp;	
	CString strSQL,strDataName;
	bool isOn;
	strSQL.Format(_T("Select * From Data Where ID= '")+
		m_csDataID+_T("' and ItemIndex='")+m_csItemIndex+
		_T("'"));
	if (rsTmp.Open(strSQL,m_pADOcn))
	{
		isOn=GetOn();
		rsTmp.SetValue(_T("DATA1"),(double)m_RValue);
		rsTmp.SetValue(_T("DATA2"),(double)m_GValue);
		rsTmp.SetValue(_T("DATA3"),(double)m_BValue);
		rsTmp.SetValue(_T("DATA4"),(double)isOn);
		rsTmp.Update();
		rsTmp.Close();
	}
	return true;
}
bool MRGBLight::Load()
{
	CADORecordset rsTmp;	
	CString strSQL;
	bool isOn;
	strSQL.Format(_T("Select * From Data Where ID= '") + m_csDataID + _T("' and ItemIndex='") + m_csItemIndex + _T("'"));;
	if (rsTmp.Open(strSQL,m_pADOcn))
	{
		double dbVal;
		rsTmp.GetValue(_T("DATA1"),dbVal);
		m_RValue=int(dbVal);
		rsTmp.GetValue(_T("DATA2"),dbVal);
		m_GValue=int(dbVal);
		rsTmp.GetValue(_T("DATA3"),dbVal);
		m_BValue=int(dbVal);
		rsTmp.GetValue(_T("DATA4"),dbVal);
		isOn=true;
		if (dbVal==0)
		{
			isOn=false;
		}
		SetOn(isOn);
		rsTmp.Close();
	}
	return true;
}

BEGIN_MESSAGE_MAP(MRGBLight, MBase)
END_MESSAGE_MAP()
