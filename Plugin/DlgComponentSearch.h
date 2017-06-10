#pragma once
#include "Machine.h"
#include "afxcmn.h"
#include "ListCtrlEx.h"

// DlgComponentSearch 對話方塊
using namespace AutoLib;
using namespace AutoUI;
class DlgComponentSearch : public CDialog
{
	DECLARE_DYNAMIC(DlgComponentSearch)

public:
	DlgComponentSearch(CWnd* pParent,Machine *pM, CString * pstrID, CString * pstrName);   // 標準建構函式
	virtual ~DlgComponentSearch();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LIB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	Machine *m_pMachine;
	CString *m_pstrSelectID;
	CString *m_pstrSelectName;
	bool m_bInit;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_listComponents;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSearch();
};
