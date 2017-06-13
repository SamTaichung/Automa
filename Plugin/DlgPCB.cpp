// DlgPCB.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgPCB.h"

// DlgPCB 對話方塊
#define IDM_INSERT  4001
#define IDM_DELETE  4002
IMPLEMENT_DYNAMIC(DlgPCB, CDialog)

DlgPCB::DlgPCB(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_PCB)
{
}

DlgPCB::~DlgPCB()
{
}

void DlgPCB::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MARK_X1, m_editMarkX[0]);
	DDX_Control(pDX, IDC_EDIT_MARK_X2, m_editMarkX[1]);
	DDX_Control(pDX, IDC_EDIT_MARK_X3, m_editMarkX[2]);
	DDX_Control(pDX, IDC_EDIT_MARK_X4, m_editMarkX[3]);
	DDX_Control(pDX, IDC_EDIT_MARK_Y1, m_editMarkY[0]);
	DDX_Control(pDX, IDC_EDIT_MARK_Y2, m_editMarkY[1]);
	DDX_Control(pDX, IDC_EDIT_MARK_Y3, m_editMarkY[2]);
	DDX_Control(pDX, IDC_EDIT_MARK_Y4, m_editMarkY[3]);
	DDX_Control(pDX, IDC_EDIT_BASE_X, m_editBaseX);
	DDX_Control(pDX, IDC_EDIT_BASE_Y, m_editBaseY);
	DDX_Control(pDX, IDC_EDIT_BASE_Z, m_editBaseZ);
	DDX_Control(pDX, IDC_CMB_NX, m_cmbNX);
	DDX_Control(pDX, IDC_CMB_NY, m_cmbNY);
	DDX_Control(pDX, IDC_EDIT_PITCH_X, m_editPitchX);
	DDX_Control(pDX, IDC_EDIT_PITCH_Y, m_editPitchY);
	DDX_Control(pDX, IDC_STATIC_PCBPICT, m_stcPicture);
}

BEGIN_MESSAGE_MAP(DlgPCB, CDialog)
	ON_BN_CLICKED(IDC_BTN_SAVE, &DlgPCB::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &DlgPCB::OnBnClickedBtnCancel)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_CMB_NX, &DlgPCB::OnCbnSelchangeCmbNx)
	ON_CBN_SELCHANGE(IDC_CMB_NY, &DlgPCB::OnCbnSelchangeCmbNy)
END_MESSAGE_MAP()


// DlgPCB 訊息處理常式


BOOL DlgPCB::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strV;
	for (int i = 0; i < 10; i++)
	{
		strV.Format(_T("%2d"), i + 1);
		m_cmbNX.AddString(strV);
		m_cmbNY.AddString(strV);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
}
void DlgPCB::RefreshPage()
{

}
void DlgPCB::ReLoadData()
{
	if (m_pMachine != NULL)
	{
		Machine *pM = ((Machine*)m_pMachine);
		MConveyor* pCV = pM->m_pConveyor;
		PCBRecipe *pPCB = pCV->GetPCBRecipe();

		for (int i = 0; i < 4; i++)
		{
			m_editMarkX[i].SetValueExchange(pPCB->m_cdPCBMark[i].x);
			m_editMarkY[i].SetValueExchange(pPCB->m_cdPCBMark[i].y);
		}
		m_editBaseX.SetValueExchange(pPCB->m_cdBase.x);
		m_editBaseY.SetValueExchange(pPCB->m_cdBase.y);
		m_editBaseZ.SetValueExchange(pPCB->m_cdBase.z);
		m_editPitchX.SetValueExchange(pPCB->m_dblXPitch);
		m_editPitchY.SetValueExchange(pPCB->m_dblYPitch);
		m_cmbNX.SetCurSel(pPCB->m_intNumOfX-1);
		m_cmbNY.SetCurSel(pPCB->m_intNumOfY-1);
	}
}
void DlgPCB::MachineMessage(MMessage *pMsg)
{
	CString strMenu;
	switch (pMsg->MsgType)
	{
	case MMessage::MESSAGETYPE::MachineComplete:
		ReLoadData();
		break;
	}
}


void DlgPCB::OnBnClickedBtnSave()
{
	CString strV;
	Machine *pM = ((Machine*)m_pMachine);
	MConveyor* pCV = pM->m_pConveyor;
	pM->OpenRecipeMDB();
	pM->SaveRecipeData(pCV);
	pM->CloseRecipeMDB();
	ReLoadData();
}


void DlgPCB::OnBnClickedBtnCancel()
{
	Machine *pM = ((Machine*)m_pMachine);
	MConveyor* pCV = pM->m_pConveyor;
	pM->OpenRecipeMDB();
	pM->LoadRecipeData(pCV);
	pM->CloseRecipeMDB();
	ReLoadData();
}


void DlgPCB::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此加入您的訊息處理常式程式碼
	m_stcPicture.LoadPictureFile(m_pMachine->GetAppPath() + _T("\\Machine\\PCB.bmp"));
}


void DlgPCB::OnCbnSelchangeCmbNx()
{
	Machine *pM = ((Machine*)m_pMachine);
	MConveyor* pCV = pM->m_pConveyor;
	PCBRecipe* pPCB = pCV->GetPCBRecipe();
	pPCB->SetNumberOfX(m_cmbNX.GetCurSel() + 1);
}


void DlgPCB::OnCbnSelchangeCmbNy()
{
	Machine *pM = ((Machine*)m_pMachine);
	MConveyor* pCV = pM->m_pConveyor;
	PCBRecipe* pPCB = pCV->GetPCBRecipe();
	pPCB->SetNumberOfY(m_cmbNY.GetCurSel() + 1);
}
