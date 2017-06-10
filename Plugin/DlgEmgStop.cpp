// DlgEmgStop.cpp : ��@��
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgEmgStop.h"
#include "afxdialogex.h"


// DlgEmgStop ��ܤ��

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


// DlgEmgStop �T���B�z�`��


void DlgEmgStop::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �b���[�J�z���T���B�z�`���{���X�M (��) �I�s�w�]��
	m_bBlinking = !m_bBlinking;
	Invalidate();

	CDialogEx::OnTimer(nIDEvent);
}


BOOL DlgEmgStop::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetTimer(0, 300, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
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
					   // TODO: �b���[�J�z���T���B�z�`���{���X
					   // ���n�I�s�Ϥ��T���� CDialogEx::OnPaint()

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
