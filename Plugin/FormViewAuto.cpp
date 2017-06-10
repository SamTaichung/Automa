// FormViewAuto.cpp : ��@��
//

#include "stdafx.h"
#include "Plugin.h"
#include "FormViewAuto.h"


// FormViewAuto

IMPLEMENT_DYNCREATE(FormViewAuto, FormViewAutoma)

FormViewAuto::FormViewAuto()
	: FormViewAutoma(IDD_AUTOVIEW)
{

}

FormViewAuto::~FormViewAuto()
{
}

void FormViewAuto::DoDataExchange(CDataExchange* pDX)
{
	FormViewAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_AUTO, m_stcPicture);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
}

BEGIN_MESSAGE_MAP(FormViewAuto, CFormView)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_PICTURE_AUTO, &FormViewAuto::OnStnClickedPictureAuto)
	ON_BN_CLICKED(IDC_BTN_START, &FormViewAuto::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// FormViewAuto �E�_

#ifdef _DEBUG
void FormViewAuto::AssertValid() const
{
	FormViewAutoma::AssertValid();
}

#ifndef _WIN32_WCE
void FormViewAuto::Dump(CDumpContext& dc) const
{
	FormViewAutoma::Dump(dc);
}
#endif
#endif //_DEBUG


// FormViewAuto �T���B�z�`��

void FormViewAuto::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_stcPicture.LoadPictureFile(m_pMachine->GetAppPath() + _T("\\Machine\\Plugin.bmp"));
	// ���n�I�s�Ϥ��T���� FormViewAutoma::OnPaint()
}


void FormViewAuto::OnStnClickedPictureAuto()
{
	// TODO: �b���[�J����i���B�z�`���{���X
}


void FormViewAuto::OnBnClickedBtnStart()
{
	m_pMachine->AutoStart();
}
