#pragma once
#ifdef AUTOMAUI_EXPORTS
#define AUTOMAUI_API __declspec(dllexport)
#else
#define AUTOMAUI_API __declspec(dllimport)
#endif
namespace AutoUI {
	AUTOMAUI_API class EditAutoma : public CEdit
	{
		DECLARE_DYNAMIC(EditAutoma)

	public:
		AUTOMAUI_API EditAutoma();
		AUTOMAUI_API virtual ~EditAutoma();
		AUTOMAUI_API void SetValueExchange(double &pV);
	protected:
		DECLARE_MESSAGE_MAP()
		CString m_strFormat;
		double *m_pdblValue;
	public:
		afx_msg void OnEnKillfocus();
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	};

}
