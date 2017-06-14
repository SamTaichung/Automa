#pragma once
#include "TabPageAutoma.h"
#include "EditAutoma.h"
#include "Machine.h"
#include "ListCtrlEx.h"
#include "afxwin.h"
using namespace AutoUI;

// DlgComponents 對話方塊

class DlgComponents : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgComponents)

public:
	DlgComponents(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~DlgComponents();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_COMPONENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	void RefreshPage();
	void SetMachine(MachineBase *pM);
	void MachineMessage(MMessage *pMsg);
	void ReloadData();
	void RefreshBtns();
	void OnSelectComponent();
	bool m_bAddMode;
	bool m_bEditMode;
	int m_intSelectIndex;
	CListCtrlEx m_listComponents;
	ComponentData *m_pComponent;
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	CEdit m_editID;
	CEdit m_editName;
	CButton m_btnSave;
	CButton m_btnCancel;
	CButton m_btnAdd;
	CButton m_btnDel;
	afx_msg void OnLvnItemchangedListComponents(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnEnSetfocusEditItem();
	EditAutoma m_editHigh,m_editPickAngle;
	double m_dblHigh;
	double m_dblPickAngle;
};
