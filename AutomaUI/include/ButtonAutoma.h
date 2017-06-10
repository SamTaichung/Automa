#pragma once

#ifdef AUTOMAUI_EXPORTS
#define AUTOMAUI_API __declspec(dllexport)
#else
#define AUTOMAUI_API __declspec(dllimport)
#endif
namespace AutoUI {
	AUTOMAUI_API class ButtonAutoma : public CButton
	{
#ifdef AUTOMAUI_EXPORTS
		DECLARE_DYNAMIC(ButtonAutoma)
#endif
		// Construction
	public:
		AUTOMAUI_API ButtonAutoma();
		AUTOMAUI_API virtual ~ButtonAutoma();

		AUTOMAUI_API void SetBackColor(const COLORREF newColor);
		//Set/Get the current color
		AUTOMAUI_API void SetColor(const COLORREF newColor);
		AUTOMAUI_API COLORREF GetColor() const;

		//Set/get allow change mode. When on, the user can change color
		//when he clicks on the box..
		void SetAllowChange(const BOOL allowchange);
		BOOL GetAllowChange() const;

		//Get/set selected mode.
		//When selected, a small frame is drawn around the box
		void SetSelected(const BOOL selected);
		BOOL GetSelected() const;

		//Start the color selector and let user change color.
		//Return true if color changed.
		BOOL SelectColor();

		//Set a pointer to the custom colors.
		//IMPORTANT: It must be a pointer to an array with at least 16 elements.
		//When user change color in the color selector, the colors here might be
		//changed as well.
		void SetCustomColors(COLORREF *customcolors);

		//Redraw the box
		void Redraw();

		// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(ButtonAutoma)
	public:
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
		virtual void PreSubclassWindow();
		//}}AFX_VIRTUAL

	// Implementation

		// Generated message map functions
	protected:
		LONG	mFntHight;
		COLORREF m_color,m_backcolor;
		BOOL m_allowChange;
		BOOL m_selected;

		COLORREF*  m_CustomColors;

		//{{AFX_MSG(ButtonAutoma)
		afx_msg BOOL OnClickedEx();
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()
	};
}