// OpView.cpp : 實作檔
//

#include "stdafx.h"
#include "OpView.h"
#include ".\opview.h"

#define ID_BTN_PREVIOUS		20001
#define ID_BTN_ENTER		20002
#define ID_BTN_TEMP			20003

//extern class CMainFrame;

using namespace AutoLib;

// VOPView
IMPLEMENT_DYNAMIC(VOPView, CFormView)

VOPView::VOPView(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{
	m_pMachine=NULL;
	m_isEnglish=false;
	m_pSplitter=NULL;
}
VOPView::~VOPView()
{
}
void VOPView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(VOPView, CFormView)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_MESSAGE(WM_MACHINEMESSAGE,OnMessageGet)
	ON_WM_GETDLGCODE()
//	ON_WM_SYSCOMMAND()
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// VOPView 診斷

#ifdef _DEBUG
void VOPView::AssertValid() const
{
	CFormView::AssertValid();
}

void VOPView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


void VOPView::OnShowWindow(BOOL bShow, UINT nStatus)
{
//	CFormView::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		
		InitialForm();
		SetTimer(TMR_FORMREFRESH,TMI_FORMREFRESH,NULL);
		SetTimer(TM_TIM1000,TM_TIM1000,NULL);
	}
	else
	{
		KillTimer(TMR_FORMREFRESH);
		KillTimer(TM_TIM1000);
	}

}

void VOPView::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==TMR_FORMREFRESH)
	{
		RefreshForm();
	}
	else if (nIDEvent==TM_TIM1000)
	{
		RefreshForm(1000);
	}

	CFormView::OnTimer(nIDEvent);
}

void VOPView::ShowObject(bool bShow)
{
	std::map<int,Position>::iterator pos;
	CWnd* pView;
	for(pos=m_ViewMap.begin();pos!=m_ViewMap.end();++pos)
	{
		pView=m_pSplitter->GetView(pos->second.iViewID);
		pView->EnableWindow(bShow);
	}
}
void VOPView::SetOPViewID(int iViewID)
{
	 m_ViewIDofSpliter=iViewID;
}
int VOPView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CFormView::OnCreate(lpCreateStruct) == -1) return -1;

	return 0;
}


LRESULT VOPView::OnMessageGet(WPARAM wParam,LPARAM lParam)
{
	MMessage *pMsg=(MMessage*) wParam;
	switch (pMsg->MsgType)
	{
		case MMessage::MESSAGETYPE::MachineComplete:
			m_pMachine=(MBase *)pMsg->m_pPara;
			InitialMachineOK();
			ChangeLanguage(m_isEnglish);
			break;
		case MMessage::MESSAGETYPE::ChangeLanguage:
			ChangeLanguage(((MBase *)pMsg->m_pPara)->m_isEnglish);
			break;
	}
	delete pMsg;
	return 0;
}



void VOPView::ChangeLanguage(const bool isEnglish)
{
	m_isEnglish=isEnglish;
}

UINT VOPView::OnGetDlgCode()
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	return CFormView::OnGetDlgCode();
}

void VOPView::SetSplitter(AW_CMultiViewSplitter* pSplitter)
{
	assert (pSplitter != NULL);
	m_pSplitter=pSplitter;
	return ;
}

CString VOPView::GetCurViewID(void)
{
	return m_csCurViewID;
}
void VOPView::SetCurViewID(CString csCurViewID)
{
	m_csCurViewID=csCurViewID;
}

