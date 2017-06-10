#pragma once
#include "afxcmn.h"
#include "OPView.h"
// VIOView ����˵�
using namespace MacLib;
class VIOView : public VOPView
{
	DECLARE_DYNCREATE(VIOView)

protected:
	VIOView();           // �ʺA�إߩҨϥΪ��O�@�غc�禡
	virtual ~VIOView();

public:
	enum { IDD = IDD_IOVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void ExecuteMenu();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	bool isLoadIOs;
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	CListCtrl m_List;
	CImageList *m_pImageList;
	virtual void OnInitialUpdate();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void RefreshForm();	//��sForm�nOverride��function
};


