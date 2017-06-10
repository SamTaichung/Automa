#pragma once

#include "FormViewAutoma.h"
using namespace AutoLib;
using namespace AutoUI;



// FormViewManual 表單檢視

class FormViewManual : public FormViewAutoma
{
	DECLARE_DYNCREATE(FormViewManual)

protected:
	FormViewManual();           // 動態建立所使用的保護建構函式
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};


