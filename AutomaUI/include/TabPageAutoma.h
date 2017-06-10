#pragma once
#include "MachineBase.h"
#ifdef AUTOMAUI_EXPORTS
#define AUTOMAUI_API __declspec(dllexport)
#else
#define AUTOMAUI_API __declspec(dllimport)
#endif
using namespace AutoLib;
namespace AutoUI {
	// TabPageAutoma 對話方塊

	AUTOMAUI_API class TabPageAutoma : public CDialog
	{
#ifdef AUTOMAUI_EXPORTS
		DECLARE_DYNAMIC(TabPageAutoma)
#endif
	public:
		AUTOMAUI_API TabPageAutoma(UINT id, CWnd* pParent = NULL);   // 標準建構函式
		AUTOMAUI_API virtual ~TabPageAutoma();
		AUTOMAUI_API virtual void SetMachine(MachineBase* pM);
		AUTOMAUI_API virtual bool isActive();
		AUTOMAUI_API virtual void SetActive(bool bActive);
		AUTOMAUI_API virtual void RefreshPage() {};
		AUTOMAUI_API virtual void MachineMessage(MMessage *pMsg) {};
		AUTOMAUI_API int GetIconSize();
	protected:
		//AUTOMAUI_API virtual void OnCancel();
		//AUTOMAUI_API virtual void OnOK();
		AUTOMAUI_API virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
		MachineBase *m_pMachine;
		bool m_bActive;
		UINT m_nTempID;
		DECLARE_MESSAGE_MAP()
	public:
		AUTOMAUI_API virtual BOOL PreTranslateMessage(MSG* pMsg);
	};
}