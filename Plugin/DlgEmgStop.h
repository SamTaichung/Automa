#pragma once
#include "afxwin.h"


// DlgEmgStop ��ܤ��

class DlgEmgStop : public CDialogEx
{
	DECLARE_DYNAMIC(DlgEmgStop)

public:
	DlgEmgStop(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~DlgEmgStop();

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ESTOP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	bool m_bBlinking;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CStatic m_stcEmgStop;
};
