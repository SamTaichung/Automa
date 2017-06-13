#include "stdafx.h"
#include "Machine.h"
using namespace AutoLib;
Machine::Machine(CWnd *pWnd):
	MachineBase(pWnd)
{
	m_ErrorCodeBase = 0;
	m_InitStep=InitStep::isCardOpen;

	m_pPilot =new MPilot(this, _T("PL01"), _T("Pilot light"));
	m_pConveyor = new MConveyor(this, _T("CV01"), _T("Conveyor"));
	m_pPickPlug = new MPickPlug(this, _T("PP01"), _T("PickPlug"),m_pConveyor);
//	m_pMotionLib = new MMotionLib4XMA(this, _T("ML01"), _T("4XMA Motion Lib"));
	m_pMotionLib = new MMotionLibVirtual(this, _T("ML02"), _T("Virtual Motion Lib"), 14);
	
	m_pIOLib = new MIOLib(this, _T("IL01"), _T("Virtual IO Lib"));
	m_pPCIMaster = new MCardPCIMaster(this, _T("PM01"), _T("PCI Master Card"));

	CreateMotor();
	CreateValve();
	CreateTimer();
	CreateIO();
}

Machine::~Machine()
{
	map<CString, ComponentData*>::iterator itComp;
	for (itComp = m_mapComponents.begin(); itComp != m_mapComponents.end(); itComp++)
	{
		delete (*itComp).second;
	}
	m_mapComponents.clear();
}
bool Machine::LoadSystemData()
{
	CIniFile iniFile;
	CreateDirectory(GetAppPath() + _T("\\System"), NULL);
	iniFile.SetPath(GetAppPath() + _T("\\System\\"));
	iniFile.SetName(_T("System"));
	iniFile.GetItemString(_T("Component"), _T("MDB"), m_strMachineMDB);
	if (m_strComponentMDB.GetLength() == 0)
	{
		m_strComponentMDB = _T("Component.mdb");
		iniFile.WriteItemString(_T("Component"), _T("MDB"), m_strComponentMDB);
	}
	return MachineBase::LoadSystemData();
}
bool Machine::OpenComponentMDB()
{
	CADORecordset rsTmp;
	CString strSQL, strTmp, strC, strE, strItemIndex;
	CString strDataPath = GetAppPath() + _T("\\System\\") + m_strComponentMDB;
	CreateDirectory(GetAppPath() + _T("\\System"), NULL);
	//-----------------------------DataPath-------------------------------------------------------------
	m_cnnComponents.m_ConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + strDataPath;
	CFile file;
	if (!file.Open(strDataPath, CFile::OpenFlags::modeRead))
	{
		m_cnnComponents.CreateMDB(strDataPath);
	}
	else
	{
		file.Close();
	}
	return m_cnnComponents.Open();
};
bool Machine::CloseComponentMDB()
{
	m_cnnComponents.Close();
	return true;
}
bool Machine::DelComponent(ComponentData* pComponent)
{
	CString	strSQL;
	map<CString, ComponentData*>::iterator itComp;
	itComp = m_mapComponents.find(pComponent->m_strID);
	if (itComp != m_mapComponents.end())
	{
		m_mapComponents.erase(itComp);
		OpenComponentMDB();
		strSQL = _T("Delete From ComponentLib Where ID='") + pComponent->m_strID + _T("'");
		m_cnnComponents.ExecuteSQL(strSQL);
		CloseComponentMDB();
	}
	return true;
}
bool Machine::AddComponent(ComponentData* pComponent)
{
	bool bRet = false;
	CADORecordset rsTmp;
	CString	strSQL;
	OpenComponentMDB();
	strSQL = _T("Select * From ComponentLib Where ID='") + pComponent->m_strID + _T("'");
	if (rsTmp.Open(strSQL, &m_cnnComponents))
	{
		m_cnnComponents.BeginTrans();
		if (rsTmp.isEOF())
		{
			rsTmp.AddNew();
		}
		rsTmp.SetValue(_T("ID"), pComponent->m_strID);
		rsTmp.SetValue(_T("Name"), pComponent->m_strName);
		rsTmp.SetValue(_T("High"), pComponent->m_dblHight);
		rsTmp.Update();
		m_cnnComponents.CommitTrans();
		bRet = true;
	}
	CloseComponentMDB();
	m_mapComponents.insert(map<CString, ComponentData*>::value_type(pComponent->m_strID, pComponent));
	return bRet;
}
bool Machine::UpdateComponent(CString strOldID,ComponentData* pComponent)
{
	bool bRet = false;
	CADORecordset rsTmp;
	CString	strSQL;
	OpenComponentMDB();
	strSQL = _T("Select * From ComponentLib Where ID='") + strOldID + _T("'");
	if (rsTmp.Open(strSQL, &m_cnnComponents))
	{
		m_cnnComponents.BeginTrans();
		if (rsTmp.isEOF())
		{
			rsTmp.AddNew();
		}
		rsTmp.SetValue(_T("ID"), pComponent->m_strID);
		rsTmp.SetValue(_T("Name"), pComponent->m_strName);
		rsTmp.SetValue(_T("High"), pComponent->m_dblHight);
		rsTmp.Update();
		m_cnnComponents.CommitTrans();
		bRet = true;
	}
	CloseComponentMDB();
	m_mapComponents.erase(strOldID);
	m_mapComponents.insert(map<CString, ComponentData*>::value_type(pComponent->m_strID, pComponent));
	return bRet;
}
ComponentData* Machine::GetComponent(CString strID)
{
	map<CString, ComponentData*>::iterator itComp;
	itComp = m_mapComponents.find(strID);
	if (itComp != m_mapComponents.end())
	{
		return (*itComp).second;
	}
	return NULL;
}		


