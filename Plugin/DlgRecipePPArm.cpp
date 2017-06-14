// DlgRecipePPArm.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgRecipePPArm.h"

// DlgRecipePPArm 對話方塊

IMPLEMENT_DYNAMIC(DlgRecipePPArm, CDialog)

DlgRecipePPArm::DlgRecipePPArm(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_RECIPE_PPARM)
{
}

DlgRecipePPArm::~DlgRecipePPArm()
{
}

void DlgRecipePPArm::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RECIPES, m_listPPArms);
	DDX_Control(pDX, IDC_EDIT, m_editValue);
	DDX_Control(pDX, IDC_CMB_TYPE, m_cmbType);
	DDX_Control(pDX, IDC_CMB_FEEDER, m_cmbFeeder);
	DDX_Control(pDX, IDC_LIST_FEEDER, m_listFeeders);
}

BEGIN_MESSAGE_MAP(DlgRecipePPArm, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FEEDER, &DlgRecipePPArm::OnNMRClickListFeeder)
END_MESSAGE_MAP()


// DlgRecipePPArm 訊息處理常式


BOOL DlgRecipePPArm::OnInitDialog()
{
	CDialog::OnInitDialog();
	RECT rect;
	m_listPPArms.GetClientRect(&rect);
	int Width = rect.right - rect.left;
	int iconsize;
	iconsize = GetIconSize();
	m_listPPArms.InsertColumn(0, _T("取臂編號"), LVCFMT_LEFT, Width*0.1);
	m_listPPArms.InsertColumn(1, _T("型式設定"), LVCFMT_LEFT, Width*0.1);
	m_listPPArms.InsertColumn(2, _T("使用供料夾"), LVCFMT_LEFT, Width*0.1);
	m_listPPArms.InsertColumn(3, _T("插件偏移X"), LVCFMT_RIGHT, Width*0.1);
	m_listPPArms.InsertColumn(4, _T("插件偏移Y"), LVCFMT_RIGHT, Width*0.1);
	m_listPPArms.InsertColumn(5, _T("插件偏移Z"), LVCFMT_RIGHT, Width*0.1);
	m_listPPArms.InsertColumn(6, _T("插件偏移S"), LVCFMT_RIGHT, Width*0.1);
	m_listPPArms.SetColumnEditor(1, &DlgRecipePPArm::InitEditor_Arm, &DlgRecipePPArm::EndEditor_Arm, &m_cmbType);
	m_listPPArms.SetColumnEditor(2, &DlgRecipePPArm::InitEditor_Arm, &DlgRecipePPArm::EndEditor_Arm, &m_cmbFeeder);
	m_listPPArms.SetColumnEditor(3, NULL, &DlgRecipePPArm::EndEditor_Arm, &m_editValue);
	m_listPPArms.SetColumnEditor(4, NULL, &DlgRecipePPArm::EndEditor_Arm, &m_editValue);
	m_listPPArms.SetColumnEditor(5, NULL, &DlgRecipePPArm::EndEditor_Arm, &m_editValue);
	m_listPPArms.SetColumnEditor(6, NULL, &DlgRecipePPArm::EndEditor_Arm, &m_editValue);
	m_listFeeders.InsertColumn(0, _T("供料編號"), LVCFMT_LEFT, Width*0.1);
	m_listFeeders.InsertColumn(1, _T("元件編號"), LVCFMT_LEFT, Width*0.2);
	m_listFeeders.InsertColumn(2, _T("元件名稱"), LVCFMT_LEFT, Width*0.4);
	m_listPPArms.InsertColumn(3, _T("取料位X"), LVCFMT_RIGHT, Width*0.1);
	m_listPPArms.InsertColumn(3, _T("取料位Y"), LVCFMT_RIGHT, Width*0.1);
	m_listPPArms.InsertColumn(3, _T("取料位Z"), LVCFMT_RIGHT, Width*0.1);
	m_listFeeders.SetColumnEditor(1, &DlgRecipePPArm::InitEditor_Feeder, &DlgRecipePPArm::EndEditor_Feeder,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void DlgRecipePPArm::MachineMessage(MMessage *pMsg)
{
	CString strV;
	switch (pMsg->MsgType)
	{
	case MMessage::MESSAGETYPE::MachineComplete:
		if (m_pMachine != NULL)
		{
			int count;
			CStringProvider * pSP=m_pMachine->GetStringProvider();
			Machine *pM = (Machine*)m_pMachine;
			m_listFeeders.DeleteAllItems();
			m_listPPArms.DeleteAllItems();
			m_cmbType.ResetContent();
			strV = pSP->GetString(_T("String"), _T("Disable"), _T("Disable"));
			m_cmbType.AddString(strV);
			strV = pSP->GetString(_T("String"), _T("Vaccum"), _T("Vaccum"));
			m_cmbType.AddString(strV);
			strV = pSP->GetString(_T("String"), _T("Clamp"), _T("Clamp"));
			m_cmbType.AddString(strV);
			strV = pSP->GetString(_T("String"), _T("Dispensor"), _T("Dispensor"));
			m_cmbType.AddString(strV);

			m_cmbFeeder.ResetContent();
			count = sizeof(pM->m_pPickPlug->m_pFeeder) / sizeof(void*);
			for (int i = 0; i < count; i++)
			{
				ComponentData* pComp=NULL;
				m_listFeeders.AddItem(i, 0, pM->m_pPickPlug->m_pFeeder[i]->m_strName);
				m_listFeeders.SetItemData(i, (DWORD_PTR)pM->m_pPickPlug->m_pFeeder[i]);

				if (pM->m_pPickPlug->m_pFeeder[i]->m_strComponentID.GetLength() > 0)
				{
					pComp=pM->GetComponent(pM->m_pPickPlug->m_pFeeder[i]->m_strComponentID);

					m_listFeeders.SetItemText(i, 1, pComp->m_strID);
					m_listFeeders.SetItemText(i, 2, pComp->m_strName);
				}
				m_cmbFeeder.AddString(pM->m_pPickPlug->m_pFeeder[i]->m_strName);
			}
			count = sizeof(pM->m_pPickPlug->m_pArm) / sizeof(void*);
			
			for (int i = 0; i < count; i++)
			{
				MPPArm *pArm = pM->m_pPickPlug->m_pArm[i];
				m_listPPArms.InsertItem(i, pArm->m_ID, 0);
				m_listPPArms.SetItemData(i, (DWORD_PTR)pArm);
				m_cmbType.GetLBText(pArm->GetArmType(), strV);
				m_listPPArms.SetItemText(i, 1, strV);
				m_cmbFeeder.GetLBText(pArm->GetFeeder(), strV);
				m_listPPArms.SetItemText(i, 2, strV);
				strV.Format(_T("%10.3f"), pArm->GetPlugOffset().x);
				m_listPPArms.SetItemText(i, 3, strV);
				strV.Format(_T("%10.3f"), pArm->GetPlugOffset().y);
				m_listPPArms.SetItemText(i, 4, strV);
				strV.Format(_T("%10.3f"), pArm->GetPlugOffset().z);
				m_listPPArms.SetItemText(i, 5, strV);
				strV.Format(_T("%10.3f"), pArm->GetPlugOffset().s);
				m_listPPArms.SetItemText(i, 6, strV);
			}
		}
		break;
	}
}
BOOL DlgRecipePPArm::InitEditor_Arm(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	switch (nColumn)
	{
		case 1:
		case 2:
		{
			CComboBox *pCmb = (CComboBox *)(*pWnd);
			pCmb->SelectString(0, strSubItemText);
		}
		break;
	};
	return TRUE;
}
BOOL DlgRecipePPArm::EndEditor_Arm(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	CString strV;
	MPPArm *pArm =(MPPArm*)dwItemData;
	Machine *pM = (Machine*)(pArm->GetTopParent());
	double dblV;
	ASSERT(pWnd);
	switch (nColumn)
	{
		case 1:
		{
			CComboBox *pCmb = (CComboBox *)(*pWnd);
			int intSel = pCmb->GetCurSel();
			pArm->SetArmType((MPPArm::ArmType)intSel);
			pCmb->GetWindowText(strSubItemText);
			pM->OpenRecipeMDB();
			pM->SaveRecipeData(pArm);
			pM->CloseRecipeMDB();
		}
		break;
		case 2:
		{
			CComboBox *pCmb = (CComboBox *)(*pWnd);
			int intSel = pCmb->GetCurSel();
			MPickPlug *pPP = (MPickPlug*)(pArm->GetParent());
			pArm->SetFeeder(intSel);
			pCmb->GetWindowText(strSubItemText);
			pM->OpenRecipeMDB();
			pM->SaveRecipeData(pArm);
			pM->CloseRecipeMDB();
		}
		break;
		case 3:
		case 4:
		case 5:
		case 6:
			{
				DCoordinate cdOffset=pArm->GetPlugOffset();
				(*pWnd)->GetWindowText(strV);
				dblV = _wtof(strV);
				switch (nColumn)
				{
				case 3:
					cdOffset.x = dblV;
					break;
				case 4:
					cdOffset.y = dblV;
					break;
				case 5:
					cdOffset.z = dblV;
					break;
				case 6:
					cdOffset.s = dblV;
					break;
				}
				pArm->SetPlugOffset(cdOffset);
				strSubItemText.Format(_T("%10.3f"), dblV);
				(*pWnd)->SetWindowText(strSubItemText);
				pM->OpenRecipeMDB();
				pM->SaveRecipeData(pArm);
				pM->CloseRecipeMDB();
			}
			break;
	}
	return TRUE;
}
BOOL DlgRecipePPArm::InitEditor_Feeder(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	MFeeder *pFeeder = (MFeeder*)dwItemData;
	Machine *pM = (Machine*)(pFeeder->GetTopParent());
	CListCtrlEx* pList = (CListCtrlEx*)pThis;
	switch (nColumn)
	{
	case 1:
	{
		INT_PTR nRet = -1;
		DlgComponentSearch *pDlg;
		CString strID , strName ;
		strID = pList->GetItemText(nRow, 1);
		strName = pList->GetItemText(nRow, 2);
		pDlg = new DlgComponentSearch(*pWnd, pM, &strID, &strName);
		nRet=pDlg->DoModal();
		delete pDlg;
		if (nRet==IDOK && strID.GetLength() > 0)
		{
			pFeeder->m_strComponentID = strID;
			pM->OpenMachineMDB();
			pM->SaveMachineData(pFeeder);
			pM->CloseMachineMDB();
			pList->SetItemText(nRow, 1, strID);
			pList->SetItemText(nRow, 2, strName);
		}
	}
	break;
	};
	return TRUE;

}
BOOL DlgRecipePPArm::EndEditor_Feeder(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	return TRUE;
}

void DlgRecipePPArm::OnNMRClickListFeeder(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	MFeeder *pFeeder = (MFeeder*)m_listFeeders.GetItemData((pNMItemActivate->iItem));
	if (AfxMessageBox(_T("是否要清除")+pFeeder->m_strName+_T("的設定？"),MB_YESNO) == IDYES)
	{
		Machine *pM = (Machine*)(pFeeder->GetTopParent());
		m_listFeeders.SetItemText(pNMItemActivate->iItem, 1, _T(""));
		m_listFeeders.SetItemText(pNMItemActivate->iItem, 2, _T(""));
		pFeeder->m_strComponentID = _T("");
		pM->OpenMachineMDB();
		pM->SaveMachineData(pFeeder);
		pM->CloseMachineMDB();

	}
	*pResult = 0;
}
