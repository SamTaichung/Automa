// FormViewTop.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "FormViewTop.h"
#include "DlgSystemStatus.h"

// FormViewTop

IMPLEMENT_DYNCREATE(FormViewTop, FormViewAutoma)

FormViewTop::FormViewTop()
	: FormViewAutoma(IDD_TOPVIEW)
{
	m_pImageList = new CImageList();

}

FormViewTop::~FormViewTop()
{
	delete m_pImageList;
}

void FormViewTop::DoDataExchange(CDataExchange* pDX)
{
	FormViewAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG, m_listMessage);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_stcLogo);
}

BEGIN_MESSAGE_MAP(FormViewTop, CFormView)
	ON_BN_CLICKED(IDC_BTN_STATUS, &FormViewTop::OnBnClickedBtnStatus)
END_MESSAGE_MAP()


// FormViewTop 診斷

#ifdef _DEBUG
void FormViewTop::AssertValid() const
{
	FormViewAutoma::AssertValid();
}

#ifndef _WIN32_WCE
void FormViewTop::Dump(CDumpContext& dc) const
{
	FormViewAutoma::Dump(dc);
}
#endif
#endif //_DEBUG


// FormViewTop 訊息處理常式
void FormViewTop::MachineMessage(MMessage *pMsg)
{
	CTime t;
	CString msg;
	int i;
	t = CTime::GetCurrentTime();
	msg = t.Format(_T("%H:%M:%S")) + _T(" ") + pMsg->strMessage;
	i = m_listMessage.GetItemCount();
	m_listMessage.InsertItem(i, msg, pMsg->MsgLevel);
	if (m_listMessage.GetItemCount()>500)
	{
		for (i = 0;i<20;i++)
		{
			m_listMessage.DeleteItem(i);
		}
	}
	m_listMessage.Scroll(CSize(0, 20));
}
void FormViewTop::OnInitialUpdate()
{
	FormViewAutoma::OnInitialUpdate();
	RECT rect;
	m_listMessage.GetClientRect(&rect);
	int Width = rect.right - rect.left;
	int inconsize = GetIconSize();
	m_pImageList->Create(inconsize, inconsize, ILC_COLOR8, 2, 2);
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GON));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GOFF));
	m_pImageList->SetBkColor(CLR_NONE);
	//	m_listMotors.SubclassDlgItem(IDC_LIST_MOTORS, this);
	m_listMessage.SetImageList(m_pImageList, LVSIL_SMALL);
	m_listMessage.InsertColumn(0, _T("訊息內容"), LVCFMT_LEFT, Width*0.95);
}


void FormViewTop::OnBnClickedBtnStatus()
{
	DlgSystemStatus *pDlg = new DlgSystemStatus(this,m_pMachine);
	pDlg->DoModal();
	delete pDlg;
}
