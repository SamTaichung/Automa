#pragma once

#include "FormViewAutoma.h"
#include "ButtonAutoma.h"
#include "PictureAutoma.h"
#include "afxwin.h"
using namespace AutoLib;
using namespace AutoUI;
// FormViewAuto 表單檢視


class FormViewAuto : public FormViewAutoma
{
	DECLARE_DYNCREATE(FormViewAuto)

protected:
	FormViewAuto();           // 動態建立所使用的保護建構函式
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	PictureAutoma m_stcPicture;
	afx_msg void OnPaint();
	afx_msg void OnStnClickedPictureAuto();
	ButtonAutoma m_btnStart;
	afx_msg void OnBnClickedBtnStart();
};


