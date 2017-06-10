#pragma once
#include "TabPageAutoma.h"
#include "Machine.h"
#include "EditAutoma.h"
#include "afxcmn.h"
#include "afxwin.h"
// DlgVision ��ܤ��
using namespace AutoUI;

class DlgVision : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgVision)

public:
	DlgVision(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~DlgVision();

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_VISION};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	DECLARE_MESSAGE_MAP()
	void RefreshPage();
	void SetMachine(MachineBase *pM);
	void MachineMessage(MMessage *pMsg);
public:
	virtual BOOL OnInitDialog();
	CEdit m_editValue;
};
