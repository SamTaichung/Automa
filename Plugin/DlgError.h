#pragma once
#include "Base.h"
#include "Error.h"
#include "Solution.h"
#include "afxwin.h"
using namespace AutoLib;
class DlgError : public CDialogEx
{
	DECLARE_DYNAMIC(DlgError)
public:
	DlgError(CWnd* pParent,MError *pError);   // 標準建構函式
	virtual ~DlgError();

	// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_ERROR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	MError *m_pError;

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_stcDescript;
	CComboBox m_cmbSolutions;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
	CStatic m_stcHappener;
	CStatic m_stcMemo;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnCancel();
};