// FormViewProduction.cpp : ��@��
//

#include "stdafx.h"
#include "Plugin.h"
#include "FormViewProduction.h"


// FormViewProduction

IMPLEMENT_DYNCREATE(FormViewProduction, FormViewAutoma)

FormViewProduction::FormViewProduction()
	: FormViewAutoma(IDD_PRODUCTIONVIEW)
{

}

FormViewProduction::~FormViewProduction()
{
}

void FormViewProduction::DoDataExchange(CDataExchange* pDX)
{
	FormViewAutoma::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FormViewProduction, CFormView)
END_MESSAGE_MAP()


// FormViewProduction �E�_

#ifdef _DEBUG
void FormViewProduction::AssertValid() const
{
	FormViewAutoma::AssertValid();
}

#ifndef _WIN32_WCE
void FormViewProduction::Dump(CDumpContext& dc) const
{
	FormViewAutoma::Dump(dc);
}
#endif
#endif //_DEBUG


// FormViewProduction �T���B�z�`��
