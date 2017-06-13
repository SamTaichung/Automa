// DlgConfigTimer.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgConfigTimer.h"

// DlgConfigTimer 對話方塊

IMPLEMENT_DYNAMIC(DlgConfigTimer, CDialog)

DlgConfigTimer::DlgConfigTimer(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_TIMER)
{
	m_pImageList = new CImageList();
}

DlgConfigTimer::~DlgConfigTimer()
{
	delete m_pImageList;
}

void DlgConfigTimer::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TIMERS, m_listTimers);
	DDX_Control(pDX, IDC_EDIT, m_editValue);
}

BEGIN_MESSAGE_MAP(DlgConfigTimer, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TIMERS, &DlgConfigTimer::OnLvnItemchangedListTimers)
END_MESSAGE_MAP()


// DlgConfigTimer 訊息處理常式


BOOL DlgConfigTimer::OnInitDialog()
{
	CDialog::OnInitDialog();
	RECT rect;
	m_listTimers.GetClientRect(&rect);
	int Width = rect.right - rect.left;
	int iconsize;
	iconsize = GetIconSize();
	m_pImageList->Create(iconsize, iconsize, ILC_COLOR8, 2, 2);
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GOFF));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GON));
	m_pImageList->SetBkColor(CLR_NONE);
	//	m_listValves.SubclassDlgItem(IDC_LIST_ValveS, this);
	m_listTimers.SetImageList(m_pImageList, LVSIL_SMALL);
	m_listTimers.InsertColumn(0, _T("編號"), LVCFMT_LEFT, Width*0.1);
	m_listTimers.InsertColumn(1, _T("中文名稱"), LVCFMT_LEFT, Width*0.3);
	m_listTimers.InsertColumn(2, _T("設定時間"), LVCFMT_RIGHT, Width*0.1);
	m_listTimers.SetColumnEditor(2, NULL, &DlgConfigTimer::EndEditor, &m_editValue);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
}
void DlgConfigTimer::RefreshPage()
{
	CString strV;
	for (int i = 0; i < m_listTimers.GetItemCount(); i++)
	{
		MTimer* pT;
		LVITEM item;
		pT = (MTimer*)(m_listTimers.GetItemData(i));
		item.iItem = i;
		item.mask = LVIF_IMAGE;
		int iTimerState = pT->isTimeOut() ? 0 : 1;
		if (m_listTimers.GetItem(&item))
		{
			if (iTimerState != item.iImage)
			{
				item.iImage = iTimerState;
				m_listTimers.SetItem(&item);
			}
		}
	}
}
void DlgConfigTimer::SetMachine(MachineBase *pM)
{
	TabPageAutoma::SetMachine(pM);

}

void DlgConfigTimer::MachineMessage(MMessage *pMsg)
{
	switch (pMsg->MsgType)
	{
	case MMessage::MESSAGETYPE::ChangeLanguage:
	case MMessage::MESSAGETYPE::MachineComplete:
		m_listTimers.DeleteAllItems();
		if (m_pMachine != NULL)
		{
			for (int i = 0; i < m_pMachine->GetTimerCount(); i++)
			{
				MTimer *pT = m_pMachine->GetTimer(i);

				m_listTimers.InsertItem(i, pT->m_ID, 0);
				m_listTimers.SetItemData(i, (DWORD_PTR)pT);
				//m_listValves.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, i, _T("Status"), 0, 0, 0, (LPARAM)pM);
				m_listTimers.SetItemText(i, 1, pT->m_strName);
				CString strV;
				strV.Format(_T("%10.3f"), pT->GetInterval());
				m_listTimers.SetItemText(i, 2, strV);
			}
		}
		break;
	}
}

BOOL DlgConfigTimer::EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	CString strV;
	MTimer* pT=(MTimer*)dwItemData;
	double dblV;
	ASSERT(pWnd);
	switch (nColumn)
	{
		case 2:
		{
			(*pWnd)->GetWindowText(strV);
			dblV = _wtof(strV);
			pT->SetInterval(dblV);
			strSubItemText.Format(_T("%10.3f"), pT->GetInterval());
			(*pWnd)->SetWindowText(strSubItemText);
			Machine *pM=(Machine *)(pT->GetTopParent());
			pM->OpenMachineMDB();
			pM->SaveMachineData(pT);
			pM->CloseMachineMDB();
		}
		break;
	}
	return TRUE;
}


void DlgConfigTimer::OnLvnItemchangedListTimers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}
