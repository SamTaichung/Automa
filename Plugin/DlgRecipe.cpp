// DlgRecipe.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgRecipe.h"

// DlgRecipe 對話方塊
#define IDM_INSERT  4001
#define IDM_DELETE  4002
IMPLEMENT_DYNAMIC(DlgRecipe, CDialog)

DlgRecipe::DlgRecipe(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_RECIPE)
{
	m_SelectItemIndex = -1;
}

DlgRecipe::~DlgRecipe()
{
}

void DlgRecipe::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RECIPES, m_listRecipes);
	DDX_Control(pDX, IDC_EDIT, m_editValue);
}

BEGIN_MESSAGE_MAP(DlgRecipe, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TIMERS, &DlgRecipe::OnLvnItemchangedListTimers)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RECIPES, &DlgRecipe::OnLvnItemchangedListRecipes)
	ON_BN_CLICKED(IDC_BTN_SAVE, &DlgRecipe::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &DlgRecipe::OnBnClickedBtnCancel)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RECIPES, &DlgRecipe::OnNMRClickListRecipes)
END_MESSAGE_MAP()


// DlgRecipe 訊息處理常式


BOOL DlgRecipe::OnInitDialog()
{
	CDialog::OnInitDialog();
	RECT rect;
	m_listRecipes.GetClientRect(&rect);
	int Width = rect.right - rect.left;
	int iconsize;
	iconsize = GetIconSize();
	m_listRecipes.InsertColumn(0, _T("編號"), LVCFMT_LEFT, Width*0.05);
	m_listRecipes.InsertColumn(1, _T("元件編號"), LVCFMT_LEFT, Width*0.1);
	m_listRecipes.InsertColumn(2, _T("插件座標X"), LVCFMT_RIGHT, Width*0.1);
	m_listRecipes.InsertColumn(3, _T("插件座標Y"), LVCFMT_RIGHT, Width*0.1);
	m_listRecipes.InsertColumn(4, _T("插件座標Z"), LVCFMT_RIGHT, Width*0.1);
	m_listRecipes.InsertColumn(5, _T("插件座標S"), LVCFMT_RIGHT, Width*0.1);
	m_listRecipes.InsertColumn(6, _T("補正X"), LVCFMT_RIGHT, Width*0.1);
	m_listRecipes.InsertColumn(7, _T("補正Y"), LVCFMT_RIGHT, Width*0.1);
	m_listRecipes.InsertColumn(8, _T("補正Z"), LVCFMT_RIGHT, Width*0.1);
	m_listRecipes.InsertColumn(9, _T("補正S"), LVCFMT_RIGHT, Width*0.1);
	m_listRecipes.SetColumnEditor(1, &DlgRecipe::InitEditor, &DlgRecipe::EndEditor, NULL);
	m_listRecipes.SetColumnEditor(2, NULL, &DlgRecipe::EndEditor, &m_editValue);
	m_listRecipes.SetColumnEditor(3, NULL, &DlgRecipe::EndEditor, &m_editValue);
	m_listRecipes.SetColumnEditor(4, NULL, &DlgRecipe::EndEditor, &m_editValue);
	m_listRecipes.SetColumnEditor(5, NULL, &DlgRecipe::EndEditor, &m_editValue);
	m_listRecipes.SetColumnEditor(6, NULL, &DlgRecipe::EndEditor, &m_editValue);
	m_listRecipes.SetColumnEditor(7, NULL, &DlgRecipe::EndEditor, &m_editValue);
	m_listRecipes.SetColumnEditor(8, NULL, &DlgRecipe::EndEditor, &m_editValue);
	m_listRecipes.SetColumnEditor(9, NULL, &DlgRecipe::EndEditor, &m_editValue);
	for (int i = 0; i <100; i++)
	{
		CString strID;
		strID.Format(_T("%d"), i + 1);
		m_listRecipes.InsertItem(i, strID);
		m_listRecipes.SetItemData(i, NULL);

	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
}
void DlgRecipe::RefreshPage()
{

}
void DlgRecipe::ReLoadData()
{
	if (m_pMachine != NULL)
	{
		Machine *pM = ((Machine*)m_pMachine);
		MConveyor* pCV = pM->m_pConveyor;
		for (int i = 0; i < 100; i++)
		{
			PluginPoint *pPoint;
			CString strV;
			if (i < pCV->GetPCBRecipe()->GetPlugPointCount())
			{
				pPoint = pCV->GetPCBRecipe()->GetPlugPoint(i);
				m_listRecipes.SetItemText(i, 1, pPoint->m_strComponentID);
				strV.Format(_T("%10.3f"), pPoint->m_cdPlugPosition.x);
				m_listRecipes.SetItemText(i, 2, strV);
				strV.Format(_T("%10.3f"), pPoint->m_cdPlugPosition.y);
				m_listRecipes.SetItemText(i, 3, strV);
				strV.Format(_T("%10.3f"), pPoint->m_cdPlugPosition.z);
				m_listRecipes.SetItemText(i, 4, strV);
				strV.Format(_T("%10.3f"), pPoint->m_cdPlugPosition.s);
				m_listRecipes.SetItemText(i, 5, strV);
				strV.Format(_T("%10.3f"), pPoint->m_cdPlugOffset.x);
				m_listRecipes.SetItemText(i, 6, strV);
				strV.Format(_T("%10.3f"), pPoint->m_cdPlugOffset.y);
				m_listRecipes.SetItemText(i, 7, strV);
				strV.Format(_T("%10.3f"), pPoint->m_cdPlugOffset.z);
				m_listRecipes.SetItemText(i, 8, strV);
				strV.Format(_T("%10.3f"), pPoint->m_cdPlugOffset.s);
				m_listRecipes.SetItemText(i, 9, strV);
			}
			else {
				for (int j = 1; j < 10; j++)
				{
					m_listRecipes.SetItemText(i, j, _T(""));
				}
			}
			m_listRecipes.SetItemData(i, (DWORD_PTR)pCV);
		}
	}
}
void DlgRecipe::MachineMessage(MMessage *pMsg)
{
	CString strMenu;
	switch (pMsg->MsgType)
	{
	case MMessage::MESSAGETYPE::MachineComplete:
		m_PopupMenu.CreatePopupMenu();
		//---------------------語言轉換----------------------------
		strMenu = m_pMachine->GetStringProvider()->GetString(_T("String"), _T("Insert"), _T("Insert"));
		m_PopupMenu.InsertMenu(0, MF_BYPOSITION, IDM_INSERT, strMenu);
		strMenu = m_pMachine->GetStringProvider()->GetString(_T("String"), _T("Delete"), _T("Delete"));
		m_PopupMenu.InsertMenu(1, MF_BYPOSITION, IDM_DELETE, strMenu);
		ReLoadData();
		break;
	}
}
BOOL DlgRecipe::InitEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	MConveyor* pCV = (MConveyor*)dwItemData;
	CListCtrlEx* pList = (CListCtrlEx*)pThis;
	switch (nColumn)
	{
		case 1:
		{
			INT_PTR nRet = -1;
			DlgComponentSearch *pDlg;
			CString strID, strV;
			double dblV;
			Machine *pM = (Machine*)(pCV->GetTopParent());

			strID = pList->GetItemText(nRow, 1);
			pDlg = new DlgComponentSearch(*pWnd, pM, &strID,NULL);
			nRet = pDlg->DoModal();
			delete pDlg;
			if (nRet == IDOK && strID.GetLength() > 0)
			{
				pList->SetItemText(nRow, 1, strID);
				for (int j = 2; j < 10; j++)
				{
					strV = pList->GetItemText(nRow, j);
					dblV = _wtof(strV);
					strV.Format(_T("%10.3f"), dblV);
					pList->SetItemText(nRow, j, strV);
				}
			}
		}
		break;
	}
	return TRUE;
}
BOOL DlgRecipe::EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	CString strV;
	double dblV;
	ASSERT(pWnd);
	switch (nColumn)
	{
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	{
			(*pWnd)->GetWindowText(strV);
			dblV = _wtof(strV);
			strSubItemText.Format(_T("%10.3f"), dblV);
		}
		break;
	}
	return TRUE;
}


