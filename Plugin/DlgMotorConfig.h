#pragma once
#include "Motor.h"
#include "afxwin.h"
#include "EditAutoma.h"
using namespace AutoLib;
using namespace AutoUI;
// DlgMotorConfig 對話方塊

class DlgMotorConfig : public CDialog
{
	DECLARE_DYNAMIC(DlgMotorConfig)

public:
	DlgMotorConfig(CWnd* pParent,MMotor* pMotor);   // 標準建構函式
	virtual ~DlgMotorConfig();

// 對話方塊資料
	enum { IDD = IDD_DLG_MOTOR_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	MMotor *m_pMotor;
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbPlsMode;
	CComboBox m_cmbCountSource;
	CComboBox m_cmbEncoder;
	CComboBox m_cmbINPLogic;
	CComboBox m_cmbINPEnable;
	CComboBox m_cmbALMLogic;
	CComboBox m_cmbALMMode;
	CComboBox m_cmbELLogic;
	CComboBox m_cmbORGLogic;
	CComboBox m_cmbLTCMode;
	CComboBox m_cmbLTCLogic;
	CComboBox m_cmbSDLogic;
	CComboBox m_cmbSDMode;
	CComboBox m_cmbSVONLogic;
	CComboBox m_cmbEZLogic;
	virtual BOOL OnInitDialog();
	CComboBox m_cmbEncLogic;
	afx_msg void OnBnClickedOk();
	CComboBox m_cmbHomeMode;
	CComboBox m_cmbAxisDir;
	CComboBox m_cmbHomeDir;
	EditAutoma m_editHomeSpeed;
	EditAutoma m_editHomeAccTime;
	EditAutoma m_editHomeDesTime;
	EditAutoma m_editUnitRev;
	EditAutoma m_editPulseRev;
};
