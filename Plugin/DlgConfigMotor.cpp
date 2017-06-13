// DlgConfigMotor.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "DlgConfigMotor.h"
#include "Machine.h"

// DlgConfigMotor 對話方塊

IMPLEMENT_DYNAMIC(DlgConfigMotor, CDialog)

DlgConfigMotor::DlgConfigMotor(CWnd* pParent /*=NULL*/)
	: TabPageAutoma(IDD_DLG_MOTOR)
{
	m_pImageList = new CImageList();
	m_pSelectedMotor = NULL;
}

DlgConfigMotor::~DlgConfigMotor()
{
	delete m_pImageList;
}

void DlgConfigMotor::DoDataExchange(CDataExchange* pDX)
{
	TabPageAutoma::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_LIST_MOTOR, m_lstMotors);
	DDX_Control(pDX, IDC_STC_POS, m_stcPos);
	DDX_Control(pDX, IDC_STC_LAST, m_stcLast);
	DDX_Control(pDX, IDC_STC_SPEED, m_stcSpeed);
	DDX_Control(pDX, IDC_LIST_MOTORS, m_listMotors);
	DDX_Control(pDX, IDC_EDIT_P1, m_editP1);
	DDX_Control(pDX, IDC_EDIT_P2, m_editP2);
	DDX_Control(pDX, IDC_BTN_PITCH0, m_btnPitch[0]);
	DDX_Control(pDX, IDC_BTN_PITCH1, m_btnPitch[1]);
	DDX_Control(pDX, IDC_BTN_PITCH2, m_btnPitch[2]);
	DDX_Control(pDX, IDC_BTN_PITCH3, m_btnPitch[3]);
	DDX_Control(pDX, IDC_BTN_PITCH4, m_btnPitch[4]);
	DDX_Control(pDX, IDC_BTN_PITCH5, m_btnPitch[5]);
	DDX_Control(pDX, IDC_EDIT_HISPEED, m_editHiSpeed);
	DDX_Control(pDX, IDC_EDIT_LOSPEED, m_editLoSpeed);
	DDX_Control(pDX, IDC_EDIT_HIACCTIME, m_editHiAccTime);
	DDX_Control(pDX, IDC_EDIT_HIDESTIME, m_editHiDesTime);
	DDX_Control(pDX, IDC_EDIT_LOACCTIME, m_editLoAccTime);
	DDX_Control(pDX, IDC_EDIT_LODESTIME, m_editLoDesTime);
	DDX_Control(pDX, IDC_LED1, m_btLED[0]);
	DDX_Control(pDX, IDC_LED2, m_btLED[1]);
	DDX_Control(pDX, IDC_LED3, m_btLED[2]);
	DDX_Control(pDX, IDC_LED4, m_btLED[3]);
	DDX_Control(pDX, IDC_LED5, m_btLED[4]);
	DDX_Control(pDX, IDC_LED6, m_btLED[5]);
	DDX_Control(pDX, IDC_LED7, m_btLED[6]);
	DDX_Control(pDX, IDC_LED8, m_btLED[7]);
	DDX_Control(pDX, IDC_LED9, m_btLED[8]);
	DDX_Control(pDX, IDC_LED10, m_btLED[9]);
	DDX_Control(pDX, IDC_LED11, m_btLED[10]);
	DDX_Control(pDX, IDC_LED12, m_btLED[11]);
	DDX_Control(pDX, IDC_LED13, m_btLED[12]);
	DDX_Control(pDX, IDC_LED14, m_btLED[13]);
	DDX_Control(pDX, IDC_LED15, m_btLED[14]);
}


BEGIN_MESSAGE_MAP(DlgConfigMotor, CDialog)
	ON_BN_CLICKED(IDC_BTN_STOP, &DlgConfigMotor::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_HOME, &DlgConfigMotor::OnBnClickedBtnHome)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MOTORS, &DlgConfigMotor::OnLvnItemchangedListMotors)
	ON_BN_CLICKED(IDC_BTN_TEST, &DlgConfigMotor::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_PITCH0, &DlgConfigMotor::OnBnClickedBtnPitch0)
	ON_BN_CLICKED(IDC_BTN_PITCH1, &DlgConfigMotor::OnBnClickedBtnPitch1)
	ON_BN_CLICKED(IDC_BTN_PITCH2, &DlgConfigMotor::OnBnClickedBtnPitch2)
	ON_BN_CLICKED(IDC_BTN_PITCH3, &DlgConfigMotor::OnBnClickedBtnPitch3)
	ON_BN_CLICKED(IDC_BTN_PITCH4, &DlgConfigMotor::OnBnClickedBtnPitch4)
	ON_BN_CLICKED(IDC_BTN_PITCH5, &DlgConfigMotor::OnBnClickedBtnPitch5)
	ON_BN_CLICKED(IDC_BTN_RMOVEP, &DlgConfigMotor::OnBnClickedBtnRmovep)
	ON_BN_CLICKED(IDC_BTN_RMOVEM, &DlgConfigMotor::OnBnClickedBtnRmovem)
	ON_BN_CLICKED(IDC_BTN_SAVE, &DlgConfigMotor::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_P1, &DlgConfigMotor::OnBnClickedBtnP1)
	ON_BN_CLICKED(IDC_BTN_P2, &DlgConfigMotor::OnBnClickedBtnP2)
	ON_BN_CLICKED(IDC_BTN_CONFIG, &DlgConfigMotor::OnBnClickedBtnConfig)
