#pragma once
#include "MachineBase.h"
#ifdef AUTOMAUI_EXPORTS
#define AUTOMAUI_API __declspec(dllexport)
#else
#define AUTOMAUI_API __declspec(dllimport)
#endif

#include <map>
#include <stack>
using namespace std;
using namespace AutoLib;
namespace AutoUI {
	AUTOMAUI_API class MultiViewSplitterAutoma : public CSplitterWnd
	{
#ifdef AUTOMAUI_EXPORTS
		DECLARE_DYNAMIC(MultiViewSplitterAutoma)
#endif
	public:
		AUTOMAUI_API MultiViewSplitterAutoma();
		AUTOMAUI_API virtual ~MultiViewSplitterAutoma();
		AUTOMAUI_API int AddView(int nRow, int nCol, int NewViewID, CRuntimeClass * pViewClass,
			CCreateContext* pContext, int sx = 10, int sy = 10);

		AUTOMAUI_API CWnd* GetView(int nViewID);
		AUTOMAUI_API void RefreshForm();
		bool	m_bResizable;	// enable/disable resize.

	protected:
		map<int, long> m_mapViewPane;
		map<long, int> m_mapCurrentViews;
		map<int, CWnd*> m_mapIDViews;
		int m_nIDCounter;
		int m_FromView;
		AUTOMAUI_API CWnd* GetCurrentView(int nRow, int nCol, int * nCurID);
		AUTOMAUI_API void SetCurrentView(int nRow, int nCol, int nViewID);
		AUTOMAUI_API int HideCurrentView(int nRow, int nCol);
		AUTOMAUI_API void GetPaneFromViewID(int nViewID, CPoint * pane);
		//覆寫下列函數,若m_bResizable==false,則窗格無法改變尺寸.
		afx_msg void OnLButtonDown(UINT, CPoint);

	public:
		DECLARE_MESSAGE_MAP()


	public:
		AUTOMAUI_API void SetMachine(MachineBase *pM);
		AUTOMAUI_API void MachineMessage(MMessage *pMsg);
		AUTOMAUI_API void ShowView(int nViewID);
		virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	};
}
