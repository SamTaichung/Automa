// EditAutoma.cpp : 實作檔
//

#include "stdafx.h"
#include "EditAutoma.h"
using namespace AutoUI;

IMPLEMENT_DYNAMIC(EditAutoma, CEdit)

EditAutoma::EditAutoma()
{
	m_strFormat = _T("%10.4f");
	m_pdblValue = NULL;
}

EditAutoma::~EditAutoma()
{
}


BEGIN_MESSAGE_MAP(EditAutoma, CEdit)
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(EN_SETFOCUS, &EditAutoma::OnEnSetfocus)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &EditAutoma::OnEnKillfocus)
END_MESSAGE_MAP()

void EditAutoma::OnEnSetfocus()
{
	SetSel(0, -1);
}
void EditAutoma::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	if (nChar == VK_RETURN)
	{
		OnEnKillfocus();
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
void EditAutoma::OnEnKillfocus()
{
	CString strV;
	GetWindowText(strV);
	double dblV;
	dblV = _wtof(strV);
	if (m_pdblValue != NULL)
	{
		*m_pdblValue = dblV;
	}
	strV.Format(m_strFormat, dblV);
	SetWindowText(strV);
} 
void EditAutoma::SetValueExchange(double &pV)
{
	CString strV;
	m_pdblValue = &pV;
	if (m_pdblValue != NULL)
	{
		strV.Format(m_strFormat, *m_pdblValue);
		SetWindowText(strV);

	}
}
// EditAutoma 訊息處理常式


