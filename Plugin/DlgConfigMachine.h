#pragma once
#include "TabPageAutoma.h"
#include "PictureAutoma.h"
#include "Machine.h"
#include "ListCtrlEx.h"
#include "EditAutoma.h"
#include "afxcmn.h"
#include "DlgComponentSearch.h"
#include "afxwin.h"
// DlgConfigMachine ��ܤ��
using namespace AutoUI;

class DlgConfigMachine : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgConfigMachine)

public:
	DlgConfigMachine(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~DlgConfigMachine();
// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MACHINE};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
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
	EditAutoma m_editCCDX;
	EditAutoma m_editCCDY;
	EditAutoma m_editCCDZ;
	EditAutoma m_editFDX;
	EditAutoma m_editFDY;
	EditAutoma m_editFDZ;
	EditAutoma m_editARMX;
	PictureAutoma m_stcPicture;
	afx_msg void OnPaint();
};
