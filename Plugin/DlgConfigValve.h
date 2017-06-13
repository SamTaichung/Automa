#pragma once
#include "TabPageAutoma.h"
#include "ListCtrlEx.h"
#include "ButtonAutoma.h"
#include "EditAutoma.h"
#include "afxwin.h"
// DlgConfigValve 對話方塊
using namespace AutoUI;
class DlgConfigValve : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgConfigValve)
public:
	DlgConfigValve(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~DlgConfigValve();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_VALVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	DECLARE_MESSAGE_MAP()
	MValve *m_pSelectedValve;
	void OnSelectValve(MValve *pM);
public:
	CImageList *m_pImageList ;
	CStatic m_stcPos;
	afx_msg void OnBnClickedBtnStop();
	CListCtrlEx m_listValves;
	afx_msg void OnLvnItemchangedListValves(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	void RefreshPage();
	void SetMachine(MachineBase* pM);
	void MachineMessage(MMessage *pMsg);
//	EditAutoma m_editP1,m_editP2;
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnRepeat();
};
