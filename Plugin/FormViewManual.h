#pragma once

#include "FormViewAutoma.h"
using namespace AutoLib;
using namespace AutoUI;



// FormViewManual ����˵�

class FormViewManual : public FormViewAutoma
{
	DECLARE_DYNCREATE(FormViewManual)

protected:
	FormViewManual();           // �ʺA�إߩҨϥΪ��O�@�غc�禡
	virtual ~FormViewManual();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANUALVIEW };
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
	afx_msg void OnBnClickedButton1();
};


