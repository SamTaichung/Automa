// VValveView.cpp : 實作檔
//

#include "stdafx.h"
#include "VValveView.h"


// VValveView

IMPLEMENT_DYNCREATE(VValveView, VOPView)

VValveView::VValveView()
	: VOPView(VValveView::IDD)
{
	p_SelectedValve=NULL;
	isLoadValves=false;
	m_pImageList=new CImageList();

}

VValveView::~VValveView()
{
	delete m_pImageList;
}

void VValveView::DoDataExchange(CDataExchange* pDX)
{
	VOPView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VALVES, m_List);
	DDX_Control(pDX,IDC_CWB_OPEN,m_Open);
	DDX_Control(pDX,IDC_CWB_CLOSE,m_Close);
	DDX_Control(pDX,IDC_CWB_ACTION,m_Action);
	DDX_Control(pDX,IDC_CWB_IOOPEN,m_IOOpen);
	DDX_Control(pDX,IDC_CWB_IOOPENSR,m_IOOpenSR);
	DDX_Control(pDX,IDC_CWB_IOCLOSE,m_IOClose);
	DDX_Control(pDX,IDC_CWB_IOCLOSESR,m_IOCloseSR);
	DDX_Control(pDX,IDC_CWN_DELAY,m_Delay);
}

BEGIN_MESSAGE_MAP(VValveView, VOPView)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(HDN_ITEMCLICK, IDC_LIST_VALVES, OnHdnItemclickListValves)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_VALVES, OnLvnItemchangedListValves)
END_MESSAGE_MAP()


// VValveView 診斷

#ifdef _DEBUG
void VValveView::AssertValid() const
{
	VOPView::AssertValid();
}

void VValveView::Dump(CDumpContext& dc) const
{
	VOPView::Dump(dc);
}
#endif //_DEBUG

void VValveView::RefreshForm()
{
	std::map<CString,MValve *>::iterator itValve;
	if (isLoadValves)
	{
		for (itValve=MValve::m_Members.begin();itValve!=MValve::m_Members.end();itValve++)
		{
			LVFINDINFO find;
			LVITEM item;
			MValve *p_Valve=(MValve *)((*itValve).second);
			find.flags = LVFI_PARAM;
			find.lParam = (LPARAM)p_Valve;
			int itemnr = m_List.FindItem(&find);
			if (itemnr != -1)
			{
				item.mask=LVIF_IMAGE;
				m_List.GetItem(&item);
				if (item.iImage!=(p_Valve->isOpen()?0:1))
				{
					m_List.SetItem(itemnr,0,LVIF_IMAGE,NULL,(p_Valve->isOpen()?0:1),0,0,NULL,0);
				}
			}
		}
		if (p_SelectedValve!=NULL)
		{
			m_Open.SetValue(p_SelectedValve->isOpen());
			m_Action.SetValue(!p_SelectedValve->isIDLE());
			m_Close.SetValue(p_SelectedValve->isClose());
			m_Delay.SetValue(p_SelectedValve->TMRepeat.m_dblInterval);
			if (p_SelectedValve->m_pIOOpen==NULL)
			{
				m_IOOpen.SetValue(false);
			}else{
				m_IOOpen.SetValue(p_SelectedValve->m_pIOOpen->GetValue());
			}
			if (p_SelectedValve->m_pIOOpenSR==NULL)
			{
				m_IOOpenSR.SetValue(false);
			}else{
				m_IOOpenSR.SetValue(p_SelectedValve->m_pIOOpenSR->GetValue());
			}
			if (p_SelectedValve->m_pIOClose==NULL)
			{
				m_IOClose.SetValue(false);
			}else{
				m_IOClose.SetValue(p_SelectedValve->m_pIOClose->GetValue());
			}
			if (p_SelectedValve->m_pIOCloseSR==NULL)
			{
				m_IOCloseSR.SetValue(false);
			}else{
				m_IOCloseSR.SetValue(p_SelectedValve->m_pIOCloseSR->GetValue());
			}
		}
	}
}
// VValveView 訊息處理常式