void DlgRecipe::OnLvnItemchangedListTimers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}


void DlgRecipe::OnLvnItemchangedListRecipes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}


void DlgRecipe::OnBnClickedBtnSave()
{
	CString strV;
	Machine *pM = ((Machine*)m_pMachine);
	MConveyor* pCV = pM->m_pConveyor;
	PCBRecipe* pPR = pCV->GetPCBRecipe();
	int count = 0;
//	pPR->RemoveAll();
	PluginPoint *pPP;
	for (int i = 0; i < 100; i++)
	{
		strV = m_listRecipes.GetItemText(i, 1);
		strV.Trim();
		if (strV.GetLength() > 0)
		{
			count++;
			if (pPR->GetPlugPointCount() < count)
			{
				pPP = new PluginPoint();
				pPR->AddPlugPoint(pPP);
			}
			else {
				pPP = pPR->GetPlugPoint(count - 1);
			}
			if (pPP->m_strComponentID.Compare(strV) != 0)
			{
				pPP->m_strComponentID = strV;
				pPP->m_bPlugFinish = false;
			}
			strV = m_listRecipes.GetItemText(i, 2);
			pPP->m_cdPlugPosition.x = _wtof(strV);
			strV = m_listRecipes.GetItemText(i, 3);
			pPP->m_cdPlugPosition.y = _wtof(strV);
			strV = m_listRecipes.GetItemText(i, 4);
			pPP->m_cdPlugPosition.z = _wtof(strV);
			strV = m_listRecipes.GetItemText(i, 5);
			pPP->m_cdPlugPosition.s = _wtof(strV);
			strV = m_listRecipes.GetItemText(i, 6);
			pPP->m_cdPlugOffset.x = _wtof(strV);
			strV = m_listRecipes.GetItemText(i, 7);
			pPP->m_cdPlugOffset.y = _wtof(strV);
			strV = m_listRecipes.GetItemText(i, 8);
			pPP->m_cdPlugOffset.z = _wtof(strV);
			strV = m_listRecipes.GetItemText(i, 9);
			pPP->m_cdPlugOffset.s = _wtof(strV);
		}
	}
	while (pPR->GetPlugPointCount() > count)
	{
		pPR->RemovePlugPoint(count);
	}
	pM->OpenRecipeMDB();
	pM->SaveRecipeData(pCV);
	pM->CloseRecipeMDB();
	ReLoadData();
}


