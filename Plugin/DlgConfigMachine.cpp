// DlgConfigMachine.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgConfigMachine.h"

// DlgConfigMachine 對話方塊
#define IDM_INSERT  4001
#define IDM_DELETE  4002
IMPLEMENT_DYNAMIC(DlgConfigMachine, CDialog)

DlgConfigMachine::DlgConfigMachine(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_PCB)
{
}

DlgConfigMachine::~DlgConfigMachine()
{
}

void DlgConfigMachine::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CCD_X, m_editCCDX);
	DDX_Control(pDX, IDC_EDIT_CCD_Y, m_editCCDY);
	DDX_Control(pDX, IDC_EDIT_CCD_Z, m_editCCDZ);
	DDX_Control(pDX, IDC_EDIT_FDX, m_editFDX);
	DDX_Control(pDX, IDC_EDIT_FDY, m_editFDY);
	DDX_Control(pDX, IDC_EDIT_FDZ, m_editFDZ);
	DDX_Control(pDX, IDC_STATIC_PCBPICT, m_stcPicture);
}

BEGIN_MESSAGE_MAP(DlgConfigMachine, CDialog)
	ON_BN_CLICKED(IDC_BTN_SAVE, &DlgConfigMachine::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &DlgConfigMachine::OnBnClickedBtnCancel)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// DlgConfigMachine 訊息處理常式


BOOL DlgConfigMachine::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
}
void DlgConfigMachine::RefreshPage()
{

}
void DlgConfigMachine::ReLoadData()
{
	if (m_pMachine != NULL)
	{
		Machine *pM = ((Machine*)m_pMachine);
		MPickPlug *pPP = pM->m_pPickPlug;

		m_editCCDX.SetValueExchange(pPP->m_cdCCD.x);
		m_editCCDY.SetValueExchange(pPP->m_cdCCD.y);
		m_editCCDZ.SetValueExchange(pPP->m_cdCCD.z);
		m_editFDX.SetValueExchange(pPP->m_cdFeederPos.x);
		m_editFDY.SetValueExchange(pPP->m_cdFeederPos.y);
		m_editFDZ.SetValueExchange(pPP->m_cdFeederPos.z);
	}
}
void DlgConfigMachine::MachineMessage(MMessage *pMsg)
{
	CString strMenu;
	switch (pMsg->MsgType)
	{
	case MMessage::MESSAGETYPE::MachineComplete:
		ReLoadData();
		break;
	}
}


void DlgConfigMachine::OnBnClickedBtnSave()
{
	CString strV;
	Machine *pM = ((Machine*)m_pMachine);
	MPickPlug *pPP = pM->m_pPickPlug;
	pM->OpenMachineMDB();
	pM->SaveMachineData(pPP);
	pM->CloseMachineMDB();
	ReLoadData();
}


void DlgConfigMachine::OnBnClickedBtnCancel()
{
	Machine *pM = ((Machine*)m_pMachine);
	MPickPlug *pPP = pM->m_pPickPlug;
	pM->OpenMachineMDB();
	pM->LoadMachineData(pPP);
	pM->CloseMachineMDB();
	ReLoadData();
}


void DlgConfigMachine::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此加入您的訊息處理常式程式碼
	m_stcPicture.LoadPictureFile(m_pMachine->GetAppPath() + _T("\\Machine\\Machine.bmp"));
}

