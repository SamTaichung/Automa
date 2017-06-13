#pragma once

#include "FormViewAutoma.h"
#include "afxcmn.h"
#include "afxwin.h"

using namespace AutoLib;
using namespace AutoUI;

// FormViewTop 表單檢視

class FormViewTop : public FormViewAutoma
{
	DECLARE_DYNCREATE(FormViewTop)

protected:
	FormViewTop();           // 動態建立所使用的保護建構函式
	virtual ~FormViewTop();
	void MachineMessage(MMessage *pMsg);

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOPVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	CImageList *m_pImageList;

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listMessage;
	virtual void OnInitialUpdate();
	CStatic m_stcLogo;
	afx_msg void OnBnClickedBtnStatus();
};


