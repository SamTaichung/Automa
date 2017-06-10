// OpView.cpp : ¹ê§@ÀÉ
//

#include "stdafx.h"
#include "FormViewAutoma.h"

using namespace AutoUI;

// FormViewAutoma
#ifdef AUTOMAUI_EXPORTS
IMPLEMENT_DYNAMIC(FormViewAutoma, CFormView)
#endif
FormViewAutoma::FormViewAutoma(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{
	m_pMachine=NULL;
	m_pSplitter=NULL;
	m_BackColor = ::GetSysColor(COLOR_APPWORKSPACE);
}
FormViewAutoma::~FormViewAutoma()
{
}

BEGIN_MESSAGE_MAP(FormViewAutoma, CFormView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// FormViewAutoma ¶EÂ_

#ifdef _DEBUG
void FormViewAutoma::AssertValid() const
{
	CFormView::AssertValid();
}

void FormViewAutoma::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG
int FormViewAutoma::GetIconSize()
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
void FormViewAutoma::SetViewID(int iViewID)
{
	 m_ViewIDofSpliter=iViewID;
}
void FormViewAutoma::SetMachine(MachineBase* pM)
{
	m_pMachine = pM;
}
void FormViewAutoma::ChangeLanguage()
{
}

void FormViewAutoma::SetSplitter(MultiViewSplitterAutoma* pSplitter)
{
	assert (pSplitter != NULL);
	m_pSplitter=pSplitter;
	return ;
}

CString FormViewAutoma::GetCurViewID(void)
{
	return m_csCurViewID;
}
void FormViewAutoma::SetCurViewID(CString csCurViewID)
{
	m_csCurViewID=csCurViewID;
}
void FormViewAutoma::SetBackColor(COLORREF color)
{
	m_BackColor = color;
}
COLORREF FormViewAutoma::GetBackColor()
{
	return m_BackColor;
}
void FormViewAutoma::OnDraw(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect, m_BackColor);
	
}
