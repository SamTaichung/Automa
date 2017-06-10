// ButtonAutoma.cpp : implementation file
//

#include "stdafx.h"
#include "ButtonAutoma.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ButtonAutoma
using namespace AutoUI;

IMPLEMENT_DYNAMIC(ButtonAutoma, CButton)
ButtonAutoma::ButtonAutoma()
{
	m_color=RGB(220,220,220);
	m_allowChange = TRUE;
	m_selected = FALSE;
	m_CustomColors = NULL;
	m_backcolor=::GetSysColor(COLOR_APPWORKSPACE);

}

ButtonAutoma::~ButtonAutoma()
{
}


BEGIN_MESSAGE_MAP(ButtonAutoma, CButton)
	//{{AFX_MSG_MAP(ButtonAutoma)
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClickedEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ButtonAutoma message handlers

//This function is called when the item is drawn
void ButtonAutoma::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CClientDC dc(this); // device context for painting
	
	CRect rect, ignoreRect;
	GetClientRect(&rect);

	//First, fill with background. Don't overwrite the button
	ignoreRect = rect;
	ignoreRect.DeflateRect(2, 2);

	dc.SaveDC();
	dc.ExcludeClipRect(&ignoreRect);
	dc.FillSolidRect(&rect, m_backcolor);

	//Is in focus?
	if(::GetFocus() == m_hWnd)
	{
		//Draw focus rect two times. This is necessary to make the focus
		//rect visible when using a high contrast color scheme in windows
		COLORREF old = dc.SetBkColor(m_backcolor);
		dc.DrawFocusRect(&rect);
		dc.SetBkColor(::GetSysColor(COLOR_BTNTEXT));
		dc.DrawFocusRect(&rect);
		dc.SetBkColor(old);
	}

	dc.RestoreDC(-1);

	rect.DeflateRect(2, 2);
	ignoreRect.DeflateRect(1, 1);

	//Draw selection rect, or background color if no selection
	{
		COLORREF rgbBorder = GetSysColor (COLOR_3DDKSHADOW);
		if(!m_selected)
			rgbBorder = m_backcolor;

		CPen borderPen(PS_SOLID, 1, rgbBorder);
		CPen* pold = dc.SelectObject(&borderPen);
		dc.MoveTo(rect.TopLeft());
		dc.LineTo(rect.right-1, rect.top);
		dc.LineTo(rect.right-1, rect.bottom-1);
		dc.LineTo(rect.left, rect.bottom-1);
		dc.LineTo(rect.left, rect.top);
		dc.SelectObject(pold);
	}


	//Shrink the rect, 1 pixel on all sides.
	rect.DeflateRect(1,1);

	//We want to ignore the area inside the border...
	ignoreRect = rect;
	ignoreRect.DeflateRect(2, 2);

	//Draw border
	UINT uFrameCtrl = DFCS_BUTTONPUSH;
	//Is button pushed?
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) == ODS_SELECTED)
		uFrameCtrl |= DFCS_PUSHED;
	//Disabled?
	if ((lpDrawItemStruct->itemState & ODS_DISABLED) == ODS_DISABLED)
		uFrameCtrl |= DFCS_INACTIVE;
	
	//Draw the frame, but ignore the area inside
	dc.SaveDC();
	dc.ExcludeClipRect(&ignoreRect);
	dc.DrawFrameControl(&rect, DFC_BUTTON, uFrameCtrl);
	dc.RestoreDC(-1);
	
	//Draw color
	rect.DeflateRect(1,1);
	dc.FillSolidRect(&rect,	m_color);

	//Draw pattern if disabled
	if(!IsWindowEnabled())
	{	
		COLORREF but = m_backcolor;

		for(int x=rect.left; x<rect.right; x++)
			for(int y=rect.top; y<rect.bottom; y++)
				if( (x+y)%2 == 0)
				dc.SetPixel(x, y, but);
	}	


	//Draw text
	// Get caption text
	CString strCaption;
	GetWindowText (strCaption);
	
	//Any text to draw?
	if(!strCaption.IsEmpty())
	{
		int oldTextColor = dc.SetTextColor(COLOR_BTNTEXT);
		
		// Determine drawing format
		DWORD  dwFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_CENTER;
			
		// Determine dimensions of caption
		CRect rectCaption = rect;
	
		//Make push effect by shrinking the rect
		if ((lpDrawItemStruct->itemState & ODS_SELECTED) == ODS_SELECTED)
			rectCaption.DeflateRect(0, 0, -2, -2);
		
		//Draw text transparent...
		int oldMode = dc.SetBkMode(TRANSPARENT);
		//...with the original font
		CFont* oldFont = dc.SelectObject(GetFont());
		
//¡¯¡¯¡¯¡¯ By HONDA 2009.10.05 ¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯
//		int	iMulit,TxtBegin=0;
//		CString strTmp;
//		strTmp=strCaption.Trim();			
//		iMulit=strTmp.Find("\n");		
//		TxtBegin=int(rectCaption.CenterPoint().y-mFntHight)-2;
//¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯¡¯
		//OK, draw the text...
		if ((lpDrawItemStruct->itemState & ODS_DISABLED) == ODS_DISABLED)
		{
			//Draw like this if disabled.
			rectCaption.OffsetRect(1, 1);
			dc.SetTextColor(GetSysColor (COLOR_3DHILIGHT));
			dc.DrawText(strCaption, &rectCaption, dwFormat);
			
			rectCaption.OffsetRect(-1,-1);
			dc.SetTextColor(GetSysColor (COLOR_GRAYTEXT));
			dc.DrawText(strCaption, &rectCaption, dwFormat);
		}
		else
			dc.DrawText(strCaption,	&rectCaption,dwFormat );

		//_pDC->DrawText(
		//	sCaption,
		//	CRect(
		//		m_rBtnSize.left,
		//		(iMulit>=0)?
		//		m_rBtnSize.Height() + m_rBtnSize.top + TxtBegin:
		//		m_rBtnSize.Height() + m_rBtnSize.top, 
		//		m_rBtnSize.right, 
		//		m_rBtnSize.Height() + m_rBtnSize.bottom),
		//		DT_CENTER | DT_VCENTER | ((iMulit>=0)?BS_MULTILINE:DT_SINGLELINE));

		//Set some stuff back
		dc.SelectObject(oldFont);
		dc.SetBkMode(oldMode);
		dc.SetTextColor(oldTextColor);
	}
}