void DlgRecipe::OnBnClickedBtnCancel()
{
	ReLoadData();
}


void DlgRecipe::OnNMRClickListRecipes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POINT point;
	m_SelectItemIndex = pNMItemActivate->iItem;
	GetCursorPos(&point);
	m_PopupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);
	*pResult = 0;
}


BOOL DlgRecipe::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	switch (wParam)
	{
	case IDM_INSERT:
		if (m_SelectItemIndex>=0)
		{
			CString strV;
			for (int i = 98; i > m_SelectItemIndex; i--)
			{
				strV.Format(_T("%d"), i + 2);
				m_listRecipes.SetItemText(i + 1, 0, strV);
				for (int j = 1; j < m_listRecipes.GetColumnCount(); j++)
				{
					strV = m_listRecipes.GetItemText(i, j);
					m_listRecipes.SetItemText(i+1, j,strV);
				}
			}
			for (int j = 0; j < m_listRecipes.GetColumnCount(); j++)
			{
				strV = _T("");
				m_listRecipes.SetItemText(m_SelectItemIndex, j, strV);
			}
		}
		break;
	case IDM_DELETE:
		if (m_SelectItemIndex >= 0)
		{

		}
		break;
	}
	return TabPageAutoma::OnCommand(wParam, lParam);
}
