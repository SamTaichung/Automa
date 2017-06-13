#include "stdafx.h"
#include "AdoTool.h"
#include "MachineBase.h"
using namespace AutoLib;
using ATL::CAtlTransactionManager;
MachineBase::MachineBase(CWnd *pWnd):
	MUnit(NULL,_T(""),_T("")),m_pMainFrame(pWnd),m_pPilot(NULL)
{

	CreateDirectory(GetAppPath() + _T("\\System"), NULL);
	m_iniLang.SetPath(GetAppPath() + _T("\\System\\"));
	m_iniLang.SetName(_T("ChineseTraditional"));
	m_pStringProvider = new CStringProvider(&m_iniLang);
	m_bShutDown = true;
	m_bErrorHappen = false;
	m_bMachineStop = false;
	m_dblScanTime = 0;
	m_bInitComplete = false;
	m_bHomeComplete = false;
	m_pThread = NULL;
	::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(MachineBase::DoThread), this, NULL, &m_pThread);
}
void MachineBase::ShowMessage(MMessage *pMessage)
{
	if (pMessage->MsgType == MMessage::MESSAGETYPE::LogToFile)
	{
		FILE *m_fp;
		m_fp = NULL;
		m_fp = _wfopen(GetAppPath() + _T("\\System\\Log.txt"), _T("a,ccs=UTF-8"));
		if (m_fp)
			// m_fp Contains the pointer
			// to the log file to log the information
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			fwprintf(m_fp,_T("[%d/%d/%d - %d:%d:%d:%d] "),
				st.wMonth, st.wDay, st.wYear, st.wHour,
				st.wMinute, st.wSecond, st.wMilliseconds);
			fwprintf(m_fp, pMessage->strMessage);
			fwprintf(m_fp, _T("\n"));

			fflush(m_fp);
			fclose(m_fp);
		}
		delete pMessage;
	}
	else {
		::PostMessage(m_pMainFrame->m_hWnd, WM_MACHINEMESSAGE, (WPARAM)pMessage, 0);
	}
}

MachineBase::~MachineBase()
{
	delete m_pStringProvider;
}

