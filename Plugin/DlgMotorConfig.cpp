// DlgMotorConfig.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgMotorConfig.h"
#include "afxdialogex.h"


// DlgMotorConfig 對話方塊

IMPLEMENT_DYNAMIC(DlgMotorConfig, CDialog)

DlgMotorConfig::DlgMotorConfig(CWnd* pParent, MMotor* pMotor)
: CDialog(DlgMotorConfig::IDD, pParent), m_pMotor(pMotor)
{

}

DlgMotorConfig::~DlgMotorConfig()
{
}

void DlgMotorConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_PLSMODE, m_cmbPlsMode);
	DDX_Control(pDX, IDC_CMB_FEEDBACK, m_cmbCountSource);
	DDX_Control(pDX, IDC_CMB_ENCODER, m_cmbEncoder);
	DDX_Control(pDX, IDC_CMB_INPL, m_cmbINPLogic);
	DDX_Control(pDX, IDC_CMB_MINP, m_cmbINPEnable);
	DDX_Control(pDX, IDC_CMB_ALML, m_cmbALMLogic);
	DDX_Control(pDX, IDC_CMB_ALMM, m_cmbALMMode);
	DDX_Control(pDX, IDC_CMB_ELL, m_cmbELLogic);
	DDX_Control(pDX, IDC_CMB_ORGL, m_cmbORGLogic);
	DDX_Control(pDX, IDC_CMB_LTCSD, m_cmbLTCMode);
	DDX_Control(pDX, IDC_CMB_LTCL, m_cmbLTCLogic);
	DDX_Control(pDX, IDC_CMB_SDL, m_cmbSDLogic);
	DDX_Control(pDX, IDC_CMB_SDM, m_cmbSDMode);
	DDX_Control(pDX, IDC_CMB_SVL, m_cmbSVONLogic);
	DDX_Control(pDX, IDC_CMB_EZL, m_cmbEZLogic);
	DDX_Control(pDX, IDC_CMB_ENCL, m_cmbEncLogic);
}