bool Machine::LoadComponentDatas()
{
	CADORecordset rsTmp;
	CString	strSQL,strID,strName;
	map<CString, ComponentData*>::iterator itComp;
	for (itComp=m_mapComponents.begin();itComp!=m_mapComponents.end();itComp++)
	{
		delete (*itComp).second;
	}
	m_mapComponents.clear();
	OpenComponentMDB();
	if (m_cnnComponents.GetTableIndex(_T("ComponentLib"))<0)
	{
		strSQL = _T("Create Table ComponentLib(ID nChar(20),");
		strSQL += _T("Name nChar(20),High float default 0,");
		strSQL += _T("Constraint PKComponentLib Primary Key(ID))");
		m_cnnComponents.BeginTrans();
		m_cnnComponents.ExecuteSQL(strSQL);
		m_cnnComponents.CommitTrans();
	}
	strSQL=_T("Select * From ComponentLib ");
	if (rsTmp.Open(strSQL, &m_cnnComponents))
	{
		while (!rsTmp.isEOF())
		{
			ComponentData* pCD;
			pCD = new ComponentData();
			rsTmp.GetValue(_T("ID"), pCD->m_strID);
			pCD->m_strID.Trim();
			rsTmp.GetValue(_T("Name"), pCD->m_strName);
			pCD->m_strName.Trim();
			rsTmp.GetValue(_T("High"), pCD->m_dblHight);
			m_mapComponents.insert(map<CString, ComponentData*>::value_type(pCD->m_strID,pCD));
			rsTmp.MoveNext();
		}
	}
	return CloseComponentMDB();
}
void Machine::CreateMotor()
{
	m_pPickPlug->m_pXMotor = AddMotor(new MMotor(m_pPickPlug, _T("M01"), _T("XMotor"),m_pMotionLib, _T("mm")));
	m_pPickPlug->m_pYMotor = AddMotor(new MMotor(m_pPickPlug, _T("M02"), _T("YMotor"), m_pMotionLib, _T("mm")));
	MPPArm *pPA;
	CString strID, strName;
	int count;
	count = sizeof(m_pPickPlug->m_pArm) / sizeof(void*);
	for (int i = 0; i < count; i++)
	{
		strID.Format(_T("M%.2d"), i + 3);
		strName.Format(_T("Z%.2dMotor"), i + 1);
		pPA = m_pPickPlug->m_pArm[i];
		pPA->m_pZMotor = AddMotor(new MMotor(pPA, strID, strName, m_pMotionLib, _T("mm")));
	}
	for (int i = 0; i < count; i++)
	{
		strID.Format(_T("M%.2d"), i +count+ 3);
		strName.Format(_T("R%.2dMotor"), i + 1);
		pPA = m_pPickPlug->m_pArm[i];
		pPA->m_pRMotor = AddMotor(new MMotor(pPA, strID, strName, m_pMotionLib, _T("°")));

	}
}
void Machine::CreateValve()
{
	CString strID, strName;
	int count;
	m_pConveyor->m_pInStopper = AddValve(new MValveVirtual(m_pConveyor, _T("V001"), _T("Conveyor Inlet Stopper")));
	m_pConveyor->m_pPlugStopper = AddValve(new MValveVirtual(m_pConveyor, _T("V002"), _T("Conveyor Plug Stopper")));
	m_pConveyor->m_pLockCylinder = AddValve(new MValveVirtual(m_pConveyor, _T("V003"), _T("Conveyor Lock Cylinder")));
	MPPArm *pArm;
	count = sizeof(m_pPickPlug->m_pArm) / sizeof(void*);
	for (int i = 0; i < count; i++)
	{
		pArm = m_pPickPlug->m_pArm[i];
		strID.Format(_T("V%.3d"), i + 4);
		strName.Format(_T("Pick Plug Arm%d"), i + 1);
		pArm->m_pValve = AddValve(new MValveVirtual(pArm, strID, strName));
	}
}
void Machine::CreateTimer()
{
	MFlash *pF;
	pF = m_pPilot->m_pFHRed;
	pF->m_pTMOn = AddTimer(new MTimer(pF, _T("T001"), _T("Pilot Red Light On Time"), 300));
	pF->m_pTMOff = AddTimer(new MTimer(pF, _T("T002"), _T("Pilot Red Light Off Time"), 300));

	pF = m_pPilot->m_pFHYellow;
	pF->m_pTMOn = AddTimer(new MTimer(pF, _T("T003"), _T("Pilot Yellow Light On Time"), 300));
	pF->m_pTMOff = AddTimer(new MTimer(pF, _T("T004"), _T("Pilot Yellow Light Off Time"), 300));

	pF = m_pPilot->m_pFHGreen;
	pF->m_pTMOn = AddTimer(new MTimer(pF, _T("T005"), _T("Pilot Green Light On Time"), 300));
	pF->m_pTMOff = AddTimer(new MTimer(pF, _T("T006"), _T("Pilot Green Light Off Time"), 300));

	MPulseIO *pP;
	pP = m_pPilot->m_pPSBuzz;
	pP->m_pOnTimer = AddTimer(new MTimer(pP, _T("T007"), _T("Buzz on Time"), 3000));

	
}
void Machine::CreateIO()
{
	int count;
	CString strID, strName;
	MValveVirtual *pV;
	//------------------------------輸入接點虛擬2點式SR--------------------------
	pV = ValveTranslation(m_pConveyor->m_pInStopper);
	pV->m_pIOOpenSR = AddIO(new MSRX2(pV, _T("Z0001"), _T("In Stopper Sensor")));
	pV = ValveTranslation(m_pConveyor->m_pPlugStopper);
	pV->m_pIOOpenSR = AddIO(new MSRX2(pV, _T("Z0002"), _T("Plug Stopper Sensor")));
	pV = ValveTranslation(m_pConveyor->m_pLockCylinder);
	pV->m_pIOOpenSR = AddIO(new MSRX4(pV, _T("Z0003"), _T("Lock Cylinder Sensor")));
	//------------------------------輸入接點-------------------------------------
	m_pIO_EMG = AddIO(new MIO(this, _T("X0100"), _T("Emergency Stop"),m_pIOLib, false, true));
	m_pIO_SysCDA = AddIO(new MIO(this, _T("X0101"), _T("System CDA SR"), m_pIOLib, false, true));
	m_pConveyor->m_pIOInPosSR = AddIO(new MIO(m_pConveyor, _T("X0102"), _T("PCB In Position SR"), m_pIOLib,false, true));
	m_pConveyor->m_pIOPlugPosSR = AddIO(new MIO(m_pConveyor, _T("X0103"), _T("PCB Plug Position SR"), m_pIOLib,false, true));
	m_pConveyor->m_pIOOutPosSR = AddIO(new MIO(m_pConveyor, _T("X0104"), _T("PCB Out Position SR"), m_pIOLib,false, true));
	MSRX2 *pSRX2;
	pV = ValveTranslation(m_pConveyor->m_pInStopper);
	pSRX2 = (MSRX2 *)(pV->m_pIOOpenSR);
	pSRX2->m_pIOSR1 = AddIO(new MIO(pSRX2, _T("X0106"), _T("In Stopper SR1"), m_pIOLib,false, true));
	pSRX2->m_pIOSR2 = AddIO(new MIO(pSRX2, _T("X0107"), _T("In Stopper SR2"), m_pIOLib,false, true));
	pV = ValveTranslation(m_pConveyor->m_pPlugStopper);
	pSRX2 = (MSRX2 *)(pV->m_pIOOpenSR);
	pSRX2->m_pIOSR1 = AddIO(new MIO(pSRX2, _T("X0108"), _T("Plug Stopper SR1"), m_pIOLib,false, true));
	pSRX2->m_pIOSR2 = AddIO(new MIO(pSRX2, _T("X0109"), _T("Plug Stopper SR2"), m_pIOLib,false, true));
	MSRX4 *pSRX4;
	pV = ValveTranslation(m_pConveyor->m_pLockCylinder);
	pSRX4 = (MSRX4 *)(pV->m_pIOOpenSR);
	pSRX4->m_pIOSR1 = AddIO(new MIO(pSRX2, _T("X0110"), _T("Plug lock SR1"), m_pIOLib,false, true));
	pSRX4->m_pIOSR2 = AddIO(new MIO(pSRX2, _T("X0111"), _T("Plug lock SR2"), m_pIOLib,false, true));
	pSRX4->m_pIOSR3 = AddIO(new MIO(pSRX2, _T("X0112"), _T("Plug lock SR3"), m_pIOLib,false, true));
	pSRX4->m_pIOSR4 = AddIO(new MIO(pSRX2, _T("X0113"), _T("Plug lock SR4"), m_pIOLib,false, true));
	//X0114-X0119
	MPPArm *pArm;
	count = sizeof(m_pPickPlug->m_pArm) / sizeof(void*);
	for (int i = 0; i < count; i++)
	{
		pArm = m_pPickPlug->m_pArm[i];
		pV = ValveTranslation(pArm->m_pValve);
		strID.Format(_T("X01%.2d"), i +14);
		strName.Format(_T("Vac-SR of Pick-Plug Arm%d"), i + 1);
		pV->m_pIOOpenSR = AddIO(new MIO(pV, strID,strName, m_pIOLib,false, true));
	}

	MFeeder *pFD;
	count = sizeof(m_pPickPlug->m_pFeeder) / sizeof(void*);
	//供料器X0200-X0231
	for (int i = 0; i < count; i++)
	{
		pFD = m_pPickPlug->m_pFeeder[i];
		strID.Format(_T("X02%.2d"), i * 4);
		strName.Format(_T("Feeder%dOK"), i + 1);
		pFD->m_pIO_OK = AddIO(new MIO(this, strID, strName, m_pIOLib,false, true));
		strID.Format(_T("X02%.2d"), (i * 4) + 1);
		strName.Format(_T("Feeder%dNG"), i + 1);
		pFD->m_pIO_NG = AddIO(new MIO(this, strID, strName, m_pIOLib,false, true));
		strID.Format(_T("X02%.2d"), (i * 4) + 2);
		strName.Format(_T("Feeder%dEmpty"), i + 1);
		pFD->m_pIO_Empty = AddIO(new MIO(this, strID, strName, m_pIOLib,false, true));
		strID.Format(_T("X02%.2d"), (i * 4) + 3);
		strName.Format(_T("Feeder%dAlarm"), i + 1);
		pFD->m_pIO_Alarm = AddIO(new MIO(this, strID, strName, m_pIOLib,false, true));
	}
	//------------------------------輸出接點-------------------------------------
	MPulseIO *pP;
	pP = m_pPilot->m_pPSBuzz;
	pP->m_pIO = AddIO(new MIO(pP, _T("Y0100"), _T("Pilot buzz"), m_pIOLib,true, true));
	MFlash *pF;
	pF = m_pPilot->m_pFHRed;
	pF->m_pIO = AddIO(new MIO(pF, _T("Y0101"), _T("Pilot Red Light"), m_pIOLib,true, true));
	pF = m_pPilot->m_pFHYellow;
	pF->m_pIO = AddIO(new MIO(pF, _T("Y0102"), _T("Pilot Yellow Light"), m_pIOLib,true, true));
	pF = m_pPilot->m_pFHGreen;
	pF->m_pIO = AddIO(new MIO(pF, _T("Y0103"), _T("Pilot Green Light"), m_pIOLib,true, true));
	m_pConveyor->m_pIOMotor = AddIO(new MIO(m_pConveyor, _T("Y0105"), _T("Conveyor motor"), m_pIOLib,true, true));

	pV = ValveTranslation(m_pConveyor->m_pInStopper);
	pV->m_pIOOpen = AddIO(new MIO(pV, _T("Y0106"), _T("In Stopper Up Coil"), m_pIOLib,true, true));
	pV->m_pIOClose = AddIO(new MIO(pV, _T("Y0107"), _T("In Stopper Down Coil"), m_pIOLib,true, true));

	pV = ValveTranslation(m_pConveyor->m_pPlugStopper);
	pV->m_pIOOpen = AddIO(new MIO(pV, _T("Y0108"), _T("Plug Stopper Up Coil"), m_pIOLib,true, true));
	pV->m_pIOClose = AddIO(new MIO(pV, _T("Y0109"), _T("Plug Stopper Down Coil"), m_pIOLib,true, true));

	pV = ValveTranslation(m_pConveyor->m_pLockCylinder);
	pV->m_pIOOpen = AddIO(new MIO(pV, _T("Y0110"), _T("PCB Lock Cylinder Up Coil"), m_pIOLib,true, true));

	//Y0111-X0122
	count = sizeof(m_pPickPlug->m_pArm) / sizeof(void*);
	for (int i = 0; i < count; i++)
	{
		pArm = m_pPickPlug->m_pArm[i];
		pV = ValveTranslation(pArm->m_pValve);
		strID.Format(_T("Y01%.2d"), i * 2 + 11);
		strName.Format(_T("Valve-Open of Pick-Plug Arm%d"), i + 1);
		pV->m_pIOOpen = AddIO(new MIO(pV, strID, strName, m_pIOLib,true, true));
		strID.Format(_T("Y01%.2d"), i * 2 + 12);
		strName.Format(_T("Valve-Close of Pick-Plug Arm%d"), i + 1);
		pV->m_pIOClose = AddIO(new MIO(pV, strID, strName, m_pIOLib,true, true));
	}
}
void Machine::Cycle(const double dblTime)
{
	MMessage *pMsg;
	MachineBase::Cycle(dblTime);
	switch (m_State)
	{
		case STATE::INIT:
		{
			switch(m_InitStep)
			{
				case InitStep::isCardOpen:
				{
					Sleep(500);
					SetLog(true);
					pMsg = new MMessage();
					pMsg->strMessage = _T("Open Cards...");
					pMsg->MsgType = MMessage::MESSAGETYPE::ShowMessage;
					ShowMessage(pMsg);
					MCardPCIMaster *pCard = (MCardPCIMaster*)m_pPCIMaster;
					pCard->Open();
					m_InitStep = InitStep::isLoadSystemData;
				}
				break;
				case InitStep::isLoadSystemData:
				{
					Sleep(500);
					SetLog(true);
					pMsg = new MMessage();
					pMsg->strMessage = _T("Load System Data...");
					pMsg->MsgType = MMessage::MESSAGETYPE::ShowMessage;
					ShowMessage(pMsg);
					LoadSystemData();
					m_InitStep = InitStep::isLoadMachineData;
				}
				break;

				case InitStep::isLoadMachineData:
				{
					pMsg = new MMessage();
					pMsg->strMessage = _T("Load Machine Data...");
					pMsg->MsgType = MMessage::MESSAGETYPE::ShowMessage;
					ShowMessage(pMsg);
					OpenMachineMDB();
					LoadMachineData(this,true);
					CloseMachineMDB();
					m_InitStep = InitStep::isLoadRecipeData;
				}
				break;
				case InitStep::isLoadRecipeData:
				{
					pMsg = new MMessage();
					pMsg->strMessage = _T("Load Recipe Data...");
					pMsg->MsgType = MMessage::MESSAGETYPE::ShowMessage;
					ShowMessage(pMsg);
					LoadComponentDatas();
					OpenRecipeMDB();
					LoadRecipeData(this,true);
					CloseRecipeMDB();
					m_InitStep = InitStep::isMachineComplete;
				}
				break;
				case InitStep::isMachineComplete:
				{
					MachineBase::Init();
					pMsg = new MMessage();
					pMsg->strMessage = _T("Load Data Complete");
					pMsg->MsgType = MMessage::MESSAGETYPE::MachineComplete;
					ShowMessage(pMsg);
					m_InitStep = InitStep::isInitMachine;
				}
				break;
				case InitStep::isInitMachine:
				{
					pMsg = new MMessage();
					pMsg->MsgType = MMessage::MESSAGETYPE::LogToFile;
					pMsg->strMessage = _T("Machine initial complete!");
					ShowMessage(pMsg);
					//MError *pError;
					//pError = new MError(this, _T("Test"), 5, _T("Memo"));
					//ErrorHappen(pError);
					m_State = STATE::IDLE;
				}
				break;
			};
			break;
		}
	}
}
