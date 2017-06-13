// TabCtrlAutoma.cpp : 實作檔
//

#include "stdafx.h"
#include "TabCtrlAutoma.h"

using namespace AutoUI;
using namespace AutoLib;
// TabCtrlAutoma

IMPLEMENT_DYNAMIC(TabCtrlAutoma, CTabCtrl)

TabCtrlAutoma::TabCtrlAutoma()
{
	m_intCurrentActiveIndex = -1;
}

TabCtrlAutoma::~TabCtrlAutoma()
{
}
void TabCtrlAutoma::RefreshPages()
{
	for (int i = 0; i <= m_pageArray.GetUpperBound(); i++)
	{
		TabPageAutoma* pPage = m_pageArray.GetAt(i);
		if (pPage->isActive())
		{ 
			pPage->RefreshPage();
		}
	}
}
void TabCtrlAutoma::SetMachine(MachineBase *pM)
{
	for (int i = 0; i <= m_pageArray.GetUpperBound(); i++)
	{
		TabPageAutoma* pPage = m_pageArray.GetAt(i);
		pPage->SetMachine(pM);
	}
}
void TabCtrlAutoma::MachineMessage(MMessage *pMsg)
{
	for (int i = 0; i <= m_pageArray.GetUpperBound(); i++)
	{
		TabPageAutoma* pPage = m_pageArray.GetAt(i);
		pPage->MachineMessage(pMsg);
	}
}
void TabCtrlAutoma::AddPage(TabPageAutoma *pDlg)
{
	int index = GetItemCount();
	CString strCaption;
	pDlg->GetWindowTextW(strCaption);
	this->InsertItem(index, strCaption);
	m_pageArray.Add(pDlg);

	pDlg->SetParent(this); // Just to be sure

	CRect rect;
	GetClientRect(&rect);

	if (rect.Width() > 0 && rect.Height() > 0)
	{
		ResizeDialog(index, rect.Width(), rect.Height());
	}
	if (index == 0)
	{
		m_intCurrentActiveIndex = index;
		pDlg->ShowWindow(SW_SHOW);
		pDlg->SetActive(true);
	}
	else {
		pDlg->ShowWindow(SW_HIDE);
		pDlg->SetActive(false);
	}
}
void TabCtrlAutoma::ResizeDialog(int nIndex, int cx, int cy) {
	if (nIndex != -1) {
		CDialog* pDialog = m_pageArray[nIndex];

		if (pDialog != NULL) {
			CRect rect;
			GetItemRect(nIndex, &rect);

			int x, y, nWidth, nHeight;
			DWORD dwStyle = GetStyle();

			if (dwStyle & TCS_VERTICAL) { // Vertical tabs
				int nTabWidth =
					rect.Width() * GetRowCount();
				x = (dwStyle & TCS_RIGHT) ? 4 : nTabWidth + 4;
				y = 4;
				nWidth = cx - nTabWidth - 8;
				nHeight = cy - 8;
			}
			else { // Horizontal tabs
				int nTabHeight =
					rect.Height() * GetRowCount();
				x = 4;
				y = (dwStyle & TCS_BOTTOM) ? 4 : nTabHeight + 4;
				nWidth = cx - 8;
				nHeight = cy - nTabHeight - 8;
			}
			pDialog->SetWindowPos(NULL, x, y, nWidth, nHeight, SWP_NOZORDER);
		}
	}
}

BEGIN_MESSAGE_MAP(TabCtrlAutoma, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelChanging)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()



void TabCtrlAutoma::OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult) {
	// Notify derived classes that the selection is changing.
	int index = GetCurSel();
	if (index == -1)
		return;
	TabPageAutoma *pDlg = m_pageArray[index];
	pDlg->ShowWindow(SW_HIDE);
	pDlg->SetActive(false);
	*pResult = 0;
}

void TabCtrlAutoma::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult) {
	int index = GetCurSel();
	if (index == -1)
		return;
	TabPageAutoma *pDlg = m_pageArray[index];
	CRect rect;
	GetClientRect(&rect);
	ResizeDialog(index, rect.Width(), rect.Height());
	pDlg->ShowWindow(SW_SHOW);
	pDlg->SetActive(true);
	*pResult = 0;
}

// TabCtrlAutoma 訊息處理常式


