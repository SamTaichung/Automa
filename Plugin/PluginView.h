
// PluginView.h : CPluginView 類別的介面
//

#pragma once


class CPluginView : public CFormView
{
protected: // 僅從序列化建立
	CPluginView();
	DECLARE_DYNCREATE(CPluginView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_PLUGIN_FORM };
#endif

// 屬性
public:
	CPluginDoc* GetDocument() const;

// 作業
public:

// 覆寫
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual void OnInitialUpdate(); // 建構後第一次呼叫

// 程式碼實作
public:
	virtual ~CPluginView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // PluginView.cpp 中的偵錯版本
inline CPluginDoc* CPluginView::GetDocument() const
   { return reinterpret_cast<CPluginDoc*>(m_pDocument); }
#endif

