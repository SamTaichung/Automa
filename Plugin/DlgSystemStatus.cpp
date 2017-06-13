// DlgSystemStatus.cpp : 實作檔
//

#include "stdafx.h"
#include "DlgSystemStatus.h"
#include "Plugin.h"

// DlgSystemStatus 對話方塊
IMPLEMENT_DYNAMIC(DlgSystemStatus, CDialog)
DlgSystemStatus::DlgSystemStatus(CWnd* pParent,MachineBase *pM)
	: CDialog(IDD_DLG_SYSTEM_STATUS, pParent),m_pMachine(pM)
{
	m_pImageList=new CImageList();
}

DlgSystemStatus::~DlgSystemStatus()
{
	delete m_pImageList;
}

void DlgSystemStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SYSTEM, m_treeMap);
}


BEGIN_MESSAGE_MAP(DlgSystemStatus, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// DlgSystemStatus 訊息處理常式
void DlgSystemStatus::AddNode(MBase *pB,HTREEITEM hParentItem)
{
	HTREEITEM hItem;

		CString strName;
		hItem=m_treeMap.InsertItem(pB->m_strName,hParentItem);
		std::map<CString,MBase *>::iterator itChild;
		for (itChild=pB->m_Childs.begin();itChild!=pB->m_Childs.end();++itChild)
		{
			MBase * p_C=(*itChild).second;
			AddNode(p_C,hItem);
		}
		m_treeMap.Expand(hItem,TVE_EXPAND);
		m_treeMap.SetItem(hItem,TVIF_IMAGE+TVIF_PARAM,NULL,0,0,0,0,(LPARAM)pB);

}
BOOL DlgSystemStatus::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT(m_pImageList != NULL);    // serious allocation failure checking
	int iconsize =48;
	m_pImageList->Create(iconsize, iconsize, ILC_COLOR8, 2, 2);
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_INFO));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GOFF));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GON));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_HOME));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_ALARM));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_EMGSTOP));
	m_treeMap.SetImageList(m_pImageList,TVSIL_NORMAL);
	m_treeMap.DeleteAllItems();
	AddNode(m_pMachine,NULL);
	SetTimer(0,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}
void DlgSystemStatus::RefreshNode(HTREEITEM hItem)
{
	int iImage,iItemImage,iItemImageSel;
	MBase *pB=(MBase*)m_treeMap.GetItemData(hItem);
	CString strName;
	if (pB->m_State==MBase::STATE::ACTION || pB->m_State==MBase::STATE::HOME){
		strName = pB->m_strName+_T("[")+pB->GetStepName()+_T("]");
	}
	else {
		strName = pB->m_strName + _T("[") + pB->GetStateName() + _T("]");
	}
	switch(pB->m_State)
	{
		case MBase::STATE::INIT:
		case MBase::STATE::IDLE:
		case MBase::STATE::ACTION:
		case MBase::STATE::HOME:
		case MBase::STATE::ERRHAPPEN:
		case MBase::STATE::EMSTOP:
			iImage=pB->m_State;
			break;
		default:
			iImage=6;
			break;
	}
	m_treeMap.GetItemImage(hItem,iItemImage,iItemImageSel);
	if (iImage!=iItemImage)
	{
		m_treeMap.SetItemImage(hItem,iImage,iImage);
	}
	CString strText=m_treeMap.GetItemText(hItem);
	if (strText!=strName)
	{
		m_treeMap.SetItemText(hItem,strName);
	}
	if (m_treeMap.ItemHasChildren(hItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_treeMap.GetChildItem(hItem);
		while (hChildItem != NULL)
		{
			RefreshNode(hChildItem);
			hNextItem = m_treeMap.GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}
	}
}
void DlgSystemStatus::OnTimer(UINT nIDEvent)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	RefreshNode(m_treeMap.GetRootItem());
	CDialog::OnTimer(nIDEvent);
}
