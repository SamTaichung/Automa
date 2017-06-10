
// MainFrm.h : CMainFrame 類別的介面
//

#pragma once
#include "Plugin.h"
#include "Machine.h"
#include "MultiViewSplitterAutoma.h"
#include "FormViewConfig.h"
#include "FormViewVision.h"
#include "FormViewRecipe.h"
#include "FormViewProduction.h"
#include "FormViewManual.h"
#include "FormViewAuto.h"
#include "FormViewTop.h"
#include "FormViewBottom.h"
#include "DlgError.h"
#include "DlgEmgStop.h"

using namespace AutoLib;
using namespace AutoUI;
class CMainFrame : public CFrameWnd
{
public:
	enum VIEWID {
		AutoView = 0,
		ManualView,
		ProductionView,
		RecipeView,
		VisionView,
		ConfigView,
		TopView,
		BottomView,
	};
	
protected: // 僅從序列化建立
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 屬性
protected:
	LRESULT MachineMessage(WPARAM wParam, LPARAM lParam);
	// CSplitterWnd m_wndSplitter;
	// 使用自己的底層
	MultiViewSplitterAutoma m_wndSplitter;

public:

// 作業
public:

// 覆寫
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 程式碼實作
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	Machine *m_pMachine;
protected:  // 控制列內嵌的成員
	//CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	// 產生的訊息對應函式


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL DestroyWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


