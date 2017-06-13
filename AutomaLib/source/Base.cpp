// Base.cpp : 實作檔
//
#include "stdafx.h"
#include "Base.h"
using namespace AutoLib;

// MBase
//pParent:上層物件(Parent)
//strID:此物件的代碼
//strCName:此物件之名稱
//intErrBase:此物件錯誤代碼基底
MBase::MBase(MBase *pParent, CString strID, CString strName) :
	m_pParent(pParent), m_ID(strID), m_strName(strName), m_pStringProvider(NULL)
{
	m_bLog = false;
	m_strRecordMsg = _T("");
	if (pParent != NULL)
	{
		m_pStringProvider = pParent->GetStringProvider();
		m_strName = m_pStringProvider->GetString(_T("Name"), m_ID, m_strName);
	}else{
		m_pStringProvider = NULL;
	}
	m_strStepName = _T("UnKnow");
	m_Mode = MODE::BASE;
	m_isInterLock = false;
	m_isEStop = false;
	m_State = STATE::INIT;
	m_bRecordLog = false;
	if (pParent != NULL)
	{
		pParent->AddChild(this);
	}
}

MBase::~MBase()
{
	ReleaseChilds();
	RemoveSolutions();
}
//----------------------------------檢查目前機構是否發生錯誤-------------------------------------
//用法：檢查目前機構及其子項是否發生錯誤。
//參數：無
//回傳：true(錯誤發生中),false(無錯誤)
//注意：如果要覆寫此Virtual Function須注意回呼底層物件的GetErrorHappen
//-----------------------------------------------------------------------------------------------
bool MBase::GetErrorHappen()
{
	bool bRet=false;
	std::map<CString,MBase *>::iterator itChild;
	if (m_State==STATE::ERRHAPPEN)
	{
		bRet=true;
	}else{
		for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
		{
			MBase * p_C=(*itChild).second;
			if (p_C->GetErrorHappen())
			{
				bRet=true;
				break;
			}
		}
	}
	return bRet;
}
//---------------------------------------取得登入使用者類型函式----------------------------------------------
//用法：通常只在最上層物件(Machine)覆寫此函式，並把它實作其回傳使用者類型。
//參數：無
//回傳：LOGINTYPE(詳Base.h)
//注意：無
//-----------------------------------------------------------------------------------------------
MBase::LOGINTYPE MBase::GetLoginType()
{
	if (m_pParent!=NULL)
	{
		return m_pParent->GetLoginType();
	}else{
		return UNDEF;
	}
}
//---------------------------------------主循環函式----------------------------------------------
//用法：呼叫所有下層物件Cycle及目前物件如果在Action的State就呼叫StepCycle
//如果是在Home的State就呼叫HomeCycle。
//參數：double dblTime(上次Cycle到本次Cycle的間隔時間，單位：ms)
//回傳：無
//注意：通常此函式並不需覆寫，如果要覆寫此Virtual Function須注意回呼底層物件的Cycle
//-----------------------------------------------------------------------------------------------
void MBase::Cycle(const double dblTime)	
{
	int intPreStep,intPreState;
	intPreStep = m_Step;
	intPreState = m_State;
	std::map<CString,MBase *>::iterator itChild;

	if (m_State==STATE::ACTION || m_State == STATE::HOME)
	{
		StepCycle(dblTime);
		if (m_bRecordLog)
		{
			MMessage *pMsg = new MMessage();
			pMsg->MsgType = MMessage::MESSAGETYPE::LogToFile;
			pMsg->strMessage = m_strName + _T(" [") + GetModeName() + _T("] ") + m_strStepName;
			ShowMessage(pMsg);
			m_bRecordLog = false;
		}		
		if (m_bLog)
		{
			if (m_State == STATE::ACTION)
			{
				if (intPreStep != m_Step || m_State!=intPreState) //Step Change
				{
					m_bRecordLog = true;
				}
			}
		}
	}
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->Cycle(dblTime);
	}
}
//---------------------------------------執行動作-----------------------------------------------
//用法：有動作的物件把執行動作的步驟寫在StepCycle內，並提供一個介面函式，在函式內就是以DoStep去
//		執行某個StepCycle的Step
//參數：int intStep(要執行的Step)
//回傳：true(執行成功),false(執行失敗)
//注意：通常此函式並不需覆寫
//-----------------------------------------------------------------------------------------------
bool MBase::DoStep(int intStep)
{
	if (m_State==STATE::IDLE && !m_isInterLock && !m_isEStop)
	{
		m_State=STATE::ACTION;
		m_Step=intStep;
		m_bRecordLog = m_bLog;
		return true;
	}
	return false;
}
//---------------------------------------復歸動作-----------------------------------------------
//用法：有需要復歸的物件把復歸動作的步驟寫在HomeCycle內，並提供一個介面函式，在函式內就是以DoHome
//		去執行某個HomeCycle的Step
//參數：int intStep(要執行的Step)
//回傳：true(執行成功),false(執行失敗)
//注意：通常此函式並不需覆寫
//-----------------------------------------------------------------------------------------------
bool MBase::DoHome(int intStep)
{
	if (m_State==STATE::IDLE && !m_isInterLock && !m_isEStop)
	{
		m_State=STATE::HOME;
		m_Step=intStep;
		return true;
	}
	return false;
}
//---------------------------------------初始化物件----------------------------------------------
//用法：有需要初始化的物件把初始化覆寫在Init內。
//參數：無
//回傳：true(初始化成功),false(初始化失敗)
//注意：如果要覆寫此Virtual Function須注意回呼底層物件的Init
//-----------------------------------------------------------------------------------------------
bool MBase::Init()
{
	bool bRet=true;
	std::map<CString,MBase *>::iterator itChild;
	m_State=STATE::IDLE;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		if (!p_C->Init())
		{
			ShowMessage(this,p_C->GetFullName()+_T(" Init failure!"),MMessage::MSGIMG::Warn);
			bRet=false;
		}
	}
	return bRet;
}
//---------------------------------------暫停物件動作--------------------------------------------
//用法：此函式提供物件暫停的介面，物件是否處於可暫停狀況則由GetAllowToPause()判斷。
//參數：無
//回傳：true(暫停成功或已處於暫停中),false(目前不可暫停)
//注意：通常此函式並不需覆寫，如果要覆寫此函數須注意子層物件也要被暫停
//-----------------------------------------------------------------------------------------------
bool MBase::Pause()
{
	bool bRet=true; 
	std::map<CString,MBase *>::iterator itChild;
	if (m_State!=STATE::PAUSE)
	{
		if (GetAllowToPause() && (m_State==STATE::ACTION || m_State==STATE::HOME || m_State==STATE::IDLE))
		{
			//-------------------子層物件也要被暫停-------------------------
			for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
			{
				MBase * p_C=(*itChild).second;
				p_C->Pause();
			}
			m_PauseState=m_State;
			m_State=STATE::PAUSE;
		}
	}
	return (m_State==STATE::PAUSE);
}
//------------------------------------是否允許暫停物件動作---------------------------------------
//用法：此函式提供物件暫停的介面，物件是否處於可暫停狀況則由GetAllowToPause()判斷。
//參數：無
//回傳：true(允許暫停),false(不可暫停)
//注意：通常此函式並不需覆寫，如果要覆寫此函數須注意回呼底層物件的GetAllowToPause()
//-----------------------------------------------------------------------------------------------
bool MBase::GetAllowToPause()
{
	bool bRet=true; 
	std::map<CString,MBase *>::iterator itChild;
	if (m_State==STATE::ACTION || m_State==STATE::HOME || m_State==STATE::IDLE || m_State==STATE::PAUSE)
	{
		for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
		{
			MBase * p_C=(*itChild).second;
			if (p_C->m_State!=STATE::PAUSE)
			{
				if (!p_C->GetAllowToPause())
				{
					bRet=false;
				}
			}
		}
	}else{ 
		bRet=false;
	}
	return bRet;
}
//----------------------------------------繼續暫停物件動作---------------------------------------
//用法：此函式提供物件繼續暫停物件動作介面，並同時呼叫其子層物件繼續。
//參數：無
//回傳：true(暫停成功或已處於暫停中),false(目前不可暫停)
//注意：通常此函式並不需覆寫，如果要覆寫此函數須注意回呼底層物件的Continue()
//-----------------------------------------------------------------------------------------------
void MBase::Continue()
{
	std::map<CString,MBase *>::iterator itChild;
	if (m_State==STATE::PAUSE)
	{
		m_State=m_PauseState;
	}
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->Continue();
	}
}
//------------------------------------物件循環停止-----------------------------------------------
//用法：物件動作通常都是程序循環，此函式提供被外部通知停止循環且並同時呼叫其子層物件停止循環。
//參數：無
//回傳：無
//注意：通常此函式只被較上層物件覆寫，如果要覆寫此函數須注意回呼底層物件的CycleStop()
//-----------------------------------------------------------------------------------------------
void MBase::CycleStop()
{
	std::map<CString,MBase *>::iterator itChild;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->CycleStop();
	}
}
//------------------------------------物件循環繼續-----------------------------------------------
//用法：物件動作通常都是程序循環，此函式提供被外部通知停止循環且並同時呼叫其子層物件停止循環。
//參數：無
//回傳：無
//注意：通常此函式只被較上層物件覆寫，如果要覆寫此函數須注意回呼底層物件的CycleStop()
//-----------------------------------------------------------------------------------------------
void MBase::CycleContinue()
{
	std::map<CString,MBase *>::iterator itChild;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->CycleContinue();
	}
}
//------------------------------------刪除子層物件-----------------------------------------------
//用法：此函式主要是列舉出所有子層物件並刪除。
//參數：無
//回傳：無
//注意：此函式主要提供MBase解構時用，不需覆寫
//-----------------------------------------------------------------------------------------------
void MBase::ReleaseChilds()
{
	std::map<CString,MBase *>::iterator itChild;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->ReleaseChilds();
		delete p_C;
	}
	m_Childs.clear();
}
//------------------------------------加入解決方案-----------------------------------------------
//用法：此函式通常提供給父層於呼叫本層或本層所屬子層動作，若發生錯誤時帶出。所附加的解決方案會一直
//		到被呼叫RemoveSolutions()或Stop()後才會移除。
//參數：MSolution *pS(要附加的解決方案指標)
//回傳：無
//注意：此函式主要提供共用功能，不需覆寫
//-----------------------------------------------------------------------------------------------
void MBase::AddSolution(MSolution *pS)
{
	m_ParentSolutions.push_back(pS);
}
//------------------------------------附加解決方案給錯誤物件-------------------------------------
//用法：此函式通常在發生錯誤時，用來把上層提供的解決方案附加到錯誤物件，使用於ErrorHappen()。
//參數：MError * pError(要被附加的錯誤物件指標)
//回傳：無
//注意：此函式主要提供MBase使用，不需覆寫
//-----------------------------------------------------------------------------------------------
void MBase::AddSolutionsTo(MError * pError)
{
	MSolution *pSolution,*pS;
	std::vector<MSolution*>::iterator itSolution;
	if (!m_ParentSolutions.empty())
	{
		for(itSolution=m_ParentSolutions.begin();itSolution!=m_ParentSolutions.end();itSolution++)
		{
			pSolution=*itSolution;
			pS=new MSolution(pSolution);
			pError->AddSolution(pS);
		}
	}
}
//------------------------------------移除被附加的解決方案---------------------------------------
//用法：此函式通常在父層呼叫動作前會，提供的解決方案，若是完成動作後未引發錯誤，用來移除動作前所
//		附加的解決方案。
//參數：無
//回傳：無
//注意：此函式主要提供共用功能，不需覆寫
//-----------------------------------------------------------------------------------------------
void MBase::RemoveSolutions()
{
	MSolution *pSolution;
	std::vector<MSolution*>::iterator itSolution;
	if (!m_ParentSolutions.empty())
	{
		for(itSolution=m_ParentSolutions.begin();itSolution!=m_ParentSolutions.end();itSolution++)
		{
			pSolution=*itSolution;
			delete pSolution;
		}
		m_ParentSolutions.clear();
	}
}
//------------------------------------強制物件停止動作-------------------------------------------
//用法：強制目前狀態改為IDLE並呼叫所有子層Stop()，且移除動作前所附加的解決方案。
//參數：無
//回傳：無
//注意：如果要覆寫此函式時，必須注意回呼底層物件的Stop
//-----------------------------------------------------------------------------------------------
void MBase::Stop()
{	
	std::map<CString,MBase *>::iterator itChild;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->Stop();
	}
	RemoveSolutions();	//清除上層Solution
	m_State=STATE::IDLE;
}
//------------------------------------急停(發生/解除)--------------------------------------------
//用法：強制目前狀態改為EMSTOP或IDLE並呼叫所有子層EStop，且移除動作前所附加的解決方案。
//參數：bool isES(true表急停發生,false表急停解除)
//回傳：無
//注意：如果要覆寫此函式時，必須注意回呼底層物件的EStop
//-----------------------------------------------------------------------------------------------
void MBase::EStop(const bool isES)
{	
	std::map<CString,MBase *>::iterator itChild;
	m_isEStop=isES;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->EStop(isES);
	}
	if (isES)
	{
		m_State=STATE::EMSTOP;
		RemoveSolutions();  //清除上層Solution
	}else{
		m_State=STATE::IDLE;
	}
}
//------------------------------------鎖定(發生/解除)--------------------------------------------
//用法：記錄鎖定發生或解除並呼叫所有子層InterLock。
//參數：bool isInterLock(true表鎖定發生,false表鎖定解除)
//回傳：無
//注意：如果要覆寫此函式時，必須注意回呼底層物件的InterLock
//-----------------------------------------------------------------------------------------------
void MBase::InterLock(const bool isInterLock)
{
	
	std::map<CString,MBase *>::iterator itChild;
	m_isInterLock=isInterLock;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->InterLock(isInterLock);
	}
}
//------------------------------------更改目前使用的語言-----------------------------------------
//用法：記錄語言使用中文或英文並呼叫所有子層ChangeLanguage。
//參數：bool isEnglish(true表英文,false表中文)
//回傳：無
//注意：如果要覆寫此函式時，必須注意回呼底層物件的ChangeLanguage
//-----------------------------------------------------------------------------------------------
void MBase::ChangeLanguage()
{
	std::map<CString,MBase *>::iterator itChild;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->ChangeLanguage();
	}
	if (m_pStringProvider!= NULL)
	{
		m_strName = m_pStringProvider->GetString(_T("Name"), m_ID, m_strName);
	}
}
//---------------------------傳回目前物件及其子物件是否都處於IDLE--------------------------------
//用法：記錄語言使用中文或英文並呼叫所有子層ChangeLanguage。
//參數：無
//回傳：true(目前都在IDLE),false(目前未全在IDLE)
//注意：此函式主要提供共用功能，不需覆寫
//-----------------------------------------------------------------------------------------------
bool MBase::isIDLE()
{
	bool ret=true;
	std::map<CString,MBase *>::iterator itChild;
	if (m_State==STATE::IDLE && !m_isInterLock && !m_isEStop)
	{
		for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
		{
			MBase * p_C=(*itChild).second;
			if(!p_C->isIDLE())
			{
				ret=false;
				break;
			}
		}
	}else{
		ret=false;
	}
	return(ret);
}
//----------------------------------------加入子層物件-------------------------------------------
//用法：加入一個子層物件到本層。
//參數：MBase * p_C(要加入的子件指標)
//回傳：被加入的子件指標
//注意：此函式主要提供共用功能，不需覆寫
//-----------------------------------------------------------------------------------------------
MBase* MBase::AddChild(MBase * p_C)
{
	m_Childs.insert(std::map<CString,MBase*>::value_type(p_C->m_ID,p_C));
	p_C->m_ErrorCodeBase = m_Childs.size()*ERROR_CODE_SPACE + m_ErrorCodeBase;
	return(p_C);
}
//----------------------------------------取得子層物件-------------------------------------------
//用法：取得的子層物件代碼為ID的物件指標。
//參數：CString ID(要取得的子件代碼)
//回傳：若找到要取得的子件代碼就傳回該子件指標，否則傳回NULL
//注意：此函式主要提供共用功能，不需覆寫
//-----------------------------------------------------------------------------------------------
MBase * MBase::GetChild(CString ID)
{
	std::map<CString,MBase *>::iterator itChild;
	itChild=m_Childs.find(ID);
	if(itChild!=m_Childs.end())
	{
		return((*itChild).second);
	}
	return(NULL);	
}
//----------------------------------------刪除子層物件-------------------------------------------
//用法：把子層物件代碼為ID的物件從子層刪除(非解構)。
//參數：CString ID(要刪除的子件代碼)
//回傳：true(找到要刪除的子件代碼，並刪除成功),false(找不到代碼)
//注意：此函式主要提供共用功能，不需覆寫
//-----------------------------------------------------------------------------------------------
bool MBase::DelChild(CString ID)
{
	std::map<CString,MBase *>::iterator itChild;
	itChild=m_Childs.find(ID);
	if(itChild!=m_Childs.end())
	{
		m_Childs.erase(ID);
		return(true);
	}
	return(false);	
}
//------------------------------------------錯誤發生---------------------------------------------
//用法：若引發錯誤的物件為本物件則把狀態改為ERRHAPPEN，把上層所附加的解決方案加到錯誤並往父層引發
//		錯誤。
//參數：MError * pError(發生的錯誤指標)
//回傳：無
//注意：
//-----------------------------------------------------------------------------------------------
void MBase::ErrorHappen(MError * pError)
{
	if (pError->m_Happener==this)
	{
		m_State=STATE::ERRHAPPEN;
	}
	if (m_pParent!=NULL) {
		AddSolutionsTo(pError);				//加入上層解決方案
		m_pParent->ErrorHappen(pError);		
	}
}
//------------------------------------------錯誤處理---------------------------------------------
//用法：處理一個解決方案。
//參數：MSolution *pSolution(要處理的解決方案)
//回傳：無
//注意：
//-----------------------------------------------------------------------------------------------
void MBase::ErrorProcess(STATE state,int intStep,int intMode)
{
	if (!m_isEStop)
	{
		m_State = (STATE)state;
		m_Step = intStep;
		m_Mode = (MODE)(intMode);
	}
}
//------------------------------------------查詢安全---------------------------------------------
//用法：主要用於動作前查詢是否為安全。
//參數：MBase *pFrom(要動作的物件指標)
//		int intState(要執行動作的狀態：通常是ACTION或HOME)
//		int intStep(要執行動作的Step)
//		int intMode(要執行動作的mode)
//		CString *pstrCDiscript(當查詢不安全時，提示說明的中文字串)
//		CString *pstrEDiscript(當查詢不安全時，提示說明的英文字串)
//回傳：true(安全),false(不安全)
//注意：如果要覆寫此函式時，必須注意回呼底層物件的QuerySafe
//-----------------------------------------------------------------------------------------------
bool MBase::QuerySafe(MBase *pFrom,int intState,int intStep,int intMode,CString *pstrReason)
{
	if (m_pParent!=NULL) {
		return m_pParent->QuerySafe(pFrom,intState,intStep,intMode, pstrReason);		//向上層詢問安全
	}
	return true;
}
//------------------------------------------訊息通知(字串型式)-----------------------------------
//用法：主要用於對最上層物件通知訊息，最上層再把訊息Post到Mainfrm去處理。
//參數：MBase *pFrom(發出訊息的物件指標)
//		CString strMsg(訊息內容)
//		MMessage::MSGIMG img(訊息類別)
//回傳：無
//注意：此函式主要提供共用功能，不需覆寫
//-----------------------------------------------------------------------------------------------
void MBase::ShowMessage(MBase *pFrom,CString strMsg,MMessage::MSGIMG img)
{
	if (m_pParent!=NULL) {
		m_pParent->ShowMessage(pFrom,strMsg,img);
	}
}
//------------------------------------------訊息通知(物件型式)------------------------------------
//用法：主要用於對最上層物件通知訊息，最上層再把訊息Post到Mainfrm去處理。
//參數：MMessage* pMsg(要通知的訊息物件指標)
//回傳：無
//注意：此函式主要提供共用功能，不需覆寫
//-----------------------------------------------------------------------------------------------

