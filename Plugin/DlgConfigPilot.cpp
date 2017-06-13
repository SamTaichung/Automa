// DlgConfigPilot.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgConfigPilot.h"

// DlgConfigPilot 對話方塊

IMPLEMENT_DYNAMIC(DlgConfigPilot, CDialog)

DlgConfigPilot::DlgConfigPilot(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_TIMER)
{
	m_pImageList = new CImageList();
}

DlgConfigPilot::~DlgConfigPilot()
{
	delete m_pImageList;
}

void DlgConfigPilot::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TIMERS, m_listState);
	DDX_Control(pDX, IDC_CMB_SV, m_cmbSV);
	DDX_Control(pDX, IDC_CMB_BUZZ, m_cmbBuzz);
}

BEGIN_MESSAGE_MAP(DlgConfigPilot, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TIMERS, &DlgConfigPilot::OnLvnItemchangedListTimers)
END_MESSAGE_MAP()


// DlgConfigPilot 訊息處理常式


BOOL DlgConfigPilot::OnInitDialog()
{
	CDialog::OnInitDialog();
	RECT rect;
	m_listState.GetClientRect(&rect);
	int Width = rect.right - rect.left;
	int iconsize;
	iconsize = GetIconSize();
	m_pImageList->Create(iconsize, iconsize, ILC_COLOR8, 2, 2);

	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GON));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GOFF));
	m_pImageList->SetBkColor(CLR_NONE);
	//	m_listValves.SubclassDlgItem(IDC_LIST_ValveS, this);
	m_listState.SetImageList(m_pImageList, LVSIL_SMALL);
	m_listState.InsertColumn(0, _T("編號"), LVCFMT_LEFT, Width*0.1);
	m_listState.InsertColumn(1, _T("狀態名稱"), LVCFMT_LEFT, Width*0.3);
	m_listState.InsertColumn(2, _T("紅燈設定"), LVCFMT_LEFT, Width*0.1);
	m_listState.InsertColumn(3, _T("黃燈設定"), LVCFMT_LEFT, Width*0.1);
	m_listState.InsertColumn(4, _T("綠燈設定"), LVCFMT_LEFT, Width*0.1);
	m_listState.InsertColumn(5, _T("蜂鳴設定"), LVCFMT_LEFT, Width*0.1);
	m_listState.SetColumnColors(2, RGB(128, 0, 0), RGB(255, 255, 255));
	m_listState.SetColumnColors(3, RGB(128,128, 0), RGB(255, 255, 255));
	m_listState.SetColumnColors(4, RGB(0, 128, 0), RGB(255, 255, 255));
	m_cmbSV.InsertString(0, _T("Off"));
	m_cmbSV.InsertString(1, _T("Flash"));
	m_cmbSV.InsertString(2, _T("On"));
	m_cmbBuzz.InsertString(0, _T("Off"));
	m_cmbBuzz.InsertString(1, _T("On"));
	m_cmbBuzz.InsertString(2, _T("10Secs"));
	m_cmbBuzz.InsertString(3, _T("30Secs"));
	m_cmbBuzz.InsertString(4, _T("90Secs"));
	m_listState.SetColumnEditor(2, &DlgConfigPilot::InitEditor, &DlgConfigPilot::EndEditor, &m_cmbSV);
	m_listState.SetColumnEditor(3, &DlgConfigPilot::InitEditor, &DlgConfigPilot::EndEditor, &m_cmbSV);
	m_listState.SetColumnEditor(4, &DlgConfigPilot::InitEditor, &DlgConfigPilot::EndEditor, &m_cmbSV);
	m_listState.SetColumnEditor(5, &DlgConfigPilot::InitEditor, &DlgConfigPilot::EndEditor, &m_cmbBuzz);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
}
void DlgConfigPilot::RefreshPage()
{
	CString strV;
	for (int i = 0; i < m_listState.GetItemCount(); i++)
	{
		LVITEM item;
		MPilot *pPilot;
		pPilot = m_pMachine->GetPilot();
		if (pPilot != NULL)
		{
			item.iItem = i;
			item.mask = LVIF_IMAGE;
			if (m_listState.GetItem(&item))
			{
			}
		}
	}
}
void DlgConfigPilot::SetMachine(MachineBase *pM)
{
	TabPageAutoma::SetMachine(pM);
}

