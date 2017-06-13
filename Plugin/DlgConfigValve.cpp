// DlgConfigValve.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgConfigValve.h"


// DlgConfigValve 對話方塊

IMPLEMENT_DYNAMIC(DlgConfigValve, CDialog)

DlgConfigValve::DlgConfigValve(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_VALVE)
{
	m_pImageList = new CImageList();
	m_pSelectedValve = NULL;
}

DlgConfigValve::~DlgConfigValve()
{
	delete m_pImageList;
}

void DlgConfigValve::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_LIST_Valve, m_lstValves);
	DDX_Control(pDX, IDC_STC_POS, m_stcPos);
	DDX_Control(pDX, IDC_LIST_VALVES, m_listValves);
}


BEGIN_MESSAGE_MAP(DlgConfigValve, CDialog)
	ON_BN_CLICKED(IDC_BTN_STOP, &DlgConfigValve::OnBnClickedBtnStop)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_VALVES, &DlgConfigValve::OnLvnItemchangedListValves)
	ON_BN_CLICKED(IDC_BTN_OPEN, &DlgConfigValve::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &DlgConfigValve::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_REPEAT, &DlgConfigValve::OnBnClickedBtnRepeat)
END_MESSAGE_MAP()

void DlgConfigValve::RefreshPage()
{
	CString strV;
	if (m_pSelectedValve != NULL)
	{
		strV=m_pSelectedValve->GetStateName();
		m_stcPos.SetWindowTextW(strV);
	}
	for (int i = 0;i < m_listValves.GetItemCount();i++)
	{
		MValve* pV;
		pV = (MValve*)(m_listValves.GetItemData(i));
		CString strText;
		strText=m_listValves.GetItemText(i, 0);
		if (pV->GetStateName().Compare(strText)!=0)
		{
			m_listValves.SetItemText(i, 0, pV->GetStateName());
		}
	}
}

void DlgConfigValve::OnBnClickedBtnStop()
{
	if (m_pSelectedValve != NULL)
	{
		m_pSelectedValve->Stop();
	}
}

void DlgConfigValve::OnLvnItemchangedListValves(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState & LVIS_FOCUSED)
	{
		CString strV;
		OnSelectValve((MValve *)(m_listValves.GetItemData(pNMLV->iItem)));
	}
	*pResult = 0;
}
void DlgConfigValve::OnSelectValve(MValve *pM)
{
	m_pSelectedValve = pM;
//	m_editP1.SetValueExchange(m_pSelectedValve->m_P1);

}
BOOL DlgConfigValve::OnInitDialog()
{
	CDialog::OnInitDialog();
	RECT rect;
	m_listValves.GetClientRect(&rect);
	int Width = rect.right - rect.left;
	int iconsize;
	iconsize = GetIconSize();
	m_pImageList->Create(iconsize, iconsize, ILC_COLOR8, 2, 2);

	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GON));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GOFF));
	m_pImageList->SetBkColor(CLR_NONE);
//	m_listValves.SubclassDlgItem(IDC_LIST_ValveS, this);
	m_listValves.SetImageList(m_pImageList, LVSIL_SMALL);
	m_listValves.InsertColumn(0, _T("狀態"), LVCFMT_LEFT, Width*0.22);
	m_listValves.InsertColumn(1, _T("編號"), LVCFMT_LEFT, Width*0.2);
	m_listValves.InsertColumn(2, _T("名稱"), LVCFMT_LEFT, Width*0.5);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
}
void DlgConfigValve::SetMachine(MachineBase *pM)
{
	TabPageAutoma::SetMachine(pM);

}

void DlgConfigValve::MachineMessage(MMessage *pMsg)
{
	switch (pMsg->MsgType)
	{
	case MMessage::MESSAGETYPE::ChangeLanguage:
	case MMessage::MESSAGETYPE::MachineComplete:
		m_listValves.DeleteAllItems();
		if (m_pMachine != NULL)
		{
			for (int i = 0; i < m_pMachine->GetValveCount(); i++)
			{
				MValve *pM = m_pMachine->GetValve(i);

				m_listValves.InsertItem(i, _T("Status"), 0);
				m_listValves.SetItemData(i, (DWORD_PTR)pM);
				//m_listValves.InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, i, _T("Status"), 0, 0, 0, (LPARAM)pM);
				m_listValves.SetItemText(i, 1, pM->m_ID);
				m_listValves.SetItemText(i, 2, pM->m_strName);
			}

		}
		if (m_listValves.GetItemCount() > 0)
		{
			m_listValves.SelectItem(0, true);
			OnSelectValve((MValve*)m_listValves.GetItemData(0));
		}
		break;
	}
}
void DlgConfigValve::OnBnClickedBtnTest()
{
	if (m_pSelectedValve != NULL)
	{
		m_pSelectedValve->Repeat();
	}
}

void DlgConfigValve::OnBnClickedBtnSave()
{
	if (m_pSelectedValve != NULL)
	{
		m_pMachine->OpenMachineMDB();
		m_pMachine->SaveMachineData(m_pSelectedValve, false);
		m_pMachine->CloseMachineMDB();
	}
}




void DlgConfigValve::OnBnClickedBtnOpen()
{
	if (m_pSelectedValve != NULL)
	{
		m_pSelectedValve->OpenValve(MValve::ValveMode::vmForce);
	}
}
void DlgConfigValve::OnBnClickedBtnClose()
{
	if (m_pSelectedValve != NULL)
	{
		m_pSelectedValve->CloseValve(MValve::ValveMode::vmForce);
	}
}


void DlgConfigValve::OnBnClickedBtnRepeat()
{
	if (m_pSelectedValve != NULL)
	{
		m_pSelectedValve->Repeat();
	}
}
