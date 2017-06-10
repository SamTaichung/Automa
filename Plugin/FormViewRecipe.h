#pragma once
#include "DlgPCB.h"
#include "DlgRecipe.h"
#include "DlgRecipePPArm.h"
#include "DlgComponents.h"
#include "FormTabAutoma.h"
#include "TabCtrlAutoma.h"
#include "afxcmn.h"
using namespace AutoLib;
using namespace AutoUI;

// FormViewRecipe ����˵�

class FormViewRecipe : public FormTabAutoma
{
	DECLARE_DYNCREATE(FormViewRecipe)

protected:
	FormViewRecipe();           // �ʺA�إߩҨϥΪ��O�@�غc�禡
	virtual ~FormViewRecipe();

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
	DlgPCB m_pagePCB;
	DlgRecipe m_pageRecipe;
	DlgRecipePPArm m_pagePPArm;
	DlgComponents m_pageComponents;
	virtual void OnInitialUpdate();
};


