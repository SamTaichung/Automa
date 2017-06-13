// IOView.cpp : 實作檔
//

#include "stdafx.h"
#include "IOView.h"
#include "MIO.h"

// VIOView

IMPLEMENT_DYNCREATE(VIOView, VOPView)

VIOView::VIOView()
	: VOPView(VIOView::IDD)
{
	isLoadIOs=false;
	m_pImageList=new CImageList();

}

VIOView::~VIOView()
{
	delete m_pImageList;
}

void VIOView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}
void VIOView::RefreshForm()
{
	std::map<CString,MIO *>::iterator itIO;
	if (isLoadIOs)
	{
		for (itIO=MIO::m_Members.begin();itIO!=MIO::m_Members.end();itIO++)
		{
			LVFINDINFO find;
			LVITEM item;
			MIO *p_IO=(MIO *)((*itIO).second);
			find.flags = LVFI_PARAM;
			find.lParam = (LPARAM)p_IO;
			int itemnr = m_List.FindItem(&find);
			if (itemnr != -1)
			{
				item.mask=LVIF_IMAGE;
				item.iItem=itemnr;
				m_List.GetItem(&item);
				if (item.iImage!=(p_IO->GetValue()?0:1))
				{
					m_List.SetItem(itemnr,0,LVIF_IMAGE,NULL,(p_IO->GetValue()?0:1),0,0,NULL,0);
				}
			}
		}
	}
}
BEGIN_MESSAGE_MAP(VIOView, VOPView)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// VIOView 診斷

#ifdef _DEBUG
void VIOView::AssertValid() const
{
	CFormView::AssertValid();
}

void VIOView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// VIOView 訊息處理常式
BEGIN_EVENTSINK_MAP(VIOView, VOPView)
END_EVENTSINK_MAP()


void VIOView::OnInitialUpdate()
{
	VOPView::OnInitialUpdate();
	m_pImageList->Create(32,32,ILC_COLOR8,2,2);
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_ON));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_OFF));
	m_pImageList->SetBkColor(CLR_NONE);
	m_List.SubclassDlgItem(IDC_IOVIEW_LIST,this);
	m_List.SetImageList(m_pImageList, LVSIL_SMALL);
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
}

void VIOView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	VOPView::OnShowWindow(bShow, nStatus);
	std::map<CString,MIO *>::iterator itIO;
	// TODO: 在此加入您的訊息處理常式程式碼
	MMachine* pMachine=(MMachine*)m_pMachine;
	if (pMachine!=NULL)
	{
		if (!isLoadIOs)
		{
			m_List.InsertColumn(0,"線碼",LVCFMT_LEFT,100);
			m_List.InsertColumn(1,"Description",LVCFMT_LEFT,300);
			m_List.InsertColumn(2,"中文說明",LVCFMT_LEFT,300);
			for (itIO=MIO::m_Members.begin();itIO!=MIO::m_Members.end();itIO++)
			{
				int index=m_List.GetItemCount();
				MIO * p_IO=(*itIO).second;															
				m_List.InsertItem(LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM,index,p_IO->m_LineID,0,0,(p_IO->GetValue()?0:1),(LPARAM)p_IO);
				m_List.SetItemText(index,1,p_IO->m_EName);
				m_List.SetItemText(index,2,p_IO->m_CName);
			}
			isLoadIOs=true;
		}
		if (bShow)
		{

		}
	}
}
void VIOView::ExecuteMenu()
{
	// TODO: 在此加入您的命令處理常式程式碼
	MMachine *pM=(MMachine*)m_pMachine;
	int iSel = m_List.GetNextItem(-1, LVIS_SELECTED | LVIS_FOCUSED);
	if (iSel != -1 && !pM->m_pIOInterLock->GetValue()){
		MIO * p_IO=(MIO*)(m_List.GetItemData(iSel));
		if (p_IO->m_isOut)
		{
			p_IO->Inverse();			
		}
	}
}
