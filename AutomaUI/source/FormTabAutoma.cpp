// OpView.cpp : ¹ê§@ÀÉ
//

#include "stdafx.h"
#include "FormTabAutoma.h"

using namespace AutoUI;

// FormTabAutoma
#ifdef AUTOMAUI_EXPORTS
IMPLEMENT_DYNAMIC(FormTabAutoma, FormViewAutoma)
#endif
FormTabAutoma::FormTabAutoma(UINT nIDTemplate,UINT nIDTab)
	: FormViewAutoma(nIDTemplate),m_TabID(nIDTab)
{
	
}
FormTabAutoma::~FormTabAutoma()
{
}
void FormTabAutoma::DoDataExchange(CDataExchange* pDX)   // DDX/DDV ¤ä´©
{
	FormViewAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, m_TabID, m_TabControl);

}
BEGIN_MESSAGE_MAP(FormTabAutoma, FormViewAutoma)
END_MESSAGE_MAP()

// FormTabAutoma ¶EÂ_

#ifdef _DEBUG
void FormTabAutoma::AssertValid() const
{
	FormViewAutoma::AssertValid();
}

void FormTabAutoma::Dump(CDumpContext& dc) const
{
	FormViewAutoma::Dump(dc);
}
#endif //_DEBUG
void FormTabAutoma::AddPage(TabPageAutoma *pDlg)
{
	m_TabControl.AddPage(pDlg);
	pDlg->SetMachine(m_pMachine);
}
void FormTabAutoma::SetMachine(MachineBase* pM)
{
	m_pMachine = pM;
	m_TabControl.SetMachine(pM);
}
void FormTabAutoma::ChangeLanguage()
{
	FormViewAutoma::ChangeLanguage();
}
void FormTabAutoma::MachineMessage(MMessage *pMsg)
{
	m_TabControl.MachineMessage(pMsg);
}
void FormTabAutoma::RefreshForm()
{
	m_TabControl.RefreshPages();
}
