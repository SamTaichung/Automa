#pragma once
#include "TabPageAutoma.h"
#include "Machine.h"
#include "ListCtrlEx.h"
#include "afxcmn.h"

using namespace AutoUI;

// DlgConfigIO 對話方塊

class DlgConfigIO : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgConfigIO)

public:
	DlgConfigIO(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~DlgConfigIO();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_IO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	void RefreshPage();
	void SetMachine(MachineBase *pM);
	void MachineMessage(MMessage *pMsg);
public:
	CImageList *m_pImageList;
	CListCtrlEx m_listIO_X;
	CListCtrlEx m_listIO_Y;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListIoX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListIoY(NMHDR *pNMHDR, LRESULT *pResult);
};
