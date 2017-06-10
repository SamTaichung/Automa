// Valve.cpp : 實作檔
//

#include "stdafx.h"
#include "ValveIO.h"

// MValveIO
using namespace AutoLib;

MValveIO::MValveIO(MBase *pB,CString strID,CString strName,CString strOpen,CString strClose,ValveStyle vsStyle):
	MValve(pB, strID, strName, strOpen, strClose, vsStyle)
{
	m_pIOOpen=NULL;
	m_pIOClose=NULL;
	m_pIOOpenSW=NULL;
	m_pIOCloseSW=NULL;
	m_pIOOpenSR=NULL;
	m_pIOCloseSR=NULL;
	m_pIOOpenSWSR=NULL;
	m_pIOCloseSWSR=NULL;
}
MValveIO::~MValveIO()
{

}
CString MValveIO::GetStateName()
{
	CString strRet;
	if (isOpen())
	{
		strRet=m_strOpenStatus;
	}else if(isClose())
	{
		strRet=m_strCloseStatus;
	}else
	{
		strRet=MValve::GetStateName();
	}
	return(strRet);
}

void MValveIO::LoadMachineData(CADOConnection * pC,bool bAllChildsLoad)
{
	MValve::LoadMachineData(pC, bAllChildsLoad);
}
void MValveIO::SaveMachineData(CADOConnection * pC,bool bAllChildsSave)
{
	MValve::SaveMachineData(pC, bAllChildsSave);
}
bool MValveIO::DoOpen()
{
	if (m_pIOOpen != NULL)
	{
		m_pIOOpen->SetValue(true);
	}
	if (m_pIOOpenSW != NULL)
	{
		m_pIOOpenSW->SetValue(m_vmMode == ValveMode::vmForce);
	}
	if (m_pIOClose != NULL)
	{
		m_pIOClose->SetValue(false);
	}
	if (m_pIOCloseSW != NULL)
	{
		m_pIOCloseSW->SetValue(false);
	}
	return true;
}
void MValveIO::OnOpening()
{
	if (isOpen() || m_isPassSensor) //Open完成，如有切換就執行
	{
		if (m_pIOOpenSW != NULL)
		{
			m_pIOOpenSW->SetValue(true);
		}
	}else{		//---------------檢查是否需要切換
		if (m_pIOOpenSWSR->GetValue() && m_pIOOpenSW != NULL)
		{
			m_pIOOpenSW->SetValue(true);
		}
	}
}
bool MValveIO::DoClose()
{
	if (m_pIOClose != NULL)
	{
		m_pIOClose->SetValue(true);
	}
	if (m_pIOCloseSW != NULL)
	{
		m_pIOCloseSW->SetValue(m_vmMode == ValveMode::vmForce);
	}
	if (m_pIOOpen != NULL)
	{
		m_pIOOpen->SetValue(false);
	}
	if (m_pIOOpenSW != NULL)
	{
		m_pIOOpenSW->SetValue(false);
	}
	return true;
}
void MValveIO::OnClosing()
{
	if (isClose() || m_isPassSensor)
	{
		if (m_pIOCloseSW != NULL)
		{
			m_pIOCloseSW->SetValue(true);
		}
	}
	else {
		if (m_pIOCloseSWSR != NULL) //---------------檢查是否需要切換
		{
			if (m_pIOCloseSWSR->GetValue() && m_pIOCloseSW != NULL)
			{
				m_pIOCloseSW->SetValue(true);
			}
		}
	}
}
bool MValveIO::OffValve()
{
	if (m_pIOOpen!=NULL)
	{
		m_pIOOpen->SetValue(false);
	}
	if (m_pIOClose!=NULL)
	{
		m_pIOClose->SetValue(false);
	}
	return true;
}
bool MValveIO::isOpen()
{
	bool bRetVal;
	if (m_pIOOpenSR!=NULL)
	{
		bRetVal=m_pIOOpenSR->GetValue();
		return bRetVal;
	}

	if (m_pIOCloseSR!=NULL)
	{
		bRetVal=!m_pIOCloseSR->GetValue();
		return bRetVal;
	}
	if (m_pIOOpen!=NULL)
		return(m_pIOOpen->GetValue());
	if (m_pIOClose!=NULL)
		return(!m_pIOClose->GetValue());
	return(true);
}
bool MValveIO::isClose()
{
	bool bRetVal;
	if (m_pIOCloseSR!=NULL)
	{
		bRetVal=m_pIOCloseSR->GetValue();
		return bRetVal;
	}
	if (m_pIOOpenSR!=NULL)
	{
		bRetVal=!m_pIOOpenSR->GetValue();
		return bRetVal;
	}
	if (m_pIOClose!=NULL)
		return(m_pIOClose->GetValue());
	if (m_pIOOpen!=NULL)
		return(!m_pIOOpen->GetValue());
	return(true);
}
