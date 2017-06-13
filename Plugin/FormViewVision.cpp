// FormViewVision.cpp : ��@��
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


// FormViewVision �E�_

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


// FormViewVision �T���B�z�`��
void FormViewVision::OnInitialUpdate()
{
	FormTabAutoma::OnInitialUpdate();
	m_pageVision.Create(IDD_DLG_RECIPE, this);
	m_pageVision.SetWindowTextW(_T("Vision"));
	AddPage(&m_pageVision);
}
