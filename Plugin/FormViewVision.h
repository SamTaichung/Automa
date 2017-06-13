#pragma once
#include "DlgVision.h"
#include "FormTabAutoma.h"
#include "TabCtrlAutoma.h"
#include "afxcmn.h"
using namespace AutoLib;
using namespace AutoUI;

// FormViewVision ����˵�

class FormViewVision : public FormTabAutoma
{
	DECLARE_DYNCREATE(FormViewVision)

protected:
	FormViewVision();           // �ʺA�إߩҨϥΪ��O�@�غc�禡
	virtual ~FormViewVision();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECIPEVIEW};
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	DlgVision m_pageVision;
	virtual void OnInitialUpdate();
};


