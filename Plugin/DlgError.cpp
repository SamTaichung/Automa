// DlgError.cpp : 實作檔
//

#include "stdafx.h"
#include "DlgError.h"
#include "afxdialogex.h"

// DlgError 對話方塊
using namespace AutoLib;
IMPLEMENT_DYNAMIC(DlgError, CDialogEx)

DlgError::DlgError(CWnd* pParent,MError* pError)
	: CDialogEx(IDD_DLG_ERROR, pParent),m_pError(pError)
{

}

DlgError::~DlgError()
{
}

void DlgError::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DESCRIPT, m_stcDescript);
	DDX_Control(pDX, IDC_COMBO_SOLUTIONS, m_cmbSolutions);
	DDX_Control(pDX, IDC_STATIC_HAPPENER, m_stcHappener);
	DDX_Control(pDX, IDC_STATIC_MEMO, m_stcMemo);
}


BEGIN_MESSAGE_MAP(DlgError, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgError::OnBnClickedOk)
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL DlgError::OnInitDialog()
{
	MSolution *pSolution;
	std::vector<MSolution*>::iterator itSolution;
	CDialogEx::OnInitDialog();
	SetWindowPos(&CDialog::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	if (m_pError != NULL)
	{
		CString strTmp;
		strTmp = m_pError->m_Happener->GetFullName();
		m_stcHappener.SetWindowText(strTmp);
		strTmp=m_pError->m_strDescript;
		m_stcDescript.SetWindowText(strTmp);
		strTmp =m_pError->m_strReason;
		m_stcMemo.SetWindowText(strTmp);
		if (!m_pError->m_Solutions.empty())	//沒有Solution
		{
			for (int i = 0;i < m_pError->m_Solutions.size();i++)
			{
				pSolution = *(m_pError->m_Solutions.begin() + i);
				m_cmbSolutions.AddString(pSolution->GetDescription());
			}
		}
		m_cmbSolutions.AddString(_T("停機/STOP"));
		if (m_cmbSolutions.GetCount())
		{
			m_cmbSolutions.SetCurSel(0);
			m_cmbSolutions.SetFocus();
		}
	}
	return TRUE;
}
void DlgError::OnBnClickedOk()
{
	MSolution *pSolution;
	std::vector<MSolution*>::iterator itSolution;
	int iSel;
	if (m_pError != NULL)
	{
		iSel = m_cmbSolutions.GetCurSel();
		if (iSel >= m_pError->m_Solutions.size())
		{
			m_pError->m_isStop = true;
		}else{
			itSolution = m_pError->m_Solutions.begin();
			pSolution = *(itSolution + iSel);
			m_pError->m_pSelectedSolution = pSolution;
		}
	}
	CDialogEx::OnOK();
}


void DlgError::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void DlgError::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此加入您的訊息處理常式程式碼
					   // 不要呼叫圖片訊息的 CDialogEx::OnPaint()
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(&rect, RGB(64,0,0));

}


HBRUSH DlgError::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此變更 DC 的任何屬性
	int CtrlID = pWnd->GetDlgCtrlID();
	switch (CtrlID)
	{
	case IDC_STATIC_HAPPENER_CAP:
	case IDC_STATIC_DESCRIPT_CAP:
	case IDC_STATIC_MEMO_CAP:
	case IDC_STATIC_SOLUTION_CAP:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255, 255, 0));
			hbr = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		}
		break;
	case IDC_STATIC_HAPPENER:
	case IDC_STATIC_DESCRIPT:
	case IDC_STATIC_MEMO:
		{
			pDC->SetBkMode(OPAQUE);
			pDC->SetTextColor(RGB(0, 0, 192));
			hbr = (HBRUSH)GetStockObject(DC_BRUSH);
		}	
		break;
	default:
		break;
	}
	// TODO:  如果預設值並非想要的，則傳回不同的筆刷
	return hbr;
}


void DlgError::OnCancel()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

//	CDialogEx::OnCancel();
}
