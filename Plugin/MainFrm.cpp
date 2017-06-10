
// MainFrm.cpp : CMainFrame ���O����@
//

#include "stdafx.h"
#include "Plugin.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
using namespace AutoLib;
using namespace AutoUI;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_MACHINEMESSAGE, MachineMessage)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���A�C���ܾ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame �غc/�Ѻc

CMainFrame::CMainFrame()
{
	// TODO: �b���[�J������l�Ƶ{���X
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("�L�k�إߤu��C\n");
	//	return -1;      // �L�k�إ�
	//}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("�L�k�إߪ��A�C\n");
		return -1;      // �L�k�إ�
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �p�G�z���n�i�H�T�w�u��C�A�ЧR���o�T��
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);
	m_pMachine = new Machine(this);
	m_wndSplitter.SetMachine(m_pMachine);

	// error Message of  Panel Add to Statusbar 
	::SetTimer(this->m_hWnd, 1, 100, NULL);
	// disable Title Bar	DARMARK 0315
	//DWORD dwStyle = this->GetStyle();
	//this->ModifyStyle(dwStyle, 0, SWP_NOSIZE | SWP_NOMOVE);
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	CRect rect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rect,0);
	m_wndSplitter.m_bResizable = false;	//Mouse L Button Splitter Bar Move disable
	m_wndSplitter.CreateStatic(this, 3, 1);
	int intHigh,iScreenY;
	iScreenY = rect.Height();
	intHigh = (iScreenY-10)*0.68;
	m_wndSplitter.AddView(0, 0, VIEWID::TopView, RUNTIME_CLASS(FormViewTop), pContext, 100, iScreenY*0.18);
	m_wndSplitter.AddView(1, 0, VIEWID::ConfigView, RUNTIME_CLASS(FormViewConfig), pContext, 100, intHigh);
	m_wndSplitter.AddView(1, 0, VIEWID::VisionView, RUNTIME_CLASS(FormViewVision), pContext, 100, intHigh);
	m_wndSplitter.AddView(1, 0, VIEWID::RecipeView, RUNTIME_CLASS(FormViewRecipe), pContext, 100, intHigh);
	m_wndSplitter.AddView(1, 0, VIEWID::ProductionView, RUNTIME_CLASS(FormViewProduction), pContext, 100, intHigh);
	m_wndSplitter.AddView(1, 0, VIEWID::ManualView, RUNTIME_CLASS(FormViewManual), pContext, 100, intHigh);
	m_wndSplitter.AddView(1, 0, VIEWID::AutoView, RUNTIME_CLASS(FormViewAuto), pContext, 100, intHigh);
	m_wndSplitter.AddView(2, 0, VIEWID::BottomView, RUNTIME_CLASS(FormViewBottom), pContext);
	//return m_wndSplitter.Create(this,
	//	2, 2,               // TODO: �վ��ƦC�B��Ʀ檺�s��
	//	CSize(10, 10),      // TODO: �վ�̤p������j�p
	//	pContext);
	((FormViewAutoma*)m_wndSplitter.GetView(VIEWID::TopView))->SetBackColor(RGB(64, 64, 64));
//	((FormViewAutoma*)m_wndSplitter.GetView(VIEWID::AutoView))->SetBackColor(RGB(64, 64, 64));
	((FormViewAutoma*)m_wndSplitter.GetView(VIEWID::BottomView))->SetBackColor(RGB(64, 64, 64));

	return(true);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �b���g�ѭק� CREATESTRUCT cs 
	// �F��ק�������O�μ˦����ت�

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

// CMainFrame �E�_

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �T���B�z�`��
LRESULT CMainFrame::MachineMessage(WPARAM wParam, LPARAM lParam)
{
	MMessage *pMsg = (MMessage*)wParam;
	switch (pMsg->MsgType)
	{
	case MMessage::MESSAGETYPE::ShowError:
	{
		DlgError *pDlgError = new DlgError(this, (MError*)(pMsg->m_pPara));
		pDlgError->DoModal();
		delete pDlgError;
	}
	break;
	case MMessage::MESSAGETYPE::EM_Stop:
	{
		DlgEmgStop *pDlgEmgStop = new DlgEmgStop(this);
		pDlgEmgStop->DoModal();
		delete pDlgEmgStop;
	}
	break;
	default:
		{
			m_wndSplitter.MachineMessage(pMsg);
		}
		break;
	}
	delete pMsg; //�T���O�QNew �X�Ӫ��A�B�z������Delete
	return 0;
}

BOOL CMainFrame::DestroyWindow()
{
	// TODO: �b���[�J�S�w���{���X�M (��) �I�s�����O
	m_pMachine->SetShutDown();
	Sleep(100);
	delete m_pMachine;
	return CFrameWnd::DestroyWindow();
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	CString strScan;
	strScan.Format(L"ScanTime:%8.3fms", m_pMachine->GetScanTime());
	m_wndStatusBar.SetPaneText(0, strScan);
	m_wndSplitter.RefreshForm();
	CFrameWnd::OnTimer(nIDEvent);
}