END_MESSAGE_MAP()

void DlgConfigMotor::RefreshPage()
{
	CString strV;
	if (m_pSelectedMotor != NULL)
	{
		WORD wStatus,wBit;
		wStatus = m_pSelectedMotor->GetIOStatus();
		wBit = 0x0001;
		for (int i = 0; i < 15; i++)
		{
			m_btLED[i].SetColor((wStatus & (wBit << i)) ? RGB(255, 32, 32) : RGB(64, 0, 0));
		}
		strV.Format(_T("%10.4f"), m_pSelectedMotor->GetPosition());
		m_stcPos.SetWindowTextW(strV);
		strV.Format(_T("%10.4f"), m_pSelectedMotor->GetSpeed());
		m_stcSpeed.SetWindowTextW(strV);

	}
	for (int i = 0;i < m_listMotors.GetItemCount();i++)
	{
		MMotor* pM;
		pM = (MMotor*)(m_listMotors.GetItemData(i));
		CString strText;
		strText=m_listMotors.GetItemText(i, 0);
		if (pM->GetStateName().Compare(strText)!=0)
		{
			m_listMotors.SetItemText(i, 0, pM->GetStateName());
		}
		LVITEM item;
		item.iItem = i;
		item.mask = LVIF_IMAGE;
		m_listMotors.GetItem(&item);
		int iState = pM->isIDLE() ? 0:
			(pM->isALM()?2:
			(pM->isHoming()?3:1));
		if (m_listMotors.GetItem(&item))
		{
			if (iState != item.iImage)
			{
				item.iImage = iState;
				m_listMotors.SetItem(&item);
			}
		}
	}
}

void DlgConfigMotor::OnBnClickedBtnStop()
{
	if (m_pSelectedMotor != NULL)
	{
		m_pSelectedMotor->Stop();
	}
}

void DlgConfigMotor::OnBnClickedBtnHome()
{
	if (m_pSelectedMotor != NULL)
	{
		m_pSelectedMotor->Home();
	}
}
void DlgConfigMotor::OnLvnItemchangedListMotors(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState & LVIS_FOCUSED)
	{
		CString strV;
		OnSelectMotor((MMotor *)(m_listMotors.GetItemData(pNMLV->iItem)));
	}
	*pResult = 0;
}
void DlgConfigMotor::OnSelectMotor(MMotor *pM)
{
	m_pSelectedMotor = pM;
	PitchSelectChange(m_pSelectedMotor->m_intPitchSelect);
	m_editP1.SetValueExchange(m_pSelectedMotor->m_P1);
	m_editP2.SetValueExchange(m_pSelectedMotor->m_P2);
	m_editHiSpeed.SetValueExchange(m_pSelectedMotor->m_HiSpeed);
	m_editHiAccTime.SetValueExchange(m_pSelectedMotor->m_HiAccTime);
	m_editHiDesTime.SetValueExchange(m_pSelectedMotor->m_HiDesTime);
	m_editLoSpeed.SetValueExchange(m_pSelectedMotor->m_LoSpeed);
	m_editLoAccTime.SetValueExchange(m_pSelectedMotor->m_LoAccTime);
	m_editLoDesTime.SetValueExchange(m_pSelectedMotor->m_LoDesTime);

}
BOOL DlgConfigMotor::OnInitDialog()
{
	CDialog::OnInitDialog();
	RECT rect;
	m_listMotors.GetClientRect(&rect);
	int Width = rect.right - rect.left;
	int iconsize;
	iconsize=GetIconSize();
	m_pImageList->Create(iconsize, iconsize, ILC_COLOR8, 2, 2);
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GOFF));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_GON));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_ALARM));
	m_pImageList->Add(theApp.LoadIcon(IDI_ICON_HOME));
	m_pImageList->SetBkColor(CLR_NONE);
//	m_listMotors.SubclassDlgItem(IDC_LIST_MOTORS, this);
	m_listMotors.SetImageList(m_pImageList, LVSIL_SMALL);
	m_listMotors.InsertColumn(0, _T("狀態"), LVCFMT_LEFT, Width*0.2);
	m_listMotors.InsertColumn(1, _T("馬達編號"), LVCFMT_LEFT, Width*0.2);
	m_listMotors.InsertColumn(2, _T("名稱"), LVCFMT_LEFT, Width*0.5);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX 屬性頁應傳回 FALSE
}
void DlgConfigMotor::SetMachine(MachineBase *pM)
{
	TabPageAutoma::SetMachine(pM);

}

