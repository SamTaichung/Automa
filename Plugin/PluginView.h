
// PluginView.h : CPluginView ���O������
//

#pragma once


class CPluginView : public CFormView
{
protected: // �ȱq�ǦC�ƫإ�
	CPluginView();
	DECLARE_DYNCREATE(CPluginView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_PLUGIN_FORM };
#endif

// �ݩ�
public:
	CPluginDoc* GetDocument() const;

// �@�~
public:

// �мg
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	virtual void OnInitialUpdate(); // �غc��Ĥ@���I�s

// �{���X��@
public:
	virtual ~CPluginView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ͪ��T�������禡
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // PluginView.cpp ������������
inline CPluginDoc* CPluginView::GetDocument() const
   { return reinterpret_cast<CPluginDoc*>(m_pDocument); }
#endif

