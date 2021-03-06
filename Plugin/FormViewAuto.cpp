// FormViewAuto.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "FormViewAuto.h"

using namespace AutoUI;
// FormViewAuto

IMPLEMENT_DYNCREATE(FormViewAuto, FormTabAutoma)

FormViewAuto::FormViewAuto()
	: FormTabAutoma(IDD_AUTOVIEW, IDC_TAB_DISPLAY)

{

}

FormViewAuto::~FormViewAuto()
{
}

void FormViewAuto::DoDataExchange(CDataExchange* pDX)
{
	FormTabAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_AUTO, m_stcPicture);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
}

BEGIN_MESSAGE_MAP(FormViewAuto, CFormView)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_PICTURE_AUTO, &FormViewAuto::OnStnClickedPictureAuto)
	ON_BN_CLICKED(IDC_BTN_START, &FormViewAuto::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// FormViewAuto 診斷

#ifdef _DEBUG
void FormViewAuto::AssertValid() const
{
	FormTabAutoma::AssertValid();
}

#ifndef _WIN32_WCE
void FormViewAuto::Dump(CDumpContext& dc) const
{
	FormTabAutoma::Dump(dc);
}
#endif
#endif //_DEBUG


// FormViewAuto 訊息處理常式

void FormViewAuto::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_stcPicture.LoadPictureFile(m_pMachine->GetAppPath() + _T("\\Machine\\Plugin.bmp"));
	// 不要呼叫圖片訊息的 FormViewAutoma::OnPaint()
}


void FormViewAuto::OnStnClickedPictureAuto()
{
	// TODO: 在此加入控制項告知處理常式程式碼
}


void FormViewAuto::OnBnClickedBtnStart()
{
	m_pMachine->AutoStart();
}


void FormViewAuto::OnInitialUpdate()
{
	FormTabAutoma::OnInitialUpdate();

	m_pageMotor.Create(IDD_DLG_AUTO_MOTORS, this);
	m_pageMotor.SetWindowTextW(_T("Motor"));
	AddPage(&m_pageMotor);

}
