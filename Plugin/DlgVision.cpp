// DlgVision.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgVision.h"

// DlgVision 對話方塊

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


// DlgVision 訊息處理常式


BOOL DlgVision::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
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

