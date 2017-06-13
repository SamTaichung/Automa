#pragma once
#include "afxwin.h"


// DlgEmgStop 對話方塊

class DlgEmgStop : public CDialogEx
{
	DECLARE_DYNAMIC(DlgEmgStop)

public:
	DlgEmgStop(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~DlgEmgStop();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ESTOP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	bool m_bBlinking;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CStatic m_stcEmgStop;
};