void DlgConfigPilot::MachineMessage(MMessage *pMsg)
{
	switch (pMsg->MsgType)
	{
	case MMessage::MESSAGETYPE::MachineComplete:
		if (m_pMachine != NULL)
		{
			MPilot *pPilot;
			pPilot = m_pMachine->GetPilot();
			if (pPilot != NULL)
			{
				for (int i = 0; i < MPilot::PilotState::UNKNOW; i++)
				{
					int intSV;
					CString strID,strSV,strT;
					strID.Format(_T("%d"), i+1);
					m_listState.InsertItem(i, strID, 0);
					m_listState.SetItemData(i, (DWORD_PTR)m_pMachine);

					m_listState.SetItemText(i, 1, pPilot->GetStateName((MPilot::PilotState)i));
					intSV = pPilot->GetRLightSet((MPilot::PilotState)i);
					strSV = (intSV == 0 ? _T("Off") : (intSV > 0 ? _T("Flash") : _T("On")));
					m_listState.SetItemText(i, 2, strSV);
					intSV = pPilot->GetRLightSet((MPilot::PilotState)i);
					strSV = (intSV == 0 ? _T("Off") : (intSV > 0 ? _T("Flash") : _T("On")));
					m_listState.SetItemText(i, 3, strSV);
					intSV = pPilot->GetRLightSet((MPilot::PilotState)i);
					strSV = (intSV == 0 ? _T("Off") : (intSV > 0 ? _T("Flash") : _T("On")));
					m_listState.SetItemText(i, 4, strSV);
					intSV = pPilot->GetBuzzSet((MPilot::PilotState)i);
					strT.Format(_T("%dSecs"),intSV);
					strSV = (intSV == 0 ? _T("Off") : (intSV < 0 ? _T("On") : strT));
					m_listState.SetItemText(i, 5, strSV);
				}
			}
		}
		break;
	}
}
BOOL DlgConfigPilot::InitEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	ASSERT(*pWnd);
	switch (nColumn)
	{
	case 2:
	case 3:
	case 4:
	case 5:
	{
		CComboBox *pCmb = dynamic_cast<CComboBox *>(*pWnd);
		pCmb->SelectString(0, strSubItemText);
	}
	break;
	}
	return TRUE;
}
BOOL DlgConfigPilot::EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	CString strV;
	Machine *pM = (Machine *)dwItemData;
	double dblV;
	ASSERT(pWnd);
	switch (nColumn)
	{
		case 2:
		case 3:
		case 4:
		case 5:
		{
			(*pWnd)->GetWindowText(strV);
			CComboBox *pCmb = (CComboBox *)(*pWnd);
			int intSel=pCmb->GetCurSel();
			int intSV;
			MPilot* pP=pM->GetPilot();
			if (pP != NULL)
			{
				if (nColumn == 5)
				{
					switch (intSel)
					{
					case 0:
						intSV = 0;
						break;
					case 1:
						intSV = -1;
						break;
					case 2: //10S
						intSV = 10;
						break;
					case 3: //30S
						intSV = 30;
						break;
					case 4: //90S
						intSV = 90;
						break;
					}
				}else{
					intSV = (intSel == 2 ? -1 : intSel);
				}
				pM->OpenMachineMDB();
				switch (nColumn)
				{
				case 2: //R
					pP->SetRLightSet(MPilot::PilotState(nRow), intSV);
					break;
				case 3: //Y
					pP->SetYLightSet(MPilot::PilotState(nRow), intSV);
					break;
				case 4: //G
					pP->SetGLightSet(MPilot::PilotState(nRow), intSV);
					break;
				case 5: //Buzz
					pP->SetBuzzSet(MPilot::PilotState(nRow), intSV);
					break;
				}
				pM->SaveMachineData(pP);
				pM->CloseMachineMDB();
			}
			strSubItemText = strV;
		}
		break;
	}
	return TRUE;
}


void DlgConfigPilot::OnLvnItemchangedListTimers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}
