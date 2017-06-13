// DlgConfigIO.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgConfigIO.h"
#include "afxdialogex.h"


// DlgConfigIO 對話方塊

IMPLEMENT_DYNAMIC(DlgConfigIO, TabPageAutoma)

DlgConfigIO::DlgConfigIO(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_IO, pParent)
{
	m_pImageList = new CImageList();
}

DlgConfigIO::~DlgConfigIO()
{
	delete m_pImageList;
}

void DlgConfigIO::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IO_X, m_listIO_X);
	DDX_Control(pDX, IDC_LIST_IO_Y, m_listIO_Y);
}


BEGIN_MESSAGE_MAP(DlgConfigIO, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IO_X, &DlgConfigIO::OnLvnItemchangedListIoX)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IO_Y, &DlgConfigIO::OnLvnItemchangedListIoY)
END_MESSAGE_MAP()


// DlgConfigIO 訊息處理常式


BOOL DlgConfigIO::OnInitDialog()
{
	TabPageAutoma::OnInitDialog();

	RECT rect;
	m_listIO_X.GetClientRect(&rect);
	int Width = rect.right - rect.left;
	int iconsize;
	iconsize = GetIconSize();
	m_pImageList->Create(iconsize, iconsize, ILC_COLOR8, 2, 2);
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GOFF));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GON));
	m_pImageList->SetBkColor(CLR_NONE);
	//	m_listValves.SubclassDlgItem(IDC_LIST_ValveS, this);
	m_listIO_X.SetImageList(m_pImageList, LVSIL_SMALL);
	m_listIO_X.InsertColumn(0, _T("編號"), LVCFMT_LEFT, Width*0.2);
	m_listIO_X.InsertColumn(1, _T("名稱"), LVCFMT_LEFT, Width*0.7);
	m_listIO_Y.SetImageList(m_pImageList, LVSIL_SMALL);
	m_listIO_Y.InsertColumn(0, _T("編號"), LVCFMT_LEFT, Width*0.2);
	m_listIO_Y.InsertColumn(1, _T("名稱"), LVCFMT_LEFT, Width*0.7);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
}
void DlgConfigIO::RefreshPage()
{
	CString strV;
	for (int i = 0; i < m_listIO_X.GetItemCount(); i++)
	{
		MIO* pIO;
		LVITEM item;
		pIO = (MIO*)(m_listIO_X.GetItemData(i));
		item.iItem = i;
		item.mask = LVIF_IMAGE;
		int iTimerState = pIO->GetValue() ? 1 : 0;
		if (m_listIO_X.GetItem(&item))
		{
			if (iTimerState != item.iImage)
			{
				item.iImage = iTimerState;
				m_listIO_X.SetItem(&item);
			}
		}
	}
	for (int i = 0; i < m_listIO_Y.GetItemCount(); i++)
	{
		MIO* pIO;
		LVITEM item;
		pIO = (MIO*)(m_listIO_Y.GetItemData(i));
		item.iItem = i;
		item.mask = LVIF_IMAGE;
		int iTimerState = pIO->GetValue() ? 1 : 0;
		if (m_listIO_Y.GetItem(&item))
		{
			if (iTimerState != item.iImage)
			{
				item.iImage = iTimerState;
				m_listIO_Y.SetItem(&item);
			}
		}
	}
}
void DlgConfigIO::SetMachine(MachineBase *pM)
{
	TabPageAutoma::SetMachine(pM);

}

void DlgConfigIO::MachineMessage(MMessage *pMsg)
{
	switch (pMsg->MsgType)
	{
	case MMessage::MESSAGETYPE::MachineComplete:
		m_listIO_X.DeleteAllItems();
		if (m_pMachine != NULL)
		{
			int xcount=0,ycount = 0;
			for (int i = 0; i < m_pMachine->GetIOCount(); i++)
			{
				
				MIO *pIO = m_pMachine->GetIO(i);
				if (pIO->GetIsOut())
				{
					m_listIO_Y.InsertItem(ycount, pIO->m_ID, 0);
					m_listIO_Y.SetItemData(ycount, (DWORD_PTR)pIO);
					m_listIO_Y.SetItemText(ycount, 1, pIO->m_strName);
					ycount++;
				}
				else {
					m_listIO_X.InsertItem(xcount, pIO->m_ID, 0);
					m_listIO_X.SetItemData(xcount, (DWORD_PTR)pIO);
					m_listIO_X.SetItemText(xcount, 1, pIO->m_strName);
					xcount++;
				}
			}
		}
		break;
	}
}



void DlgConfigIO::OnLvnItemchangedListIoX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}


void DlgConfigIO::OnLvnItemchangedListIoY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}
