#pragma once
#include "afxcmn.h"
#include "OPView.h"
// VIOView 表單檢視
using namespace MacLib;
class VIOView : public VOPView
{
	DECLARE_DYNCREATE(VIOView)

protected:
	VIOView();           // 動態建立所使用的保護建構函式
	virtual ~VIOView();

public:
	enum { IDD = IDD_IOVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void ExecuteMenu();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	bool isLoadIOs;
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	CListCtrl m_List;
	CImageList *m_pImageList;
	virtual void OnInitialUpdate();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void RefreshForm();	//更新Form要Override此function
};


