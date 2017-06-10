// FormViewRecipe.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "FormViewRecipe.h"


// FormViewRecipe

IMPLEMENT_DYNCREATE(FormViewRecipe, FormTabAutoma)

FormViewRecipe::FormViewRecipe()
	: FormTabAutoma(IDD_RECIPEVIEW, IDC_TAB_RECIPE)
{
}

FormViewRecipe::~FormViewRecipe()
{

}
BEGIN_MESSAGE_MAP(FormViewRecipe, CFormView)
END_MESSAGE_MAP()


// FormViewRecipe 診斷

#ifdef _DEBUG
void FormViewRecipe::AssertValid() const
{
	FormTabAutoma::AssertValid();
}

#ifndef _WIN32_WCE
void FormViewRecipe::Dump(CDumpContext& dc) const
{
	FormTabAutoma::Dump(dc);
}
#endif
#endif //_DEBUG


// FormViewRecipe 訊息處理常式
void FormViewRecipe::OnInitialUpdate()
{
	FormTabAutoma::OnInitialUpdate();
	m_pagePCB.Create(IDD_DLG_PCB, this);
	m_pagePCB.SetWindowTextW(_T("PCB"));
	AddPage(&m_pagePCB);
	m_pageRecipe.Create(IDD_DLG_RECIPE, this);
	m_pageRecipe.SetWindowTextW(_T("Recipe"));
	AddPage(&m_pageRecipe);
	m_pagePPArm.Create(IDD_DLG_RECIPE_PPARM, this);
	m_pagePPArm.SetWindowTextW(_T("P.P.Arms"));
	AddPage(&m_pagePPArm);
	m_pageComponents.Create(IDD_DLG_COMPONENT, this);
	m_pageComponents.SetWindowTextW(_T("Components"));
	AddPage(&m_pageComponents);
}
