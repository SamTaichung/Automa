// Valve.cpp : ¹ê§@ÀÉ
//

#include "stdafx.h"
#include "ValveVirtual.h"

// MValveVirtual
using namespace AutoLib;

MValveVirtual::MValveVirtual(MBase *pB,CString strID,CString strName,CString strOpen,CString strClose,ValveStyle vsStyle):
	MValve(pB,strID,strName,strOpen,strClose, vsStyle)
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
MValveVirtual::~MValveVirtual()
{

}
bool MValveVirtual::DoOpen()
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
void MValveVirtual::OnOpening()
{
	if (m_pIOOpenSW != NULL)
	{
		m_pIOOpenSW->SetValue(true);
	}
}
bool MValveVirtual::DoClose()
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
void MValveVirtual::OnClosing()
{
	if (m_pIOCloseSW != NULL)
	{
		m_pIOCloseSW->SetValue(true);
	}
}
bool MValveVirtual::OffValve()
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
bool MValveVirtual::isOpen()
{
	bool bRetVal;
	if (m_pIOOpen!=NULL)
		return(m_pIOOpen->GetValue());
	if (m_pIOClose!=NULL)
		return(!m_pIOClose->GetValue());
	return(true);
}
bool MValveVirtual::isClose()
{
	bool bRetVal;
	if (m_pIOClose!=NULL)
		return(m_pIOClose->GetValue());
	if (m_pIOOpen!=NULL)
		return(!m_pIOOpen->GetValue());
	return(true);
}
