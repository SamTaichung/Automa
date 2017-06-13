#pragma once

#include "FormViewAutoma.h"
#include "FormTabAutoma.h"
#include "TabCtrlAutoma.h"
#include "ButtonAutoma.h"
#include "PictureAutoma.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "DlgAutoMotorPage.h"

using namespace AutoLib;
using namespace AutoUI;
// FormViewAuto ����˵�

class FormViewAuto : public FormTabAutoma
{
	DECLARE_DYNCREATE(FormViewAuto)

protected:
	FormViewAuto();           // �ʺA�إߩҨϥΪ��O�@�غc�禡
	virtual ~FormViewAuto();
	
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AUTOVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
public:
	PictureAutoma m_stcPicture;
	afx_msg void OnPaint();
	afx_msg void OnStnClickedPictureAuto();
	ButtonAutoma m_btnStart;
	afx_msg void OnBnClickedBtnStart();
	DlgAutoMotorPage m_pageMotor;
	virtual void OnInitialUpdate();
};