void VValveView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	std::map<CString,MValve *>::iterator itValve;

	VOPView::OnShowWindow(bShow, nStatus);
	MMachine* pMachine=(MMachine*)m_pMachine;
	if (pMachine!=NULL)
	{
		if (!isLoadValves)
		{
			m_List.InsertColumn(0,"English Description",LVCFMT_LEFT,250);
			m_List.InsertColumn(1,"中文說明",LVCFMT_LEFT,250);
			for (itValve=MValve::m_Members.begin();itValve!=MValve::m_Members.end();itValve++)
			{
				int index=m_List.GetItemCount();
				MValve * p_Valve=(*itValve).second;	
				m_List.InsertItem(LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM,index,p_Valve->m_EName,0,0,(p_Valve->isOpen()?0:1),(LPARAM)p_Valve);
				m_List.SetItemText(index,1,p_Valve->m_CName);
			}
			isLoadValves=true;
		}
		if (bShow)
		{

		}
	}
	// TODO: 在此加入您的訊息處理常式程式碼
}

void VValveView::OnInitialUpdate()
{
	VOPView::OnInitialUpdate();

	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	m_pImageList->Create(32,32,ILC_COLOR8,2,2);
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_ON));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_OFF));
	m_pImageList->SetBkColor(CLR_NONE);
	m_List.SetImageList(m_pImageList, LVSIL_SMALL);

}

void VValveView::OnHdnItemclickListValves(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	p_SelectedValve=(MValve *)(m_List.GetItemData(phdr->iItem));
	*pResult = 0;
}
BEGIN_EVENTSINK_MAP(VValveView, VOPView)
	ON_EVENT(VValveView, IDC_GO_OPEN, DISPID_CLICK, ClickGoOpen, VTS_NONE)
	ON_EVENT(VValveView, IDC_GO_CLOSE, DISPID_CLICK, ClickGoClose, VTS_NONE)
	ON_EVENT(VValveView, IDC_STOP, DISPID_CLICK, ClickStop, VTS_NONE)
	ON_EVENT(VValveView, IDC_REPEAT, DISPID_CLICK, ClickRepeat, VTS_NONE)
	ON_EVENT(VValveView, IDC_SAVE, DISPID_CLICK, ClickSave, VTS_NONE)
	ON_EVENT(VValveView, IDC_CWN_DELAY, 1, ValueChangedCwnDelay, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(VValveView, IDC_OFF_VALVE, DISPID_CLICK, ClickOffValve, VTS_NONE)
END_EVENTSINK_MAP()

void VValveView::ClickGoOpen()
{
	// TODO: 在此加入您的訊息處理常式程式碼
	if (p_SelectedValve!=NULL)
	{
		p_SelectedValve->OpenValve();
	}
}

void VValveView::ClickGoClose()
{
	// TODO: 在此加入您的訊息處理常式程式碼
	if (p_SelectedValve!=NULL)
	{
		p_SelectedValve->CloseValve();
	}
}

void VValveView::ClickStop()
{
	// TODO: 在此加入您的訊息處理常式程式碼
	if (p_SelectedValve!=NULL)
	{
		p_SelectedValve->Stop();
	}
}


//void VValveView::OnMenuExecute()
//{
//	// TODO: 在此加入您的命令處理常式程式碼
//	int iSel = m_List.GetNextItem(-1, LVIS_SELECTED | LVIS_FOCUSED);
//	if (iSel != -1){
//		p_SelectedValve=(MValve*)(m_List.GetItemData(iSel));
//	}
//}


void VValveView::ClickRepeat()
{
	// TODO: 在此加入您的訊息處理常式程式碼
	if (p_SelectedValve!=NULL)
	{
		p_SelectedValve->Repeat();
	}
}

void VValveView::ClickSave()
{
	// TODO: 在此加入您的訊息處理常式程式碼
	if (p_SelectedValve!=NULL)
	{
		p_SelectedValve->SaveMachineData(&(((MMachine*)m_pMachine)->m_cnnMD));
	}
}

void VValveView::ValueChangedCwnDelay(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	// TODO: 在此加入您的訊息處理常式程式碼
	if (p_SelectedValve!=NULL)
	{
		p_SelectedValve->TMRepeat.m_dblInterval=Value->dblVal;
	}
}

void VValveView::OnLvnItemchangedListValves(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
	if (pNMLV->uNewState & LVIS_FOCUSED) 
	{
		p_SelectedValve=(MValve *)(pNMLV->lParam);
	}
}

void VValveView::ClickOffValve()
{
	// TODO: 在此加入您的訊息處理常式程式碼
	if (p_SelectedValve!=NULL)
	{
		p_SelectedValve->OffValve();
	}

}
