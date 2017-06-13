// TabPageAutoma.cpp : 實作檔
//

#include "stdafx.h"
#include "TabPageAutoma.h"
#include "afxdialogex.h"
using namespace AutoUI;
using namespace AutoLib;

// TabPageAutoma 對話方塊

IMPLEMENT_DYNAMIC(TabPageAutoma, CDialog)

TabPageAutoma::TabPageAutoma(UINT id,CWnd* pParent /*=NULL*/)
	: CDialog(id, pParent)
{
	m_pMachine = NULL;
	m_bActive = false;
}

TabPageAutoma::~TabPageAutoma()
{
}

void TabPageAutoma::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void TabPageAutoma::SetMachine(MachineBase* pM)
{
	m_pMachine = pM;
}
bool TabPageAutoma::isActive()
{
	return m_bActive;
}
void TabPageAutoma::SetActive(bool bActive)
{
	m_bActive = bActive;
}
//void TabPageAutoma::OnCancel()
//{
//}
//void TabPageAutoma::OnOK()
//{
//}

BEGIN_MESSAGE_MAP(TabPageAutoma, CDialog)
END_MESSAGE_MAP()

BOOL TabPageAutoma::PreTranslateMessage(MSG* pMsg)
{
	// 要繞過CDialog否則VK_RETURN會被當ON_OK處理
	return CWnd::PreTranslateMessage(pMsg);
}
int TabPageAutoma::GetIconSize()
{
	RECT rect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
	if ((rect.bottom - rect.top) >= 1900)
	{
		return 64;
	}
	else{
		if ((rect.bottom - rect.top) >= 1600)
		{
			return 48;
		}
	}
	return 32;
}
// TabPageAutoma 訊息處理常式
