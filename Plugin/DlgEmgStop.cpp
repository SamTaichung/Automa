// DlgEmgStop.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgEmgStop.h"
#include "afxdialogex.h"


// DlgEmgStop 對話方塊

IMPLEMENT_DYNAMIC(DlgEmgStop, CDialogEx)

DlgEmgStop::DlgEmgStop(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ESTOP, pParent)
{

}

DlgEmgStop::~DlgEmgStop()
{
}

void DlgEmgStop::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_EMGSTOP, m_stcEmgStop);
}


BEGIN_MESSAGE_MAP(DlgEmgStop, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// DlgEmgStop 訊息處理常式


void DlgEmgStop::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	m_bBlinking = !m_bBlinking;
	Invalidate();

	CDialogEx::OnTimer(nIDEvent);
}


BOOL DlgEmgStop::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetTimer(0, 300, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
}


HBRUSH DlgEmgStop::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	int CtrlID = pWnd->GetDlgCtrlID();
	if (CtrlID == IDC_STATIC_EMGSTOP)
	{
		pDC->SetBkMode(TRANSPARENT);
		if (m_bBlinking)
		{
			pDC->SetTextColor(RGB(0, 128, 128));
		}
		else {
			pDC->SetTextColor(RGB(0, 0, 0));
		}
		hbr = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	return hbr;
}


void DlgEmgStop::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此加入您的訊息處理常式程式碼
					   // 不要呼叫圖片訊息的 CDialogEx::OnPaint()

	CRect rect;
	GetClientRect(&rect);
	if (m_bBlinking)
	{
		dc.FillSolidRect(&rect, RGB(128, 0, 0));
	}
	else {
		dc.FillSolidRect(&rect, RGB(192, 128, 0));
	}
}