void MBase::ShowMessage(MMessage* pMsg)
{
	if (m_pParent!=NULL)
	{
		m_pParent->ShowMessage(pMsg);
	}else{
		delete pMsg;
	}
}
//------------------------------------------取得物件完整名稱-------------------------------------
//用法：主要用於對最上層物件通知訊息，最上層再把訊息Post到Mainfrm去處理。
//參數：MMessage* pMsg(要通知的訊息物件指標)
//回傳：無
//注意：此函式主要提供共用功能，不需覆寫
//-----------------------------------------------------------------------------------------------
CString MBase::GetFullName()
{
	CString strRet;
	strRet=m_strName;
	if(m_pParent!=NULL && m_pParent->GetFullName() !=_T(""))
	{
		strRet=m_pParent->GetFullName()+"_"+strRet;
	}
	return strRet;
}

//--------------------------------------載入機台參數---------------------------------------------
//用法：如果物件有機台參數要自行存取時覆寫此函式，此函式由最上層物件開始呼叫。
//參數：CADOConnection * p_C(存放機台參數的資料庫連線指標)
//回傳：無
//注意：如果要覆寫此函式時，必須注意回呼底層物件的LoadMachineData
//備註：機台參數是指會隨不同機台而變，而不隨產品型號而變的資料，例如：基準位
//-----------------------------------------------------------------------------------------------
void MBase::LoadMachineData(CADOConnection * p_C,bool bAllChildsLoad)
{
	if (bAllChildsLoad)
	{
		std::map<CString, MBase *>::iterator itChild;
		for (itChild = m_Childs.begin();itChild != m_Childs.end();++itChild)
		{
			MBase * pChild = (*itChild).second;
			pChild->LoadMachineData(p_C,bAllChildsLoad);
		}
	}
}
//--------------------------------------儲存機台參數---------------------------------------------
//用法：如果物件有機台參數要自行存取時覆寫此函式，此函式由最上層物件開始呼叫。
//參數：CADOConnection * p_C(存放機台參數的資料庫連線指標)
//回傳：無
//注意：如果要覆寫此函式時，必須注意回呼底層物件的SaveMachineData
//備註：機台參數是指會隨不同機台而變，而不隨產品型號而變的資料，例如：基準位
//-----------------------------------------------------------------------------------------------
void MBase::SaveMachineData(CADOConnection * p_C,bool bAllChildsSave)
{
	if (bAllChildsSave)
	{
		std::map<CString, MBase *>::iterator itChild;
		for (itChild = m_Childs.begin();itChild != m_Childs.end();itChild++)
		{
			MBase * pChild = (*itChild).second;
			pChild->SaveMachineData(p_C, bAllChildsSave);
		}
	}
}

//--------------------------------------載入產品參數---------------------------------------------
//用法：如果物件有產品參數要自行存取時覆寫此函式，此函式由最上層物件開始呼叫。
//參數：CADOConnection * p_C(存放產品參數的資料庫連線指標)
//回傳：無
//注意：如果要覆寫此函式時，必須注意回呼底層物件的LoadRecipeData
//備註：產品參數是指會隨不同產品型號而變，而不隨機台而變的資料，例如：產品長、寬、厚及對位記號
//		座標等
//-----------------------------------------------------------------------------------------------
void MBase::LoadRecipeData(CADOConnection * pC,bool bAllChildsLoad)
{
	if (bAllChildsLoad)
	{
		std::map<CString, MBase *>::iterator itChild;
		for (itChild = m_Childs.begin();itChild != m_Childs.end();itChild++)
		{
			MBase * p_C = (*itChild).second;
			p_C->LoadRecipeData(pC, bAllChildsLoad);
		}
	}
}

//--------------------------------------儲存產品參數---------------------------------------------
//用法：如果物件有產品參數要自行存取時覆寫此函式，此函式由最上層物件開始呼叫。
//參數：CADOConnection * p_C(存放產品參數的資料庫連線指標)
//回傳：無
//注意：如果要覆寫此函式時，必須注意回呼底層物件的SaveRecipeData
//備註：產品參數是指會隨不同產品型號而變，而不隨機台而變的資料，例如：產品長、寬、厚及對位記號
//		座標等
//-----------------------------------------------------------------------------------------------
void MBase::SaveRecipeData(CADOConnection * pC,bool bAllChildsSave)
{
	if (bAllChildsSave)
	{
		std::map<CString, MBase *>::iterator itChild;
		for (itChild = m_Childs.begin();itChild != m_Childs.end();itChild++)
		{
			MBase * p_C = (*itChild).second;
			p_C->SaveRecipeData(pC, bAllChildsSave);
		}
	}
}

