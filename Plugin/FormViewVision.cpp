// FormViewVision.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "FormViewVision.h"


// FormViewVision

IMPLEMENT_DYNCREATE(FormViewVision, FormTabAutoma)

FormViewVision::FormViewVision()
	: FormTabAutoma(IDD_VISIONVIEW, IDC_TAB_VISION)
{
}

FormViewVision::~FormViewVision()
{

}
BEGIN_MESSAGE_MAP(FormViewVision, CFormView)
END_MESSAGE_MAP()


// FormViewVision 診斷

#ifdef _DEBUG
void FormViewVision::AssertValid() const
{
	FormTabAutoma::AssertValid();
}

#ifndef _WIN32_WCE
void FormViewVision::Dump(CDumpContext& dc) const
{
	FormTabAutoma::Dump(dc);
}
#endif
#endif //_DEBUG


// FormViewVision 訊息處理常式
void FormViewVision::OnInitialUpdate()
{
	FormTabAutoma::OnInitialUpdate();
	m_pageVision.Create(IDD_DLG_RECIPE, this);
	m_pageVision.SetWindowTextW(_T("Vision"));
	AddPage(&m_pageVision);
}
