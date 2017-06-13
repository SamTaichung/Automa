// FormViewBottom.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "FormViewBottom.h"
#include "MainFrm.h"

// FormViewBottom

IMPLEMENT_DYNCREATE(FormViewBottom, FormViewAutoma)

FormViewBottom::FormViewBottom()
	: FormViewAutoma(IDD_BOTTOMVIEW)
{

}

FormViewBottom::~FormViewBottom()
{
}

void FormViewBottom::DoDataExchange(CDataExchange* pDX)
{
	FormViewAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_AUTO, m_btnFunction[0]);
	DDX_Control(pDX, IDC_BT_MANUAL, m_btnFunction[1]);
	DDX_Control(pDX, IDC_BT_PRODUCTION, m_btnFunction[2]);
	DDX_Control(pDX, IDC_BT_RECIPE, m_btnFunction[3]);
	DDX_Control(pDX, IDC_BT_VISION, m_btnFunction[4]);
	DDX_Control(pDX, IDC_BT_CONFIG, m_btnFunction[5]);
	DDX_Control(pDX, IDC_BT_STOP, m_btnStop);

	COLORREF colorbtn;
	colorbtn = RGB(0, 64, 192);
	for (int i = 0;i < 6;i++)
	{
		m_btnFunction[i].SetColor(colorbtn);
		m_btnFunction[i].SetBackColor(GetBackColor());
	}
	m_btnStop.SetColor(colorbtn);
	m_btnStop.SetBackColor(GetBackColor());

}

BEGIN_MESSAGE_MAP(FormViewBottom, CFormView)
	ON_BN_CLICKED(IDC_BT_AUTO, &FormViewBottom::OnBnClickedBtAuto)
	ON_BN_CLICKED(IDC_BT_MANUAL, &FormViewBottom::OnBnClickedBtManual)
	ON_BN_CLICKED(IDC_BT_PRODUCTION, &FormViewBottom::OnBnClickedBtProduction)
	ON_BN_CLICKED(IDC_BT_RECIPE, &FormViewBottom::OnBnClickedBtRecipe)
	ON_BN_CLICKED(IDC_BT_CONFIG, &FormViewBottom::OnBnClickedBtConfig)
	ON_BN_CLICKED(IDC_BT_STOP, &FormViewBottom::OnBnClickedBtStop)
	ON_BN_CLICKED(IDC_BT_VISION, &FormViewBottom::OnBnClickedBtVision)
END_MESSAGE_MAP()


// FormViewBottom 診斷

#ifdef _DEBUG
void FormViewBottom::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FormViewBottom::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// FormViewBottom 訊息處理常式


void FormViewBottom::OnBnClickedBtAuto()
{
	this->m_pSplitter->ShowView(CMainFrame::VIEWID::AutoView);
	OnFunctionSelect(0);
}


void FormViewBottom::OnBnClickedBtManual()
{
	this->m_pSplitter->ShowView(CMainFrame::VIEWID::ManualView);
	OnFunctionSelect(1);

}


void FormViewBottom::OnBnClickedBtProduction()
{
	this->m_pSplitter->ShowView(CMainFrame::VIEWID::ProductionView);
	OnFunctionSelect(2);

}


void FormViewBottom::OnBnClickedBtRecipe()
{
	this->m_pSplitter->ShowView(CMainFrame::VIEWID::RecipeView);
	OnFunctionSelect(3);

}
void FormViewBottom::OnBnClickedBtConfig()
{
	this->m_pSplitter->ShowView(CMainFrame::VIEWID::ConfigView);
	OnFunctionSelect(5);

}
void FormViewBottom::OnFunctionSelect(int index)
{
	COLORREF colorbtn;
	for (int i = 0;i < 6;i++)
	{
		if (index == i)
		{
			colorbtn = RGB(0, 128, 255);
		}
		else {
			colorbtn = RGB(0, 64, 192);
		}
		m_btnFunction[i].SetColor(colorbtn);
		m_btnFunction[i].SetBackColor(GetBackColor());
	}
}

void FormViewBottom::OnBnClickedBtStop()
{
	if (m_pMachine != NULL)
	{
		m_pMachine->MachineStop(); //給UI用的停機呼叫
	}
}


void FormViewBottom::OnBnClickedBtVision()
{
	this->m_pSplitter->ShowView(CMainFrame::VIEWID::VisionView);
	OnFunctionSelect(4);
}