//-------------------------------取得最上層物件(一般是Machine)-----------------------------------
//用法：如果物件要取得最上層物件指標就以此函式呼叫。
//參數：無
//回傳：MBase *(最上層物件指標)
//注意：此函式主要提供共用功能，不需覆寫
//-----------------------------------------------------------------------------------------------
MBase * MBase::GetTopParent()
{
	if (m_pParent != NULL)
	{
		return(m_pParent->GetTopParent());
	}
	else
	{
		return(this);
	}
}
//-------------------------------取得上層物件-----------------------------------
//用法：如果物件要取得最上層物件指標就以此函式呼叫。
//參數：無
//回傳：MBase *(最上層物件指標)
//注意：此函式主要提供共用功能，不需覆寫
//-----------------------------------------------------------------------------------------------
MBase * MBase::GetParent()
{
		return(m_pParent);
}
CString MBase::GetStateName()
{
	CString strRet[9] = { _T("INIT"),
			_T("IDLE"),
			_T("ACTION"),
			_T("HOME"),
			_T("ERROR"),
			_T("ESTOP"),
			_T("PAUSE"),
			_T("SUSPEND"),
			_T("RESUME") };
	return strRet[m_State];
}
CString MBase::GetStepName()
{
	if (m_State == STATE::ACTION || m_State == STATE::HOME)
	{
		return m_strStepName;
	}
	return _T("");
}
CString MBase::GetModeName()
{
	CString strRet[4] = { _T("BASE"),
		_T("AUTO"),
		_T("MANUAL"),
		_T("MAINTAIN")};
	return strRet[m_Mode];
}
CStringProvider* MBase::GetStringProvider()
{
	return m_pStringProvider;
}
void MBase::SetLog(bool bLog)
{
	std::map<CString, MBase *>::iterator itChild;
	m_bLog = bLog;
	for (itChild = m_Childs.begin(); itChild != m_Childs.end(); itChild++)
	{
		MBase * p_C = (*itChild).second;
		p_C->SetLog(bLog);
	}
}
// MBase 訊息處理常式
