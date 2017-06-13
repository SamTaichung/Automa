#pragma once
#include "TabPageAutoma.h"
#include "Machine.h"
#include "ListCtrlEx.h"
#include "EditAutoma.h"
#include "afxcmn.h"
#include "afxwin.h"
// DlgConfigPilot 對話方塊
using namespace AutoUI;

class DlgConfigPilot : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgConfigPilot)

public:
	DlgConfigPilot(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~DlgConfigPilot();
	static BOOL InitEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);
	static BOOL EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PILOT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	DECLARE_MESSAGE_MAP()
	void RefreshPage();
	void SetMachine(MachineBase *pM);
	void MachineMessage(MMessage *pMsg);
public:
	CImageList *m_pImageList;
	CListCtrlEx m_listState;
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListTimers(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_cmbSV;
	CComboBox m_cmbBuzz;
};
