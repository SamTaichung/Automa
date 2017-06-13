// DlgComponents.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgComponents.h"
#include "afxdialogex.h"


// DlgComponents 對話方塊

IMPLEMENT_DYNAMIC(DlgComponents, TabPageAutoma)

DlgComponents::DlgComponents(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_COMPONENT, pParent)
{
	m_bAddMode = false;
	m_bEditMode = false;
	m_intSelectIndex = -1;
	m_pComponent = NULL;
}

DlgComponents::~DlgComponents()
{
}

void DlgComponents::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COMPONENTS, m_listComponents);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BTN_DEL, m_btnDel);
	DDX_Control(pDX, IDC_EDIT_HIGH, m_editHigh);
}


BEGIN_MESSAGE_MAP(DlgComponents, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COMPONENTS, &DlgComponents::OnLvnItemchangedListComponents)
	ON_BN_CLICKED(IDC_BTN_SAVE, &DlgComponents::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &DlgComponents::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_ADD, &DlgComponents::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &DlgComponents::OnBnClickedBtnDel)
	ON_EN_SETFOCUS(IDC_EDIT_ID, &DlgComponents::OnEnSetfocusEditItem)
	ON_EN_SETFOCUS(IDC_EDIT_NAME, &DlgComponents::OnEnSetfocusEditItem)
	ON_EN_SETFOCUS(IDC_EDIT_HIGH, &DlgComponents::OnEnSetfocusEditItem)
END_MESSAGE_MAP()


// DlgComponents 訊息處理常式


BOOL DlgComponents::OnInitDialog()
{
	TabPageAutoma::OnInitDialog();

	RECT rect;
	m_listComponents.GetClientRect(&rect);
	int Width = rect.right - rect.left;
	//	m_listValves.SubclassDlgItem(IDC_LIST_ValveS, this);
	m_listComponents.InsertColumn(0, _T("編號"), LVCFMT_LEFT, Width*0.8);
	RefreshBtns();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
}
void DlgComponents::RefreshPage()
{
	CString strV;

}
void DlgComponents::SetMachine(MachineBase *pM)
{
	TabPageAutoma::SetMachine(pM);

}

void DlgComponents::MachineMessage(MMessage *pMsg)
{
	switch (pMsg->MsgType)
	{
	case MMessage::MESSAGETYPE::MachineComplete:
		ReloadData();
		break;
	}
}
void DlgComponents::ReloadData()
{
	m_listComponents.DeleteAllItems();
	if (m_pMachine != NULL)
	{
		Machine *pM = (Machine*)m_pMachine;
		map<CString, ComponentData*>* pComponents;
		map<CString, ComponentData*>::iterator itComp;
		pComponents = pM->GetComponentDatas();
		int Count = 0;
		for (itComp = pComponents->begin(); itComp != pComponents->end(); itComp++)
		{
			m_listComponents.InsertItem(Count, (*itComp).first, 0);
			m_listComponents.SetItemData(Count, (DWORD_PTR)((*itComp).second));
			Count++;
		}
	}
}
void DlgComponents::OnSelectComponent()
{
	if (m_pComponent != NULL)
	{
		m_editID.SetWindowText(m_pComponent->m_strID);
		m_editName.SetWindowText(m_pComponent->m_strName);
		m_editHigh.SetValueExchange(m_pComponent->m_dblHight);
	}
}
void DlgComponents::OnLvnItemchangedListComponents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState & LVIS_FOCUSED)
	{
		if (m_pMachine != NULL)
		{
			Machine *pM = (Machine*)m_pMachine;
			CString strV;
			strV=m_listComponents.GetItemText(pNMLV->iItem, 0);
			strV.Trim();
			
		}
		m_intSelectIndex = pNMLV->iItem;
		m_pComponent = (ComponentData*)(m_listComponents.GetItemData(m_intSelectIndex));
		OnSelectComponent();
	}
	*pResult = 0;
}


void DlgComponents::OnBnClickedBtnSave()
{
	if (m_pMachine != NULL)
	{
		Machine *pM = (Machine*)m_pMachine;
		CString strOldID,strV;
		ComponentData* pComponent;
		if (m_bAddMode) //新增
		{
			pComponent = new ComponentData();
		}else{
			pComponent = m_pComponent;
			strOldID = m_pComponent->m_strID;
		}
		m_editID.GetWindowText(pComponent->m_strID);
		pComponent->m_strID.Trim();
		m_editName.GetWindowText(pComponent->m_strName);
		pComponent->m_strName.Trim();
		m_editHigh.GetWindowText(strV);
		pComponent->m_dblHight = _wtof(strV);
		if (m_bAddMode) //新增
		{
			pM->AddComponent(pComponent);
			m_intSelectIndex = m_listComponents.GetItemCount();
			m_listComponents.InsertItem(m_intSelectIndex, pComponent->m_strID, 0);
			m_pComponent = pComponent;
		}else{
			pM->AddComponent(pComponent);
			m_listComponents.SetItemText(m_intSelectIndex, 0, m_pComponent->m_strID);
		}
	}
	m_bAddMode = false;
	m_bEditMode = false;
	RefreshBtns();

}


void DlgComponents::OnBnClickedBtnCancel()
{
	OnSelectComponent();
	m_bAddMode = false;
	m_bEditMode = false;
	RefreshBtns();

}


void DlgComponents::OnBnClickedBtnAdd()
{
	m_bAddMode = true;
	m_bEditMode = true;
	m_editID.SetWindowText(_T(""));
	m_editName.SetWindowText(_T(""));
	m_dblHigh = 0;
	m_editHigh.SetValueExchange(m_dblHigh);
	m_editHigh.SetWindowText(_T(""));
	m_editID.SetFocus();
	RefreshBtns();

}


void DlgComponents::OnBnClickedBtnDel()
{
	if (m_pComponent!=NULL)
	{
		if (m_pMachine != NULL)
		{
			Machine *pM = (Machine*)m_pMachine;
			pM->DelComponent(m_pComponent);
		}
		m_listComponents.DeleteItem(m_intSelectIndex);
		m_pComponent = NULL;
		m_intSelectIndex = -1;
	}
	OnSelectComponent();
}
void DlgComponents::RefreshBtns()
{
	if (m_bEditMode)
	{
		m_btnAdd.EnableWindow(FALSE);
		m_btnDel.EnableWindow(FALSE);
		m_btnCancel.EnableWindow(TRUE);
		m_btnSave.EnableWindow(TRUE);
	}
	else {
		if (m_pComponent!=NULL)
		{
			m_btnDel.EnableWindow(TRUE);
		}
		else {
			m_btnDel.EnableWindow(FALSE);
		}
		m_btnAdd.EnableWindow(TRUE);
		m_btnCancel.EnableWindow(FALSE);
		m_btnSave.EnableWindow(FALSE);
	}
}
void DlgComponents::OnEnSetfocusEditItem()
{
	if (!m_bEditMode)
	{
		m_bEditMode = true;
		if (m_pComponent!=NULL)
		{
			m_bAddMode = false;
		}
		else {
			m_bAddMode = true;

		}
	}

	RefreshBtns();
}


