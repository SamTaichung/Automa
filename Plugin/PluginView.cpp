
// PluginView.cpp : CPluginView ���O����@
//

#include "stdafx.h"
// SHARED_HANDLERS �i�H�w�q�b��@�w���B�Y�ϩM�j�M�z�����B�z�`����
// ATL �M�פ��A�ä��\�P�ӱM�צ@�Τ��{���X�C
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

// CPluginView �غc/�Ѻc

CPluginView::CPluginView()
	: CFormView(IDD_PLUGIN_FORM)
{
	// TODO: �b���[�J�غc�{���X

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
	// TODO: �b���g�ѭק� CREATESTRUCT cs 
	// �F��ק�������O�μ˦����ت�

	return CFormView::PreCreateWindow(cs);
}

void CPluginView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CPluginView �E�_

#ifdef _DEBUG
void CPluginView::AssertValid() const
{
	CFormView::AssertValid();
}

void CPluginView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CPluginDoc* CPluginView::GetDocument() const // ���O�D��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPluginDoc)));
	return (CPluginDoc*)m_pDocument;
}
#endif //_DEBUG


// CPluginView �T���B�z�`��
