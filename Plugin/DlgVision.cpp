// DlgVision.cpp : ��@��
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgVision.h"

// DlgVision ��ܤ��

IMPLEMENT_DYNAMIC(DlgVision, CDialog)

DlgVision::DlgVision(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_TIMER)
{
}

DlgVision::~DlgVision()
{
}

void DlgVision::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(DlgVision, CDialog)
END_MESSAGE_MAP()


// DlgVision �T���B�z�`��


BOOL DlgVision::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}
void DlgVision::RefreshPage()
{
	CString strV;

}
void DlgVision::SetMachine(MachineBase *pM)
{
	TabPageAutoma::SetMachine(pM);
}

void DlgVision::MachineMessage(MMessage *pMsg)
{

}

