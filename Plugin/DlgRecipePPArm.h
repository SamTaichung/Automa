#pragma once
#include "TabPageAutoma.h"
#include "Machine.h"
#include "ListCtrlEx.h"
#include "EditAutoma.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "DlgComponentSearch.h"
// DlgRecipePPArm 對話方塊
using namespace AutoUI;

class DlgRecipePPArm : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgRecipePPArm)

public:
	DlgRecipePPArm(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~DlgRecipePPArm();
	static BOOL InitEditor_Arm(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);
	static BOOL InitEditor_Feeder(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);
	static BOOL EndEditor_Arm(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);
	static BOOL EndEditor_Feeder(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RECIPE_PPARM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	DECLARE_MESSAGE_MAP()
	void MachineMessage(MMessage *pMsg);
public:
	CListCtrlEx m_listPPArms;
	CListCtrlEx m_listFeeders;
	virtual BOOL OnInitDialog();
	CEdit m_editValue;
	CComboBox m_cmbType;
	CComboBox m_cmbFeeder;
	afx_msg void OnNMRClickListFeeder(NMHDR *pNMHDR, LRESULT *pResult);
};
