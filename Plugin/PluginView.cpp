
// PluginView.cpp : CPluginView 類別的實作
//

#include "stdafx.h"
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "Plugin.h"
#endif

#include "PluginDoc.h"
#include "PluginView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPluginView

IMPLEMENT_DYNCREATE(CPluginView, CFormView)

BEGIN_MESSAGE_MAP(CPluginView, CFormView)
END_MESSAGE_MAP()

// CPluginView 建構/解構

CPluginView::CPluginView()
	: CFormView(IDD_PLUGIN_FORM)
{
	// TODO: 在此加入建構程式碼

}

CPluginView::~CPluginView()
{
}

void CPluginView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CPluginView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return CFormView::PreCreateWindow(cs);
}

void CPluginView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CPluginView 診斷

#ifdef _DEBUG
void CPluginView::AssertValid() const
{
	CFormView::AssertValid();
}

void CPluginView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CPluginDoc* CPluginView::GetDocument() const // 內嵌非偵錯版本
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPluginDoc)));
	return (CPluginDoc*)m_pDocument;
}
#endif //_DEBUG


// CPluginView 訊息處理常式
