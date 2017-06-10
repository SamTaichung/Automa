#pragma once
#include "afxcmn.h"
#include "MachineBase.h"
#include "afxwin.h"
using namespace AutoLib;
class DlgSystemStatus : public CDialog
{
	DECLARE_DYNAMIC(DlgSystemStatus)

public:
	DlgSystemStatus(CWnd* pParent, MachineBase *pM);   // 標準建構函式
	virtual ~DlgSystemStatus();
protected:
	// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SYSTEMS_STATUS };
#endif
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	CImageList *m_pImageList;
	CTreeCtrl m_treeMap;
	MachineBase *m_pMachine;
	UINT m_uTreeCtrlID;
	void AddNode(MBase *pB, HTREEITEM ParentItem);
	void RefreshNode(HTREEITEM hItem);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
};
