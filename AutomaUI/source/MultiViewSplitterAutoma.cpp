#include "stdafx.h"
#include "MultiViewSplitterAutoma.h"
#include "FormViewAutoma.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace AutoUI;
using namespace AutoLib;
IMPLEMENT_DYNAMIC(MultiViewSplitterAutoma, CSplitterWnd)

MultiViewSplitterAutoma::MultiViewSplitterAutoma()
{
	m_nIDCounter = 1;
	m_FromView=-1;
	//m_PreviousView=-1;
	m_bResizable = true;	//預設可拉動窗格大小.
}

MultiViewSplitterAutoma::~MultiViewSplitterAutoma()
{

}
void MultiViewSplitterAutoma::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	if (pDC != NULL)
	{
		pDC->FillSolidRect(&rect, RGB(0, 0, 128));
	}
	//CSplitterWnd::OnDrawSplitter(pDC, nType, rect);
}
void MultiViewSplitterAutoma::RefreshForm()
{
	map<int, CWnd*>::iterator itView;
	for (itView = m_mapIDViews.begin();itView != m_mapIDViews.end();itView++)
	{
		((FormViewAutoma*)((*itView).second))->RefreshForm();
	}
}
void MultiViewSplitterAutoma::SetMachine(MachineBase *pM)
{
	map<int, CWnd*>::iterator itView;
	for (itView = m_mapIDViews.begin();itView != m_mapIDViews.end();itView++)
	{
		((FormViewAutoma*)((*itView).second))->SetMachine(pM);
	}
}
void MultiViewSplitterAutoma::MachineMessage(MMessage *pMsg)
{
	map<int, CWnd*>::iterator itView;
	for (itView = m_mapIDViews.begin();itView != m_mapIDViews.end();itView++)
	{
		((FormViewAutoma*)((*itView).second))->MachineMessage(pMsg);
	}
}
int MultiViewSplitterAutoma::AddView(int nRow, int nCol,int NewViewID, CRuntimeClass * pViewClass, 
								   CCreateContext* pContext,int sx,int sy)
{
	// hide the current view of the pane
	int PreviousID = HideCurrentView(nRow, nCol);

	// create the new view, if fail, set the previous view current 
	if (CreateView(nRow, nCol, pViewClass, CSize(sx,sy), pContext) == 0)
	{
		if (PreviousID != -1)
			SetCurrentView(nRow, nCol, PreviousID);
		return -1;
	}

	// get and store the niew view
	CWnd* pNewWnd = GetPane(nRow, nCol);
	CPoint pane(nRow, nCol);
	long paneID = MAKELONG(pane.x,pane.y);
	m_mapViewPane.insert(map<int, long>::value_type(NewViewID,paneID));
	m_mapIDViews.insert(map<int, CWnd*>::value_type(NewViewID, pNewWnd));

	// set the new view current
	SetCurrentView(nRow, nCol, NewViewID);

	RedrawWindow();
	m_nIDCounter ++;
	((FormViewAutoma *)(pNewWnd))->SetViewID(NewViewID);
	((FormViewAutoma *)(pNewWnd))->SetSplitter(this);

	return NewViewID;
}

void MultiViewSplitterAutoma::ShowView(int nViewID)
{
	if (GetView(nViewID) == NULL)
		return;

	// find the pane containing the view 
	CPoint pane;
	GetPaneFromViewID(nViewID, &pane);

	// switch views
	HideCurrentView(pane.x, pane.y);
	SetCurrentView(pane.x, pane.y, nViewID);

	RecalcLayout();
}

CWnd* MultiViewSplitterAutoma::GetView(int nViewID)
{
	map<int, CWnd*>::iterator itView;
	itView = m_mapIDViews.find(nViewID);
	if(itView==m_mapIDViews.end())
		return NULL;
	else
		return (*itView).second;
}

CWnd* MultiViewSplitterAutoma::GetCurrentView(int nRow, int nCol, int * nCurID)
{
	long paneID = MAKELONG(nRow,nCol);
	
	map<long, int>::iterator itCur;
	itCur = m_mapCurrentViews.find(paneID);
	if (itCur == m_mapCurrentViews.end())
		return NULL;
	else
	{
		int PreviousID = (*itCur).second;
		*nCurID = PreviousID;
		return GetView(PreviousID);
	}
}

void MultiViewSplitterAutoma::SetCurrentView(int nRow, int nCol, int nViewID)
{
	long paneID = MAKELONG(nRow,nCol);

	map<long, int>::iterator itCur;
	itCur = m_mapCurrentViews.find(paneID);
	if (itCur != m_mapCurrentViews.end())
		(*itCur).second = nViewID;
	else
		m_mapCurrentViews.insert(map<long,int>::value_type(paneID,nViewID));

	CWnd * pView = GetView(nViewID);
	pView->SetDlgCtrlID(IdFromRowCol(nRow, nCol));
	pView->ShowWindow(SW_SHOW);
	CSplitterWnd::SetActivePane(nRow,nCol);
}

int MultiViewSplitterAutoma::HideCurrentView(int nRow, int nCol)
{
	int prevID;
	CWnd * pCurView = (CWnd *)GetCurrentView(nRow, nCol, &prevID);
	if (pCurView == NULL)
		return -1;
	else
	{
		pCurView->SetDlgCtrlID(0);
		pCurView->ShowWindow(SW_HIDE);	
		return prevID;
	}
}

void MultiViewSplitterAutoma::GetPaneFromViewID(int nViewID, CPoint * pane)
{
	map<int, long>::iterator itPane;
	itPane = m_mapViewPane.find(nViewID);
	if (itPane==m_mapViewPane.end())
	{
		pane = NULL;
		return;
	}
	long paneID = (*itPane).second;
	CPoint p(paneID);
	pane->x = p.x;
	pane->y = p.y;
}

//CSplitterWnd的窗格的尺寸變化,是由滑鼠左鍵下,拖拉(mousemove),左鍵上
// 三種訊息組合而成, 若要固定尺寸,只需吃掉左鍵下訊息即可.
// (必須 改呼叫 基礎類別CWnd::OnLButtonDown函數,這樣視窗的左鍵可保有正常功能)
void MultiViewSplitterAutoma::OnLButtonDown(UINT uFlags, CPoint point)
{ 
	if (m_bResizable)	{
		CSplitterWnd::OnLButtonDown(uFlags, point);
	}else	{
		CWnd::OnLButtonDown(uFlags, point);
	}
} 

BEGIN_MESSAGE_MAP(MultiViewSplitterAutoma, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()






