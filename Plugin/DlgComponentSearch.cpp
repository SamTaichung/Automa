// DlgComponentSearch.cpp : ��@��
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgComponentSearch.h"
#include "afxdialogex.h"


// DlgComponentSearch ��ܤ��

IMPLEMENT_DYNAMIC(DlgComponentSearch, CDialog)

DlgComponentSearch::DlgComponentSearch(CWnd* pParent, Machine *pM, CString* pstrID, CString* pstrName)
	: CDialog(IDD_DLG_LIB, pParent),m_pMachine(pM), m_pstrSelectID(pstrID), m_pstrSelectName(pstrName)
{
	m_bInit = false;
}

DlgComponentSearch::~DlgComponentSearch()
{
}

void DlgComponentSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listComponents);
}


BEGIN_MESSAGE_MAP(DlgComponentSearch, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &DlgComponentSearch::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &DlgComponentSearch::OnBnClickedBtnSearch)
END_MESSAGE_MAP()


// DlgComponentSearch �T���B�z�`��


BOOL DlgComponentSearch::OnInitDialog()
{
	CDialog::OnInitDialog();
	RECT rect;
	m_listComponents.GetClientRect(&rect);
	int Width = rect.right - rect.left;
	m_listComponents.InsertColumn(0, _T("����s��"), LVCFMT_LEFT, Width*0.3);
	m_listComponents.InsertColumn(1, _T("����W��"), LVCFMT_LEFT, Width*0.6);

	if (m_pMachine != NULL)
	{
		map<CString,ComponentData*>* pComponentDatas;
		CString strDefault;
		int intDefault = -1;
		int Count;
		strDefault = *m_pstrSelectID;
		strDefault.Trim();
		pComponentDatas=m_pMachine->GetComponentDatas();
		Count = 0;
		for (auto& itComp : *pComponentDatas) //�qComponentDatas�C�|�X�C�@��
		{
			ComponentData* pComponent;
			pComponent = itComp.second;
			m_listComponents.InsertItem(Count, pComponent->m_strID, 0);
			if (strDefault.Compare(pComponent->m_strID)==0)
			{
				intDefault = Count;
			}
			m_listComponents.SetItemText(Count, 1, pComponent->m_strName);
			m_listComponents.SetItemData(Count, (DWORD_PTR)pComponent);
			Count++;
		}
		if (intDefault >= 0)
		{
			m_listComponents.SetFocus();
			m_listComponents.SetItemState(-1, 0, LVIS_SELECTED);
			m_listComponents.SetItemState(intDefault, LVIS_SELECTED, LVIS_SELECTED);
		}
		m_bInit = true;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}


void DlgComponentSearch::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (m_bInit)
	{
		*m_pstrSelectID = m_listComponents.GetItemText(pNMLV->iItem, 0);
		if (m_pstrSelectName != NULL)
		{
			*m_pstrSelectName = m_listComponents.GetItemText(pNMLV->iItem, 1);
		}
	}
	*pResult = 0;
}


void DlgComponentSearch::OnBnClickedBtnSearch()
{
	// TODO: �b���[�J����i���B�z�`���{���X
}
