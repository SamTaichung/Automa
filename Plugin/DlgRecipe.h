#pragma once
#include "TabPageAutoma.h"
#include "Machine.h"
#include "ListCtrlEx.h"
#include "EditAutoma.h"
#include "afxcmn.h"
#include "DlgComponentSearch.h"
#include "afxwin.h"
// DlgRecipe 對話方塊
using namespace AutoUI;

class DlgRecipe : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgRecipe)

public:
	DlgRecipe(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~DlgRecipe();
	static BOOL InitEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);
	static BOOL EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RECIPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	DECLARE_MESSAGE_MAP()
	void RefreshPage();
	void ReLoadData();
	void MachineMessage(MMessage *pMsg);
public:
	CListCtrlEx m_listRecipes;
	virtual BOOL OnInitDialog();
	CEdit m_editValue;
	afx_msg void OnLvnItemchangedListTimers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListRecipes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnNMRClickListRecipes(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
private:
	CMenu m_PopupMenu;
	int m_SelectItemIndex;
};
