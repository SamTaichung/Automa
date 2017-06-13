#pragma once

#include "FormViewAutoma.h"
#include "ButtonAutoma.h"
#include "afxwin.h"
using namespace AutoLib;
using namespace AutoUI;

// FormViewBottom ����˵�

class FormViewBottom : public FormViewAutoma
{
	DECLARE_DYNCREATE(FormViewBottom)

protected:
	FormViewBottom();           // �ʺA�إߩҨϥΪ��O�@�غc�禡
	virtual ~FormViewBottom();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOTTOMVIEW };
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
	void OnFunctionSelect(int index);
	ButtonAutoma m_btnFunction[6];
	ButtonAutoma m_btnStop;
	afx_msg void OnBnClickedBtAuto();
	afx_msg void OnBnClickedBtManual();
	afx_msg void OnBnClickedBtProduction();
	afx_msg void OnBnClickedBtRecipe();
	afx_msg void OnBnClickedBtConfig();
	afx_msg void OnBnClickedBtStop();
	afx_msg void OnBnClickedBtVision();
};


