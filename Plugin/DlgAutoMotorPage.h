#pragma once
#include "TabPageAutoma.h"
#include "ListCtrlEx.h"
#include "ButtonAutoma.h"
#include "EditAutoma.h"
#include "afxwin.h"
using namespace AutoUI;
class DlgAutoMotorPage : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgAutoMotorPage)

public:
	DlgAutoMotorPage(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~DlgAutoMotorPage();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_AUTO_MOTORS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	void RefreshPage();

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_stcXPos;
	CStatic m_stcYPos;
	CStatic m_stcZPos[6];
	CStatic m_stcRPos[6];

};