BEGIN_MESSAGE_MAP(DlgMotorConfig, CDialog)
	ON_BN_CLICKED(IDOK, &DlgMotorConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgMotorConfig 訊息處理常式


BOOL DlgMotorConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strMode;

	// TODO:  在此加入額外的初始化
	for (int i = 0; i<8; i++)
	{
		strMode.Format(_T("%d"), i);
		this->m_cmbPlsMode.AddString(strMode);
	}
	strMode = _T("0:Encoder");
	this->m_cmbCountSource.AddString(strMode);
	strMode = _T("1:Command");
	this->m_cmbCountSource.AddString(strMode);
	strMode = _T("0:1X AB Phase");
	this->m_cmbEncoder.AddString(strMode);
	strMode = _T("1:2X AB Phase");
	this->m_cmbEncoder.AddString(strMode);
	strMode = _T("2:4X AB Phase");
	this->m_cmbEncoder.AddString(strMode);
	strMode = _T("3:CW/CCW");
	this->m_cmbEncoder.AddString(strMode);
	
	strMode = _T("0:Active LOW");
	this->m_cmbEncLogic.AddString(strMode);
	this->m_cmbINPLogic.AddString(strMode);
	this->m_cmbALMLogic.AddString(strMode);
	this->m_cmbELLogic.AddString(strMode);
	this->m_cmbORGLogic.AddString(strMode);
	this->m_cmbLTCLogic.AddString(strMode);
	this->m_cmbSDLogic.AddString(strMode);
	this->m_cmbSVONLogic.AddString(strMode);
	this->m_cmbEZLogic.AddString(strMode);
	strMode = _T("1:Active HIGH");
	this->m_cmbEncLogic.AddString(strMode);
	this->m_cmbINPLogic.AddString(strMode);
	this->m_cmbALMLogic.AddString(strMode);
	this->m_cmbELLogic.AddString(strMode);
	this->m_cmbORGLogic.AddString(strMode);
	this->m_cmbLTCLogic.AddString(strMode);
	this->m_cmbSDLogic.AddString(strMode);
	this->m_cmbSVONLogic.AddString(strMode);
	this->m_cmbEZLogic.AddString(strMode);
	strMode = _T("0:Disable");
	this->m_cmbINPEnable.AddString(strMode);
	strMode = _T("1:Enable");
	this->m_cmbINPEnable.AddString(strMode);
	strMode = _T("0:Immediate stop");
	this->m_cmbALMMode.AddString(strMode);
	strMode = _T("1:Deceleration stop");
	this->m_cmbALMMode.AddString(strMode);
	strMode = _T("0:Latch");
	this->m_cmbLTCMode.AddString(strMode);
	strMode = _T("1:SD");
	this->m_cmbLTCMode.AddString(strMode);
	strMode = _T("0:SlowDown");
	this->m_cmbSDMode.AddString(strMode);
	strMode = _T("1:SlowStop");
	this->m_cmbSDMode.AddString(strMode);
	m_cmbPlsMode.SetCurSel(m_pMotor->m_PulseMode);
	m_cmbCountSource.SetCurSel(m_pMotor->m_CountSource);
	m_cmbEncoder.SetCurSel(m_pMotor->m_EncoderMode);
	m_cmbEncLogic.SetCurSel(m_pMotor->m_EncoderLogic ? 1 : 0);
	m_cmbINPLogic.SetCurSel(m_pMotor->m_INPLogic ? 1 : 0);
	m_cmbINPEnable.SetCurSel(m_pMotor->m_INPEnable ? 1 : 0);
	m_cmbALMLogic.SetCurSel(m_pMotor->m_ALMLogic ? 1 : 0);
	m_cmbALMMode.SetCurSel(m_pMotor->m_ALMMode);
	m_cmbELLogic.SetCurSel(m_pMotor->m_PLimLogic ? 1 : 0);
	m_cmbORGLogic.SetCurSel(m_pMotor->m_OrgLogic ? 1 : 0);
	m_cmbLTCLogic.SetCurSel(m_pMotor->m_LTCLogic ? 1 : 0);
	m_cmbLTCMode.SetCurSel(m_pMotor->m_LTCMode);
	m_cmbSDLogic.SetCurSel(m_pMotor->m_SDLogic ? 1 : 0);
	m_cmbSDMode.SetCurSel(m_pMotor->m_SDMode);
	m_cmbSVONLogic.SetCurSel(m_pMotor->m_SVOnLogic ? 1 : 0);
	m_cmbEZLogic.SetCurSel(m_pMotor->m_EZLogic ? 1 : 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION:  OCX 屬性頁應傳回 FALSE
}


void DlgMotorConfig::OnBnClickedOk()
{
	m_pMotor->m_PulseMode=m_cmbPlsMode.GetCurSel();
	m_pMotor->m_CountSource=m_cmbCountSource.GetCurSel();
	m_pMotor->m_EncoderMode=m_cmbEncoder.GetCurSel();
	m_pMotor->m_EncoderLogic = m_cmbEncLogic.GetCurSel();
	m_pMotor->m_INPLogic = m_cmbINPLogic.GetCurSel();
	m_pMotor->m_INPEnable = m_cmbINPEnable.GetCurSel();
	m_pMotor->m_ALMLogic = m_cmbALMLogic.GetCurSel();
	m_pMotor->m_ALMMode = m_cmbALMMode.GetCurSel();
	m_pMotor->m_PLimLogic=m_cmbELLogic.GetCurSel();
	m_pMotor->m_OrgLogic=m_cmbORGLogic.GetCurSel();
	m_pMotor->m_LTCLogic=m_cmbLTCLogic.GetCurSel();
	m_pMotor->m_LTCMode=m_cmbLTCMode.GetCurSel();
	m_pMotor->m_SDLogic=m_cmbSDLogic.GetCurSel();
	m_pMotor->m_SDMode=m_cmbSDMode.GetCurSel();
	m_pMotor->m_SVOnLogic=m_cmbSVONLogic.GetCurSel();
	m_pMotor->m_EZLogic=m_cmbEZLogic.GetCurSel();
	CDialog::OnOK();
}