void ButtonAutoma::PreSubclassWindow() 
{
	//Make sure owner draw
	ModifyStyle(0, BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}

//When allow change is on, the color may be changed when the user 
//click on the button.
BOOL ButtonAutoma::OnClickedEx() 
{
	//if(m_allowChange)
	//{
	//	SelectColor();
	//}
	SetSelected(true);
	//Return FALSE. The parent will get the OnClick message.
	return FALSE;
}


//Redraws color box
void ButtonAutoma::Redraw()
{
	Invalidate(FALSE);
}
void ButtonAutoma::SetBackColor(const COLORREF newColor)
{
	//Set color
	m_backcolor = newColor;
	Redraw();
}
//Set/get color
void ButtonAutoma::SetColor(const COLORREF newColor)
{
	//Set color
	m_color=newColor;
	Redraw();
}

COLORREF ButtonAutoma::GetColor() const
{
	return m_color;
}

//Get/set selected
void ButtonAutoma::SetSelected(const BOOL selected)
{
	m_selected = selected;
	Redraw();
}

BOOL ButtonAutoma::GetSelected() const
{
	return m_selected;
}

//Set/get if user can change color by clicking
void ButtonAutoma::SetAllowChange(const BOOL allowchange)
{
	m_allowChange = allowchange;
}

BOOL ButtonAutoma::GetAllowChange() const
{
	return m_allowChange;
}

void ButtonAutoma::SetCustomColors(COLORREF *customcolors)
{
	m_CustomColors = customcolors;
}

//Show color box and let user select color
BOOL ButtonAutoma::SelectColor()
{
	BOOL ret = FALSE;

	CColorDialog *dlg = new CColorDialog;

	if(m_CustomColors != NULL)
		dlg->m_cc.lpCustColors = m_CustomColors;

	if(dlg)
	{
		//Set flags, init color
		dlg->m_cc.Flags = dlg->m_cc.Flags | CC_RGBINIT | CC_FULLOPEN;
		dlg->m_cc.rgbResult = m_color;

		if(dlg->DoModal() == IDOK)
		{
			TRACE(_T("User selected color: %d\n"), dlg->GetColor());
			
			if(dlg->GetColor() != m_color)
			{
				SetColor(dlg->GetColor());
				ret = TRUE;
			}
		}
	}

	delete dlg;
	
	return ret;
}