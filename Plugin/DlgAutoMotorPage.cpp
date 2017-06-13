// DlgAutoMotorPage.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "Machine.h"
#include "DlgAutoMotorPage.h"
#include "afxdialogex.h"


// DlgAutoMotorPage 對話方塊

IMPLEMENT_DYNAMIC(DlgAutoMotorPage, CDialog)

DlgAutoMotorPage::DlgAutoMotorPage(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_AUTO_MOTORS)
{

}

DlgAutoMotorPage::~DlgAutoMotorPage()
{
}

void DlgAutoMotorPage::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_X_POS, m_stcXPos);
	DDX_Control(pDX, IDC_STC_Y_POS, m_stcYPos);
	DDX_Control(pDX, IDC_STC_Z1, m_stcZPos[0]);
	DDX_Control(pDX, IDC_STC_Z2, m_stcZPos[1]);
	DDX_Control(pDX, IDC_STC_Z3, m_stcZPos[2]);
	DDX_Control(pDX, IDC_STC_Z4, m_stcZPos[3]);
	DDX_Control(pDX, IDC_STC_Z5, m_stcZPos[4]);
	DDX_Control(pDX, IDC_STC_Z6, m_stcZPos[5]);
	DDX_Control(pDX, IDC_STC_R1, m_stcRPos[0]);
	DDX_Control(pDX, IDC_STC_R2, m_stcRPos[1]);
	DDX_Control(pDX, IDC_STC_R3, m_stcRPos[2]);
	DDX_Control(pDX, IDC_STC_R4, m_stcRPos[3]);
	DDX_Control(pDX, IDC_STC_R5, m_stcRPos[4]);
	DDX_Control(pDX, IDC_STC_R6, m_stcRPos[5]);
}
void DlgAutoMotorPage::RefreshPage()
{
	if (m_pMachine != NULL)
	{
		CString strV;
		Machine *pM = (Machine*)m_pMachine;
		if (pM->isInitComplete())
		{
			strV.Format(_T("%10.3f"), pM->m_pPickPlug->m_pXMotor->GetPosition());
			m_stcXPos.SetWindowText(strV);
			strV.Format(_T("%10.3f"), pM->m_pPickPlug->m_pYMotor->GetPosition());
			m_stcYPos.SetWindowText(strV);
			for (int i = 0; i < 6; i++)
			{
				strV.Format(_T("%10.3f"), pM->m_pPickPlug->m_pArm[i]->m_pZMotor->GetPosition());
				m_stcZPos[i].SetWindowText(strV);
				strV.Format(_T("%10.3f"), pM->m_pPickPlug->m_pArm[i]->m_pRMotor->GetPosition());
				m_stcRPos[i].SetWindowText(strV);

			}
		}
	}
}

BEGIN_MESSAGE_MAP(DlgAutoMotorPage, CDialog)
END_MESSAGE_MAP()


// DlgAutoMotorPage 訊息處理常式