UINT MachineBase::DoThread(void * pM)
{
	int intLC = 0;
	double dblTotalTime, dblTime;
	MachineBase * pMachine = (MachineBase *)pM;

	LARGE_INTEGER m_nStartTime, m_nStopTime, m_nFrequency;
	QueryPerformanceFrequency(&m_nFrequency);
	QueryPerformanceCounter(&m_nStartTime);

	dblTotalTime = 0;
	//不同的Thread要重新CoInitialize
	HRESULT	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		return -1;
	}
	pMachine->m_bShutDown = false;
	while (!pMachine->m_bShutDown)
	{
		QueryPerformanceCounter(&m_nStopTime);
		dblTime = (((double)(m_nStopTime.QuadPart - m_nStartTime.QuadPart)) / m_nFrequency.QuadPart)*1000;
		m_nStartTime = m_nStopTime;
		dblTotalTime += dblTime;
		pMachine->Cycle(dblTime);
		intLC++;
		if (intLC==100) {
			::Sleep(1);
			pMachine->m_dblScanTime = dblTotalTime / intLC;
			intLC = 0;
			dblTotalTime = 0;
		};
	}
	CoUninitialize();
	return(0);
}
void MachineBase::SetMachineState(MPilot::PilotState state)
{
	if (m_pPilot != NULL)
	{
		m_pPilot->SetState(state);
	}
}
void MachineBase::Cycle(const double dblTime)
{
	MMessage* pMsg;
	MError*	pError;
	MSolution* pSolution;
	std::vector<MError*>::iterator itError;
	if (m_isEStop) { //急停中
		if (!isEStop()) //急停解除
		{
			m_bMachineStop = false;
			EStop(false);
			SetMachineState(MPilot::PilotState::IDLE);
		}
	}else{
		if (isEStop()) //急停發生
		{
			EStop(true);
			SetMachineState(MPilot::PilotState::EMERGENCY_STOP);
		}else{
			if (m_bMachineStop) //UI或不同Thread呼叫MachineStop
			{
				Stop();
				m_bMachineStop = false;
			}else{
				if (m_bErrorHappen) //異常中
				{
					itError = m_Errors.begin();
					pError = *itError;
					if (pError->m_pSelectedSolution != NULL) //錯誤處理方法已設定
					{
						//-------------------清除錯誤------------------
						m_Errors.erase(itError);
						pSolution = pError->m_pSelectedSolution;
						//-------------------執行選取的錯誤處理---------------
						pSolution->DoProcess();
						//-------------------解除錯誤狀況--------------------
						delete pError;
						m_bErrorHappen = false;
						switch (m_State)
						{
						case MBase::STATE::ACTION:
							SetMachineState(MPilot::PilotState::AUTOMATIC_RUN);
							break;
						case MBase::STATE::HOME:
							SetMachineState(MPilot::PilotState::HOMING);
							break;
						default:
							SetMachineState(MPilot::PilotState::IDLE);
							break;
						}
					}else{
						if (pError->m_isStop) //------停機
						{
							for (itError = m_Errors.begin();itError != m_Errors.end();itError++)
							{
								pError = *itError;
								delete pError;
							}
							m_Errors.clear();
							m_bErrorHappen = false;
							m_bMachineStop = false;
							Stop();
							SetMachineState(MPilot::PilotState::IDLE);
						}
					}
				}
				else {
					if (!m_Errors.empty()) //收到異常
					{
						switch (m_State)
						{
						case MBase::STATE::ACTION:
							SetMachineState(MPilot::PilotState::ERROR_ON_AUTO);
							break;
						case MBase::STATE::HOME:
							SetMachineState(MPilot::PilotState::ERROR_ON_HOMING);
							break;
						default:
							SetMachineState(MPilot::PilotState::ERROR_ON_MANUAL);
							break;
						}
						m_bErrorHappen = true;
						itError = m_Errors.begin();
						pError = *itError;
						pMsg = new MMessage; //錯誤
						pMsg->MsgType = MMessage::MESSAGETYPE::ShowError;
						pMsg->m_pPara = (LPVOID)pError;
						PostMessage(m_pMainFrame->m_hWnd, WM_MACHINEMESSAGE, (WPARAM)pMsg, 0);
					}else{ //正常狀態下
						switch (m_State)
						{
						case MBase::STATE::ACTION:
							SetMachineState(MPilot::PilotState::AUTOMATIC_RUN);
							break;
						case MBase::STATE::IDLE:
							SetMachineState(MPilot::PilotState::IDLE);
							break;
						case MBase::STATE::HOME:
							SetMachineState(MPilot::PilotState::HOMING);
							break;
						case MBase::STATE::PAUSE:
							SetMachineState(MPilot::PilotState::PAUSE_STOP);
							break;
						}
					}
				}
			}
		}
	}
	if (m_isInterLock) //鎖定中
	{
		if (!isInterLock()) //解鎖
		{
			InterLock(false);
		}
	}else { //未鎖定
		if (isInterLock()) //鎖定
		{
			InterLock(true);
		}
	}
	//------------------------------------------------------------
	MUnit::Cycle(dblTime);
}
void MachineBase::ErrorHappen(MError *pError)
{
	CADORecordset rsTmp;
	CString	strSQL;
	CTime t = CTime::GetCurrentTime();
	if (isEStop()) //如果急停中的再發錯誤，不再引發
	{
		delete pError;
		return;
	}
	pError->tHappen = t;		//記錄異常時間
	MUnit::ErrorHappen(pError); //進人異常狀態
	if (pError->m_intErrorCode != 0)
	{
		//------------記錄錯誤到資料庫中-------------
		OpenProductionMDB();
		strSQL.Format(_T("Select *	From Errors Where ID=%d"), pError->m_intErrorCode);
		if (rsTmp.Open(strSQL, &m_cnnProduction))
		{
			CString sTmp;
			sTmp = pError->m_Happener->m_strName + _T(" ") + pError->m_strDescript;
			if (rsTmp.isEOF())
			{
				strSQL.Format(_T("Insert into Errors(ID,Name,Reason) Values(%d,'%s','%s')"),
					pError->m_intErrorCode,
					sTmp,
					pError->m_strReason);
				m_cnnProduction.BeginTrans();
				m_cnnProduction.ExecuteSQL(strSQL);
				m_cnnProduction.CommitTrans();
			}
			rsTmp.Close();
		}
		pError->tHappen = t;
		strSQL.Format(_T("Insert into ErrorHistory(ErrorCode,HappenTime,SlovedTime) Values(%d,'%s','%s')"),
			pError->m_intErrorCode, pError->tHappen.Format(_T("%Y/%m/%d %H:%M:%S")), pError->tHappen.Format(_T("%Y/%m/%d %H:%M:%S")));
		m_cnnProduction.BeginTrans();
		m_cnnProduction.ExecuteSQL(strSQL);
		m_cnnProduction.CommitTrans();
		CloseProductionMDB();
	}
	m_Errors.push_back(pError);
}
void MachineBase::SetShutDown()
{
	m_bShutDown = true;
}
double MachineBase::GetScanTime()
{
	return m_dblScanTime;
};
MMotor* MachineBase::AddMotor(MMotor* pMotor)
{
	m_Motors.Add(pMotor);
	return pMotor;
}
MValve* MachineBase::AddValve(MValve* pValve)
{
	m_Valves.Add(pValve);
	return pValve;
}
MIO* MachineBase::AddIO(MIO* pIO)
{
	m_IOs.Add(pIO);
	return pIO;
}
MTimer* MachineBase::AddTimer(MTimer* pTimer)
{
	m_Timers.Add(pTimer);
	return pTimer;
}
int MachineBase::GetMotorCount()
{
	return m_Motors.GetCount();
}
int MachineBase::GetValveCount()
{
	return m_Valves.GetCount();
}
int MachineBase::GetIOCount()
{
	return m_IOs.GetCount();
}
int MachineBase::GetTimerCount()
{
	return m_Timers.GetCount();
}
MMotor* MachineBase::GetMotor(int index)
{
	MMotor* pRet=NULL;
	if (index < m_Motors.GetCount())
	{
		pRet = m_Motors[index];
	}
	return pRet;
}
MValve* MachineBase::GetValve(int index)
{
	MValve* pRet = NULL;
	if (index < m_Valves.GetCount())
	{
		pRet = m_Valves[index];
	}
	return pRet;
}
MIO* MachineBase::GetIO(int index)
{
	MIO* pRet = NULL;
	if (index < m_IOs.GetCount())
	{
		pRet = m_IOs[index];
	}
	return pRet;
}
MTimer* MachineBase::GetTimer(int index)
{
	MTimer* pRet = NULL;
	if (index < m_Timers.GetCount())
	{
		pRet = m_Timers[index];
	}
	return pRet;
}
CString MachineBase::GetAppPath()
{
	CString strRet;
	wchar_t strPath[255];
	GetModuleFileName(NULL,strPath,255);
	strRet = strPath;
	strRet.MakeUpper();
	strRet.Replace(_T("\\DEBUG"), _T(""));
	int iFind = strRet.ReverseFind(_T('\\'));
	strRet=strRet.Left(iFind);
	return strRet;
}
bool MachineBase::LoadSystemData()
{
	CIniFile iniFile;
	CreateDirectory(GetAppPath() + _T("\\System"), NULL);
	iniFile.SetPath(GetAppPath()+_T("\\System\\"));
	iniFile.SetName(_T("System"));
	iniFile.GetItemString(_T("Machine"), _T("MDB"), m_strMachineMDB);
	if (m_strMachineMDB.GetLength() == 0)
	{
		m_strMachineMDB = _T("Machine.mdb");
		iniFile.WriteItemString(_T("Machine"), _T("MDB"), m_strMachineMDB);
	}
	iniFile.GetItemString(_T("Recipe"), _T("MDB"), m_strRecipeMDB);
	if (m_strRecipeMDB.GetLength() == 0)
	{
		m_strRecipeMDB = _T("Recipe.mdb");
		iniFile.WriteItemString(_T("Recipe"), _T("MDB"), m_strRecipeMDB);
	}
	ChangeLanguage();
	return true;
}
bool MachineBase::OpenMachineMDB()
{
	CADORecordset rsTmp;
	CString strSQL, strTmp, strC, strE, strItemIndex;
	CString strDataPath = GetAppPath() + _T("\\Machine\\") + m_strMachineMDB;
	CreateDirectory(GetAppPath() + _T("\\Machine"), NULL);
	//-----------------------------DataPath-------------------------------------------------------------
	m_cnnMachine.m_ConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + strDataPath;
	CFile file;
	if (!file.Open(strDataPath, CFile::OpenFlags::modeRead))
	{
		m_cnnMachine.CreateMDB(strDataPath);
	}
	else
	{
		file.Close();
	}

	return m_cnnMachine.Open();
};
bool MachineBase::CloseMachineMDB()
{
	m_cnnMachine.Close();
	return true;
}
bool MachineBase::OpenProductionMDB()
{
	bool bRet;
	CADORecordset rsTmp;
	CString strSQL, strTmp, strC, strE, strItemIndex;
	CString strDataPath = GetAppPath() + _T("\\Machine\\Production.mdb");
	CreateDirectory(GetAppPath() + _T("\\Machine"), NULL);
	//-----------------------------DataPath-------------------------------------------------------------
	m_cnnProduction.m_ConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + strDataPath;
	CFile file;
	if (!file.Open(strDataPath, CFile::OpenFlags::modeRead))
	{
		m_cnnProduction.CreateMDB(strDataPath);
		//---------------檢查是否需要建立表單----------------
		bRet = m_cnnProduction.Open();
		if (m_cnnProduction.GetTableIndex(_T("Errors"))<0)
		{
			strSQL = _T("Create Table Errors(ID int,Name nChar(80),");
			strSQL += _T("Reason Text,");
			strSQL += _T("Constraint PKErrors Primary Key(ID))");
			m_cnnProduction.BeginTrans();
			m_cnnProduction.ExecuteSQL(strSQL);
			m_cnnProduction.CommitTrans();
		}
		if (m_cnnProduction.GetTableIndex(_T("ErrorHistory"))<0)
		{
			strSQL = _T("Create Table ErrorHistory(ID int IDENTITY (1, 1) NOT NULL,");
			strSQL += _T("ErrorCode int,HappenTime DateTime, SlovedTime DateTime, SpentTime nChar(40),");
			strSQL += _T("Constraint FKErrorHistory Foreign Key(ErrorCode) References Errors(ID),");
			strSQL += _T("Constraint ErrorHistory Primary Key(ID))");
			m_cnnProduction.BeginTrans();
			m_cnnProduction.ExecuteSQL(strSQL);
			m_cnnProduction.CommitTrans();
		}
	}
	else
	{
		file.Close();
		bRet = m_cnnProduction.Open();
	}
	return bRet;
};
bool MachineBase::CloseProductionMDB()
{
	m_cnnProduction.Close();
	return true;
}
bool MachineBase::OpenRecipeMDB()
{
	CADORecordset rsTmp;
	CString strSQL, strTmp, strC, strE, strItemIndex;
	CString strDataPath = GetAppPath() + _T("\\Recipe\\") + m_strRecipeMDB;
	CreateDirectory(GetAppPath() + _T("\\Recipe"), NULL);
	//-----------------------------DataPath-------------------------------------------------------------
	m_cnnRecipe.m_ConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + strDataPath;
	CFile file;
	if (!file.Open(strDataPath, CFile::OpenFlags::modeRead))
	{
		m_cnnRecipe.CreateMDB(strDataPath);
	}
	else
	{
		file.Close();
	}
	return m_cnnRecipe.Open();
};
bool MachineBase::CloseRecipeMDB()
{
	m_cnnRecipe.Close();
	return true;
}
bool MachineBase::SaveMachineData(MBase *pC, bool bAllChildsSave)
{
	pC->SaveMachineData(&m_cnnMachine, bAllChildsSave);
	return true;
}
bool MachineBase::LoadMachineData(MBase *pC, bool bAllChildsSave)
{
	pC->LoadMachineData(&m_cnnMachine, bAllChildsSave);
	return true;
}
bool MachineBase::SaveRecipeData(MBase *pC, bool bAllChildsSave)
{
	pC->SaveRecipeData(&m_cnnRecipe, bAllChildsSave);
	return true;
}
bool MachineBase::LoadRecipeData(MBase *pC, bool bAllChildsSave)
{
	pC->LoadRecipeData(&m_cnnRecipe, bAllChildsSave);
	return true;
}
void MachineBase::MachineStop()
{
	m_bMachineStop = true;
}
MPilot* MachineBase::GetPilot()
{
	return m_pPilot;
}
//根據輸入的密碼參數,決定使用者身分
MBase::LOGINTYPE MachineBase::CheckPassword(CADOConnection* pCnn, CString szPWDtoCheck)
{
	m_USERTYPE = MBase::LOGINTYPE::UNDEF;
	CADORecordset	m_rsTmp;
	CString	szSQL;
	szSQL = _T("SELECT SData FROM DataSet WHERE ID='PWDdev'");
	CString	szPWDdev;
	m_rsTmp.Open(szSQL, pCnn);
	if (!m_rsTmp.isEOF())
		m_rsTmp.GetValue(_T("SData"), szPWDdev);
	m_rsTmp.Close();	// 取得 developer 的密碼

	szSQL = _T("SELECT SData FROM DataSet WHERE ID='PWDeng'");
	CString	szPWDeng;
	m_rsTmp.Open(szSQL, pCnn);
	if (!m_rsTmp.isEOF())
		m_rsTmp.GetValue(_T("SData"), szPWDeng);
	m_rsTmp.Close();	// 取得 engineer 的密碼

	szSQL = _T("SELECT SData FROM DataSet WHERE ID='PWDop'");
	CString	szPWDop;
	m_rsTmp.Open(szSQL, pCnn);
	if (!m_rsTmp.isEOF())
		m_rsTmp.GetValue(_T("SData"), szPWDop);
	m_rsTmp.Close();	// 取得 engineer 的密碼

	if (szPWDtoCheck == szPWDdev.Trim()) {
		m_USERTYPE = MBase::LOGINTYPE::MAKER;
	}
	else if (szPWDtoCheck == szPWDeng.Trim()) {
		m_USERTYPE = MBase::LOGINTYPE::ENGINEER;
	}
	else if (szPWDtoCheck == szPWDop.Trim()) {
		m_USERTYPE = MBase::LOGINTYPE::OPERATOR;
	}
	else {
		m_USERTYPE = MBase::LOGINTYPE::UNDEF;
	}

	return m_USERTYPE;
}

