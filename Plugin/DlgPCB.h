#pragma once
#include "TabPageAutoma.h"
#include "PictureAutoma.h"
#include "Machine.h"
#include "ListCtrlEx.h"
#include "EditAutoma.h"
#include "afxcmn.h"
#include "DlgComponentSearch.h"
#include "afxwin.h"
// DlgPCB 對話方塊
using namespace AutoUI;

class DlgPCB : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgPCB)

public:
	DlgPCB(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~DlgPCB();
// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PCB };
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
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnCancel();
	EditAutoma m_editMarkX[4];
	EditAutoma m_editMarkY[4];
	EditAutoma m_editBaseX;
	EditAutoma m_editBaseY;
	EditAutoma m_editBaseZ;
	CComboBox m_cmbNX;
	CComboBox m_cmbNY;
	EditAutoma m_editPitchX;
	EditAutoma m_editPitchY;
	PictureAutoma m_stcPicture;
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeCmbNx();
	afx_msg void OnCbnSelchangeCmbNy();
};
