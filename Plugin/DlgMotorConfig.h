#pragma once
#include "Motor.h"
#include "afxwin.h"
using namespace AutoLib;
// DlgMotorConfig ��ܤ��

class DlgMotorConfig : public CDialog
{
	DECLARE_DYNAMIC(DlgMotorConfig)

public:
	DlgMotorConfig(CWnd* pParent,MMotor* pMotor);   // �зǫغc�禡
	virtual ~DlgMotorConfig();

// ��ܤ�����
	enum { IDD = IDD_DLG_MOTOR_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
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
};
