// Flash.cpp : 實作檔
//

#include "stdafx.h"
#include "Flash.h"

using namespace AutoLib;
// MFlash

IMPLEMENT_DYNAMIC(MFlash, MBase)
MFlash::MFlash(MBase *pB,CString strID,CString strName):
	MBase(pB,strID,strName),m_pIO(NULL)
{
	isDoBlink=false;
	blNow=false;
}

MFlash::~MFlash()
{
	
}
void MFlash::Cycle(const double dblTime)
{
	MBase::Cycle(dblTime);
	if (isDoBlink) 
	{ 
		if (blNow)
		{
			if (m_pTMOn->isTimeOut()) 
			{
				m_pIO->SetValue(false);
				m_pTMOff->Start();
				blNow=false;
			}
		}else{
			if (m_pTMOff->isTimeOut()) 
			{
				m_pTMOn->Start();
				m_pIO->SetValue(true);
				blNow=true;
			}
		}
	}
}
void MFlash::Stop()
{
	DoOff();
}
void MFlash::EStop(const bool isES)
{
	MBase::EStop(isES);
	if (isES)
	{
		isDoBlink=false;
	}
}
bool MFlash::isIDLE()
{
	return(true);
}
void MFlash::DoBlink()
{
	if (!isDoBlink)
	{
		blNow = m_pIO->GetValue();
		if (blNow)
		{
			m_pTMOn->Start();
		}
		else {
			m_pTMOff->Start();
		}
	}
	isDoBlink=true;
}
void MFlash::DoOff()
{
	if(m_pIO!=NULL)
	{
		m_pIO->SetValue(false);
		isDoBlink=false;
		blNow=false;
	}
}
void MFlash::DoOn()
{
	if(m_pIO!=NULL)
	{
		m_pIO->SetValue(true);
		isDoBlink=false;
		blNow=true;
	}
}

BEGIN_MESSAGE_MAP(MFlash, MBase)
END_MESSAGE_MAP()



// MFlash 訊息處理常式
