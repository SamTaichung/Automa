#pragma once
#include "TabPageAutoma.h"
#include "Machine.h"
#include "ListCtrlEx.h"
#include "EditAutoma.h"
#include "afxcmn.h"
#include "afxwin.h"
// DlgConfigTimer ��ܤ��
using namespace AutoUI;

class DlgConfigTimer : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgConfigTimer)

public:
	DlgConfigTimer(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~DlgConfigTimer();
	static BOOL EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TIMER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	DECLARE_MESSAGE_MAP()
	void RefreshPage();
	void SetMachine(MachineBase *pM);
	void MachineMessage(MMessage *pMsg);
public:
	CImageList *m_pImageList;
	CListCtrlEx m_listTimers;
	virtual BOOL OnInitDialog();
	CEdit m_editValue;
	afx_msg void OnLvnItemchangedListTimers(NMHDR *pNMHDR, LRESULT *pResult);
};
