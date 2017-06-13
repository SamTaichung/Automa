
// MainFrm.h : CMainFrame ���O������
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
	
protected: // �ȱq�ǦC�ƫإ�
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// �ݩ�
protected:
	LRESULT MachineMessage(WPARAM wParam, LPARAM lParam);
	// CSplitterWnd m_wndSplitter;
	// �ϥΦۤv�����h
	MultiViewSplitterAutoma m_wndSplitter;

public:

// �@�~
public:

// �мg
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �{���X��@
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	Machine *m_pMachine;
protected:  // ����C���O������
	//CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	// ���ͪ��T�������禡


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL DestroyWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


