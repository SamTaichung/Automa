// FormViewManual.cpp : ��@��
//

#include "stdafx.h"
#include "Plugin.h"
#include "FormViewManual.h"


// FormViewManual

IMPLEMENT_DYNCREATE(FormViewManual, FormViewAutoma)

FormViewManual::FormViewManual()
	: FormViewAutoma(IDD_MANUALVIEW)
{

}

FormViewManual::~FormViewManual()
{
}

void FormViewManual::DoDataExchange(CDataExchange* pDX)
{
	FormViewAutoma::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FormViewManual, CFormView)
END_MESSAGE_MAP()


// FormViewManual �E�_

#ifdef _DEBUG
void FormViewManual::AssertValid() const
{
	FormViewAutoma::AssertValid();
}

#ifndef _WIN32_WCE
void FormViewManual::Dump(CDumpContext& dc) const
{
	FormViewAutoma::Dump(dc);
}
#endif
#endif //_DEBUG


// FormViewManual �T���B�z�`��
