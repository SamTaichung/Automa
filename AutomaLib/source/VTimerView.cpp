// VTimerView.cpp : 實作檔
#include "stdafx.h"
#include "VTimerView.h"
#include ".\vtimerview.h"

// VTimerView

IMPLEMENT_DYNCREATE(VTimerView, VOPView)

VTimerView::VTimerView()
	: VOPView(VTimerView::IDD)
{
	p_cnnTmp=NULL;
}

VTimerView::~VTimerView()
{
}

void VTimerView::DoDataExchange(CDataExchange* pDX)
{
	VOPView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATAGRID_TIMERS, m_Grid);
}

BEGIN_MESSAGE_MAP(VTimerView, VOPView)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// VTimerView 診斷

#ifdef _DEBUG
void VTimerView::AssertValid() const
{
	VOPView::AssertValid();
}

void VTimerView::Dump(CDumpContext& dc) const
{
	VOPView::Dump(dc);
}
#endif //_DEBUG


// VTimerView 訊息處理常式

void VTimerView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	VOPView::OnShowWindow(bShow, nStatus);

	// TODO: 在此加入您的訊息處理常式程式碼
	CString strSQL,strV;
	double dblV;
	CADORecordset rsTmp;

	MMachine* pMachine=(MMachine*)m_pMachine;
	if (pMachine!=NULL)
	{
		if (bShow)
		{
			p_cnnTmp=&(pMachine->m_cnnTmp);
			if (p_cnnTmp->GetTableIndex("Timers")>=0)
			{
				p_cnnTmp->BeginTrans();
				p_cnnTmp->ExecuteSQL("Drop Table Timers");
				p_cnnTmp->CommitTrans();
			}
			strSQL="Create Table Timers(ID nChar(10),CName nChar(40),EName nChar(40),";
			strSQL+="[Interval] float not null default 0,";
			strSQL+="Constraint PKTimers Primary Key(ID))";
			p_cnnTmp->ExecuteSQL(strSQL);
			if (rsTmp.Open("Select * From Timers",&pMachine->m_cnnMD))
			{
				while (!rsTmp.isEOF()){
					strSQL="Insert into Timers(ID,CName,EName,[Interval]) Values('";
					rsTmp.GetValue("ID",strV);
					strSQL+=strV+"','";
					rsTmp.GetValue("CName",strV);
					strSQL+=strV+"','";
					rsTmp.GetValue("EName",strV);
					strSQL+=strV+"',";
					rsTmp.GetValue("Interval",dblV);
					strV.Format("%10.3f",dblV);
					strSQL+=strV+")";
					p_cnnTmp->ExecuteSQL(strSQL);
					rsTmp.MoveNext();
				}
			}
			rsTimer.Open("Select * From Timers",p_cnnTmp);
			m_Grid.SetRefDataSource(rsTimer.m_pRecordset);
			CColumn colTmp;
			colTmp=m_Grid.GetColumns().GetItem(_variant_t(short(0),VT_I2));	//ID
			colTmp.SetWidth(80);
			colTmp.SetLocked(true);
			colTmp=m_Grid.GetColumns().GetItem(_variant_t(short(1),VT_I2)); //EName
			colTmp.SetWidth(280);
			colTmp.SetLocked(true);
			colTmp=m_Grid.GetColumns().GetItem(_variant_t(short(2),VT_I2)); //CName
			colTmp.SetWidth(280);
			colTmp.SetLocked(true);
			colTmp=m_Grid.GetColumns().GetItem(_variant_t(short(3),VT_I2)); //CName
			colTmp.SetWidth(150);
			colTmp.SetAlignment(1);
			m_Grid.SetCol(3);
		}else{
			if(rsTimer.GetEditMode()!=ADODB::adEditNone)
			{
				rsTimer.CancelUpdate();
			}
			rsTimer.Close();
		}
	}
}
BEGIN_EVENTSINK_MAP(VTimerView, VOPView)
	ON_EVENT(VTimerView, IDC_DATAGRID_TIMERS, 200, AfterColEditDatagridTimers, VTS_I2)
END_EVENTSINK_MAP()

void VTimerView::AfterColEditDatagridTimers(short ColIndex)
{
	double dblTime;
	CString strID,strSQL;
	MTimer *pT;
	if (rsTimer.GetState())
	{
		rsTimer.GetValue("ID",strID);
		strID.Trim();
		pT=MTimer::GetTimer(strID);
		if (pT!=NULL)
		{
			rsTimer.GetValue("Interval",dblTime);
			strSQL.Format("Update Timers Set [Interval]=%f Where ID='%s'",dblTime,strID);
			((MMachine*)m_pMachine)->m_cnnMD.ExecuteSQL(strSQL);
			pT->m_dblInterval=dblTime;
		}
	}
}
