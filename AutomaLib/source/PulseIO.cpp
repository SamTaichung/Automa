// PulseIO.cpp : 實作檔
//

#include "stdafx.h"
#include "PulseIO.h"


// MPulseIO
using namespace AutoLib;

IMPLEMENT_DYNAMIC(MPulseIO, MBase)
MPulseIO::MPulseIO(MBase *pB,CString strID,CString strName):
	MBase(pB,strID,strName)
{
}

MPulseIO::~MPulseIO()
{
}

void MPulseIO::StepCycle(const double dblTime)
{
	if (m_pOnTimer->isTimeOut())
	{
		ASSERT(m_pIO!=NULL);
		m_State=MBase::STATE::IDLE;
		m_pIO->SetValue(false);
	}

}
void MPulseIO::SetIOValue(bool blV)
{
	ASSERT(m_pIO!=NULL);
	m_pIO->SetValue(blV);
}
bool MPulseIO::GetIOValue()
{
	ASSERT(m_pIO!=NULL);
	return(m_pIO->GetValue());
}
void MPulseIO::Pulse()
{
	ASSERT(m_pIO!=NULL);
	m_pIO->SetValue(true);
	m_pOnTimer->Start();
	m_State=MBase::STATE::ACTION;
}
void MPulseIO::Stop()
{
	m_State=MBase::STATE::IDLE;
	m_pOnTimer->Stop();
}
void MPulseIO::EStop(const bool isES)
{
	m_pOnTimer->Stop();
	MBase::EStop(isES);
}

void MPulseIO::SetOnTimer(double dbTimer)
{
	m_pOnTimer->SetInterval(dbTimer);
}
BEGIN_MESSAGE_MAP(MPulseIO, MBase)
END_MESSAGE_MAP()



// MPulseIO 訊息處理常式
