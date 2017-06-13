#pragma once

#include "FormViewAutoma.h"
using namespace AutoLib;
using namespace AutoUI;



// FormViewProduction 表單檢視

class FormViewProduction : public FormViewAutoma
{
	DECLARE_DYNCREATE(FormViewProduction)

protected:
	FormViewProduction();           // 動態建立所使用的保護建構函式
	virtual ~FormViewProduction();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRODUCTIONVIEW };
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
};


