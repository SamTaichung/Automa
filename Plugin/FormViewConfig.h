#pragma once

#include "FormTabAutoma.h"
#include "DlgConfigMachine.h"
#include "DlgConfigMotor.h"
#include "DlgConfigTimer.h"
#include "DlgConfigValve.h"
#include "DlgConfigIO.h"
#include "DlgConfigPilot.h"
#include "TabCtrlAutoma.h"
#include "afxcmn.h"

using namespace AutoLib;
using namespace AutoUI;

// FormViewConfig ����˵�

class FormViewConfig : public FormTabAutoma
{
	DECLARE_DYNCREATE(FormViewConfig)

protected:
	FormViewConfig();           // �ʺA�إߩҨϥΪ��O�@�غc�禡
	virtual ~FormViewConfig();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIGVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DlgConfigMachine m_pageMachine;
	DlgConfigMotor m_pageMotor;
	DlgConfigValve m_pageValve;
	DlgConfigTimer m_pageTimer;
	DlgConfigIO m_pageIO;
	DlgConfigPilot m_pagePilot;

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


