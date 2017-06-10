#pragma once
#ifdef AUTOMAUI_EXPORTS
#define AUTOMAUI_API __declspec(dllexport)
#else
#define AUTOMAUI_API __declspec(dllimport)
#endif
#define UM_HIDEEDITOR	(WM_USER+10)
namespace AutoUI {
	class CMsgHook
	{
	public:
		CMsgHook(void);
		virtual ~CMsgHook(void);


		BOOL Attach(HWND hWnd, HWND hParent, HWND hNotify = NULL);
		BOOL Detach(void);
		inline BOOL IsAttached() { return m_hWnd != NULL; }

	protected:
		static LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam);

		static HHOOK m_hHook;
		static int m_nInstances;
		static CMap<HWND, HWND&, CMsgHook*, CMsgHook*&> m_mapHookedWindows;  // map of windows to hooks

		HWND m_hWnd;
		HWND m_hParent;
		HWND m_hNotify;
		BOOL m_bDropDown;
	};
}