#pragma once
#include "TabPageAutoma.h"
#include "ListCtrlEx.h"
#include "ButtonAutoma.h"
#include "EditAutoma.h"
#include "afxwin.h"
// DlgConfigMotor 對話方塊
using namespace AutoUI;
class DlgConfigMotor : public TabPageAutoma
{
	DECLARE_DYNAMIC(DlgConfigMotor)
public:
	DlgConfigMotor(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~DlgConfigMotor();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MOTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	DECLARE_MESSAGE_MAP()
	MMotor *m_pSelectedMotor;
	void OnSelectMotor(MMotor *pM);
public:
	CImageList *m_pImageList ;
	CStatic m_stcPos;
	CStatic m_stcLast;
	CStatic m_stcSpeed;
	EditAutoma m_editAMove;
	EditAutoma m_editRMove;
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnHome();
	CListCtrlEx m_listMotors;
	afx_msg void OnLvnItemchangedListMotors(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	void RefreshPage();
	void SetMachine(MachineBase* pM);
	void MachineMessage(MMessage *pMsg);
	EditAutoma m_editP1,m_editP2;
	afx_msg void OnBnClickedBtnTest();
	ButtonAutoma m_btnPitch[6];
	afx_msg void OnBnClickedBtnPitch0();
	afx_msg void OnBnClickedBtnPitch1();
	afx_msg void OnBnClickedBtnPitch2();
	afx_msg void OnBnClickedBtnPitch3();
	afx_msg void OnBnClickedBtnPitch4();
	afx_msg void OnBnClickedBtnPitch5();
	void PitchSelectChange(int index);
	afx_msg void OnBnClickedBtnRmovep();
	afx_msg void OnBnClickedBtnRmovem();
	void PitchMove(bool bPMove);
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnP1();
	afx_msg void OnBnClickedBtnP2();
	EditAutoma m_editHiSpeed,m_editLoSpeed;
	EditAutoma m_editHiAccTime, m_editHiDesTime;
	EditAutoma m_editLoAccTime, m_editLoDesTime;
};