void DlgConfigMotor::MachineMessage(MMessage *pMsg)
{
	switch (pMsg->MsgType)
	{
	case MMessage::MESSAGETYPE::MachineComplete:
		m_listMotors.DeleteAllItems();
		if (m_pMachine != NULL)
		{
			for (int i = 0; i < m_pMachine->GetMotorCount(); i++)
			{
				MMotor *pM = m_pMachine->GetMotor(i);

				m_listMotors.InsertItem(i, _T("Status"), 0);
				m_listMotors.SetItemData(i, (DWORD_PTR)pM);
				m_listMotors.SetItemText(i, 1, pM->m_ID);
				m_listMotors.SetItemText(i, 2, pM->m_strName);
			}

		}
		if (m_listMotors.GetItemCount() > 0)
		{
			m_listMotors.SelectItem(0, true);
			OnSelectMotor((MMotor*)m_listMotors.GetItemData(0));
		}
		break;
	}
}
void DlgConfigMotor::OnBnClickedBtnTest()
{
	if (m_pSelectedMotor != NULL)
	{
		m_pSelectedMotor->Repeat();
	}
}
void DlgConfigMotor::PitchSelectChange(int index)
{
	for (int i = 0;i < 6;i++)
	{
		if (i == index)
		{
			m_btnPitch[i].SetBackColor(RGB(255, 128, 128));
			m_btnPitch[i].SetColor(RGB(255, 128, 128));
		}else {
			m_btnPitch[i].SetBackColor(RGB(128, 64, 64));
			m_btnPitch[i].SetColor(RGB(128, 64, 64));
		}
	}
	if (m_pSelectedMotor != NULL)
	{
		m_pSelectedMotor->m_intPitchSelect = index;
		
	}
}
void DlgConfigMotor::OnBnClickedBtnPitch0()
{
	PitchSelectChange(0);
}
void DlgConfigMotor::OnBnClickedBtnPitch1()
{
	PitchSelectChange(1);
}
void DlgConfigMotor::OnBnClickedBtnPitch2()
{
	PitchSelectChange(2);
}
void DlgConfigMotor::OnBnClickedBtnPitch3()
{
	PitchSelectChange(3);
}
void DlgConfigMotor::OnBnClickedBtnPitch4()
{
	PitchSelectChange(4);
}
void DlgConfigMotor::OnBnClickedBtnPitch5()
{
	PitchSelectChange(5);
}

void DlgConfigMotor::OnBnClickedBtnRmovep()
{
	PitchMove(true);
}
void DlgConfigMotor::OnBnClickedBtnRmovem()
{
	PitchMove(false);
}
void DlgConfigMotor::PitchMove(bool bPMove)
{
	double dblPitch;
	if (m_pSelectedMotor != NULL)
	{
		switch (m_pSelectedMotor->m_intPitchSelect)
		{
		case 0:
			{
				if (bPMove)
				{
					m_pSelectedMotor->RPulse(1);
				}
				else
				{
					m_pSelectedMotor->RPulse(-1);
				}
			}
			break;
		case 1: //0.001
			{
				if (bPMove)
				{
					m_pSelectedMotor->RMove(0.001);
				}
				else
				{
					m_pSelectedMotor->RMove(-0.001);
				}
			}
			break;
		case 2: //0.01
			{
				if (bPMove)
				{
					m_pSelectedMotor->RMove(0.01);
				}
				else
				{
					m_pSelectedMotor->RMove(-0.01);
				}
			}
			break;
		case 3: //0.1
			{
				if (bPMove)
				{
					m_pSelectedMotor->RMove(0.1);
				}
				else
				{
					m_pSelectedMotor->RMove(-0.1);
				}
			}
			break;
		case 4: //1
			{
				if (bPMove)
				{
					m_pSelectedMotor->RMove(1);
				}
				else
				{
					m_pSelectedMotor->RMove(-1);
				}
			}
			break;
		case 5: //10
			{
				if (bPMove)
				{
					m_pSelectedMotor->RMove(10);
				}
				else
				{
					m_pSelectedMotor->RMove(-10);
				}
			}
			break;
		}
	}
}


void DlgConfigMotor::OnBnClickedBtnSave()
{
	if (m_pSelectedMotor != NULL)
	{
		m_pMachine->OpenMachineMDB();
		m_pMachine->SaveMachineData(m_pSelectedMotor, false);
		m_pMachine->CloseMachineMDB();
	}
}


void DlgConfigMotor::OnBnClickedBtnP1()
{
	if (m_pSelectedMotor != NULL)
	{
		m_pSelectedMotor->AMove(m_pSelectedMotor->m_P1);
	}
}


void DlgConfigMotor::OnBnClickedBtnP2()
{
	if (m_pSelectedMotor != NULL)
	{
		m_pSelectedMotor->AMove(m_pSelectedMotor->m_P2);
	}
}

void DlgConfigMotor::OnBnClickedBtnConfig()
{
	if (m_pSelectedMotor != NULL)
	{
		DlgMotorConfig *pDlg = new DlgMotorConfig(this,m_pSelectedMotor);
		pDlg->DoModal();
		m_pSelectedMotor->Init();
		delete pDlg;
		
	}
}