// 檢查輸入的新舊密碼,和使用者身份, 若正確,則將新密碼寫入資料庫,回傳true
bool MachineBase::RenewPassword(CADOConnection* pCnn, CString szOldPwd,
	CString szNewPwd, MBase::LOGINTYPE nUserType)
{
	szOldPwd = szOldPwd.Trim();
	szNewPwd = szNewPwd.Trim();
	if ("" == szOldPwd || "" == szNewPwd)
		return false;	//不允許空字串做為密碼
	MBase::LOGINTYPE nTmpType = CheckPassword(pCnn, szOldPwd);
	if (MBase::LOGINTYPE::UNDEF == nTmpType) {
		return false;
	}
	else {
		WritePassword(pCnn, szNewPwd, nUserType);
		return true;
	}

}

bool MachineBase::WritePassword(CADOConnection* pCnn, CString szNewPwd, MBase::LOGINTYPE UserType)
{
	szNewPwd = szNewPwd.Trim();
	CString	szSQL;
	switch (UserType)
	{
	case MBase::LOGINTYPE::MAKER:
		szSQL.Format(_T("%s%s%s"), _T("UPDATE DataSet SET SData='"), szNewPwd,
			_T("' WHERE ID='PWDdev'"));
		break;
	case MBase::LOGINTYPE::ENGINEER:
		szSQL.Format(_T("%s%s%s"), _T("UPDATE DataSet SET SData='"), szNewPwd,
			_T("' WHERE ID='PWDeng'"));
		break;
	case MBase::LOGINTYPE::OPERATOR:
		szSQL.Format(_T("%s%s%s"), _T("UPDATE DataSet SET SData='"), szNewPwd,
			_T("' WHERE ID='PWDop'"));
		break;
	default:
		szSQL = "Wrong LoginType!";
	}
	if ("Wrong LoginType!" == szSQL) {
		AfxMessageBox(_T("不正確的LoginType,無法進行密碼更新."));
		return false;
	}
	else {
		return (pCnn->ExecuteSQL(szSQL));
	}
}
// 密碼正確,則可使registry內的shell變為正常,或是只能run 目前這個應用程式.
bool MachineBase::SetShellAsMyApp(CADOConnection* pCnnPasswordMDB, CString szPassword, bool bUseMyApp)
{
	if (MBase::LOGINTYPE::MAKER != CheckPassword(pCnnPasswordMDB, szPassword))
		return false;	// 不是 MAKER密碼, 則不允許更新 registry的 Shell
	CString szTmp, szMsg;
	HKEY hKey;
	LONG lRet;
	const int BUFSIZE = 255;
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"),
		0, KEY_ALL_ACCESS, &hKey);	// 注意: 若用 KEY_QUERY_VALUE 開啟,則無法寫入.
	if (ERROR_SUCCESS == lRet) {
		wchar_t szAppName[BUFSIZE];
		unsigned char szShellName[BUFSIZE];
		DWORD dwBufLen = BUFSIZE;
		if (bUseMyApp) {
			GetModuleFileName(NULL, szAppName, BUFSIZE);
			szTmp.Format(_T("%s"), szAppName);
		}
		else {
			szTmp.Format(_T("%s"), _T("explorer.exe"));
		}
		int i;
		for (i = 0; i < szTmp.GetLength(); ++i) {
			szShellName[i] = szTmp.GetAt(i);
		}
		szShellName[i] = _T('\0');

		RegSetValueEx(hKey, _T("Shell"), NULL, REG_SZ,
			szShellName, DWORD(i));
		AfxMessageBox(szTmp);

		lRet = RegQueryValueEx(hKey, _T("Shell"), NULL, NULL,
			(LPBYTE)szShellName, &dwBufLen);
		if ((lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE))
			return false;
		szMsg.Format(_T("%s"), szShellName);
		m_AdoTool.EnableCtrlAltDel(!bUseMyApp);	//51207
	}
	else {
		szMsg.Format(_T("%s"), _T("RegOpenKeyEx 失敗!"));
		return false;
	}
	AfxMessageBox(szMsg);
	RegCloseKey(hKey);
	return true;
}
bool MachineBase::isInitComplete()
{
	return m_bInitComplete;
}
bool MachineBase::isHomeComplete()
{
	return m_bHomeComplete;
}
bool MachineBase::Init()
{
	m_bInitComplete = MUnit::Init();
	return m_bInitComplete;
}