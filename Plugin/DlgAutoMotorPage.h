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
	DlgAutoMotorPage(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~DlgAutoMotorPage();

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_AUTO_MOTORS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	void RefreshPage();

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_stcXPos;
	CStatic m_stcYPos;
	CStatic m_stcZPos[6];
	CStatic m_stcRPos[6];

};
