// FormViewConfig.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "FormViewConfig.h"


// FormViewConfig

IMPLEMENT_DYNCREATE(FormViewConfig, FormTabAutoma)

FormViewConfig::FormViewConfig()
	: FormTabAutoma(IDD_CONFIGVIEW, IDC_TABCONFIG)
{
}

FormViewConfig::~FormViewConfig()
{

}


BEGIN_MESSAGE_MAP(FormViewConfig, CFormView)
END_MESSAGE_MAP()


// FormViewConfig 診斷

#ifdef _DEBUG
void FormViewConfig::AssertValid() const
{
	FormTabAutoma::AssertValid();
}

#ifndef _WIN32_WCE
void FormViewConfig::Dump(CDumpContext& dc) const
{
	FormTabAutoma::Dump(dc); 
}
#endif
#endif //_DEBUG


// FormViewConfig 訊息處理常式


void FormViewConfig::OnInitialUpdate()
{
	FormTabAutoma::OnInitialUpdate();
	m_pageMachine.Create(IDD_DLG_MACHINE, this);
	m_pageMotor.Create(IDD_DLG_MOTOR, this);
	m_pageValve.Create(IDD_DLG_VALVE, this);
	m_pageTimer.Create(IDD_DLG_TIMER, this);
	m_pageIO.Create(IDD_DLG_IO, this);
	m_pagePilot.Create(IDD_DLG_PILOT, this);
	m_pageMachine.SetWindowTextW(_T("Machine"));
	m_pageMotor.SetWindowTextW(_T("Motor"));
	m_pageValve.SetWindowTextW(_T("Valve"));
	m_pageTimer.SetWindowTextW(_T("Timer"));
	m_pageIO.SetWindowTextW(_T("IO"));
	m_pagePilot.SetWindowTextW(_T("Pilot"));
	AddPage(&m_pageMachine);
	AddPage(&m_pageMotor);
	AddPage(&m_pageValve);
	AddPage(&m_pageTimer);
	AddPage(&m_pageIO);
	AddPage(&m_pagePilot);
}

