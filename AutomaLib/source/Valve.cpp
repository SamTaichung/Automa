// Valve.cpp : 實作檔
//

#include "stdafx.h"
#include "Valve.h"

// MValve
using namespace AutoLib;

IMPLEMENT_DYNAMIC(MValve, MBase)

MValve::MValve(MBase *pB,CString strID,CString strName,CString strOpen,CString strClose, ValveStyle vsStyle):
	MBase(pB,strID,strName),m_strOpenStatus(strOpen),m_strCloseStatus(strClose),m_vsStyle(ValveStyle::CylinderValve)
{
	std::map<CString,MValve*>::iterator itFind;
	m_pTMRepeat = NULL;
	m_isPassSensor=false;
	m_bIsVac=false;
	m_pTMOpenTimeOut=NULL;
	m_pTMCloseTimeOut=NULL;
	m_pTMOpenStable=NULL;
	m_pTMCloseStable=NULL;
}
MValve::~MValve()
{

}
bool MValve::GetTestResult()
{
	return(m_TestResult);
}
MError* MValve::OnError(ErrorCode code, CString strError, int RetryStep, CString strReason)
{
	MError *pError;
	MSolution *pSolution;
	m_ErrorCode = code;
	CString strID, strDescript;
	strID.Format(_T("ErrorCode%d"), m_ErrorCode);
	strDescript = m_pStringProvider->GetString(_T("Valve"), strID, strError);
	pError = new MError(this, strDescript, m_ErrorCodeBase + m_ErrorCode, strReason);
	if (RetryStep >= 0)
	{
		strID = _T("Retry");
		strDescript = m_pStringProvider->GetString(_T("String"), _T("Retry"), _T("Retry"));
		pSolution = new MSolution(strDescript,
			this, m_State, RetryStep, m_Mode);
		pError->AddSolution(pSolution);
	}
	ErrorHappen(pError);
	return pError;
}
void MValve::StepCycle(const double dblTime)
{
	MError *pError;
	MSolution *pSolution;
	CString strReason;
	bool isOK=true;
	bool isOK2=true;

	switch(m_Step){
		case STEP::StartOpen:
			m_strStepName = _T("Start Open");
			if (QuerySafe(this,m_State,m_Step,m_Mode,&strReason))
			{
				m_ErrorCode=ErrorCode::ErrNoError;
				if (DoOpen())
				{
					if (m_vmMode==ValveMode::vmForce)
					{
						m_State=MBase::STATE::IDLE;	//強制開啟，不需檢查
					}else{
						if (m_pTMOpenTimeOut!=NULL)
						{
							m_pTMOpenTimeOut->Start();
						}
						m_Step=STEP::WaitOpen;
					}
				}
			}else{		// QuerySafe失敗
				OnError(ErrorCode::ErrNotSafe, _T("Motion of valve is not safe"), m_Step, strReason);
			}
			break;
		case STEP::WaitOpen:
			m_strStepName = _T("Wait Open");
			OnOpening();
			if (isOpen() || m_isPassSensor)
			{
				m_TestResult=isOK;
				m_bIsVac=true;
				if(m_pTMOpenStable!=NULL)
				{
					m_pTMOpenStable->Start();
					m_Step=STEP::WaitOpenStable;
				}else{
					m_Step=STEP::ActionFinish;
				}
			}
			else
			{

				if (m_pTMOpenTimeOut!=NULL)
				{
					if (m_pTMOpenTimeOut->isTimeOut()){
						ASSERT(m_pParent!=NULL);
						if (m_vmMode==ValveMode::vmAutoOff || m_vmMode==ValveMode::vmNGThenOff)
						{
							OffValve();
						}
						if(m_vmMode==ValveMode::vmTest)
						{
							m_bIsVac=false;
							OffValve();
							m_State=MBase::STATE::IDLE;
						}else{
							OnError(ErrorCode::ErrOpenTimeOut, _T("Valve Open TimeOut"), STEP::StartOpen, strReason);
						}
					}
				}
			}
			break;
		case STEP::StartClose:
			m_strStepName = _T("Start Close");
			if (QuerySafe(this,m_State,m_Step,m_Mode,&strReason))	 //向上層查問 電磁閥可否設為CLOSE
			{
				m_ErrorCode=ErrorCode::ErrNoError;
				if (DoClose())
				{
					if (m_vmMode == ValveMode::vmForce)
					{
						m_State = MBase::STATE::IDLE;	//強制關閉，不需檢查
					}
					else {
						if (m_pTMCloseTimeOut != NULL)
						{
							m_pTMCloseTimeOut->Start();
						}
					}
				}
				m_Step=STEP::WaitClose;
			}else{		// QuerySafe失敗
				OnError(ErrorCode::ErrNotSafe, _T("Motion of valve is not safe"), m_Step, strReason);
			}
			break;
		case STEP::WaitClose:
			m_strStepName = _T("Wait Close");
			if (isClose() || m_isPassSensor)
			{
				m_TestResult=true;
				if(m_pTMCloseStable!=NULL)
				{
					m_pTMCloseStable->Start();
					m_Step=STEP::WaitCloseStable;
				}else{
					m_Step=STEP::ActionFinish;
				}
			}else{
				if (m_pTMCloseTimeOut!=NULL)
				{
					if (m_pTMCloseTimeOut->isTimeOut())
					{
						ASSERT(m_pParent!=NULL);
						if (m_vmMode==ValveMode::vmAutoOff || m_vmMode==ValveMode::vmNGThenOff)
						{
							OffValve();
						}
						OnError(ErrorCode::ErrNotSafe, _T("Valve Close TimeOut"), STEP::StartClose, strReason);
					}
				}
			}
			break;
		case STEP::WaitOpenStable:
			m_strStepName = _T("Wait Open-Stable");
			if (m_pTMOpenStable->isTimeOut())
			{
				m_Step=STEP::ActionFinish;
			}
			break;
		case STEP::WaitCloseStable:
			m_strStepName = _T("Wait Close-Stable");
			if (m_pTMCloseStable->isTimeOut())
			{
				m_Step=STEP::ActionFinish;
			}
			break;
		case STEP::ActionFinish:
			m_strStepName = _T("Action finish");
			if (m_vmMode==ValveMode::vmAutoOff || m_vmMode==ValveMode::vmOKThenOff)
			{
				OffValve();
			}
			m_State=MBase::STATE::IDLE;
			break;
		case STEP::RepeatOpen:
			m_strStepName = _T("Repeat-Open");
			if (QuerySafe(this,m_State,m_Step,m_Mode,&strReason))	// 向上層查問 電磁閥可否設為OPEN
			{
				if (DoOpen())
				{
					if (m_pTMRepeat != NULL)
					{
						m_pTMRepeat->Start();
					}
					m_Step = STEP::RepeatWaitOpen;
				}
			}else{
				OnError(ErrorCode::ErrNotSafe, _T("Motion of valve is not safe"), m_Step, strReason);
			}
			break;
		case STEP::RepeatWaitOpen:
			m_strStepName = _T("Wait Repeat-Open");
			if (m_pTMRepeat == NULL)
			{
				m_Step = STEP::RepeatClose;
			}else{
				if (m_pTMRepeat->isTimeOut())
				{
					m_Step = STEP::RepeatClose;
				}
			}
			break;
		case STEP::RepeatClose:
			m_strStepName = _T("Repeat-Close");
			if (QuerySafe(this, m_State, m_Step, m_Mode, &strReason))	// 向上層查問 電磁閥可否設為OPEN
			{
				if (DoClose())
				{
					if (m_pTMRepeat != NULL)
					{
						m_pTMRepeat->Start();
					}
					m_Step = STEP::RepeatWaitClose;
				}
			}else{
				OnError(ErrorCode::ErrNotSafe, _T("Motion of valve is not safe"), m_Step, strReason);
			}
			break;
		case STEP::RepeatWaitClose:
			m_strStepName = _T("Wait Repeat-Close");
			if (m_pTMRepeat == NULL)
			{
				m_Step = STEP::RepeatOpen;
			}
			else {
				if (m_pTMRepeat->isTimeOut())
				{
					m_Step = STEP::RepeatOpen;
				}
			}
			break;
	}
}
CString MValve::GetStateName()
{
	CString strRet;
	if (isOpen())
	{
		strRet=m_strOpenStatus;
	}else if(isClose())
	{
		strRet=m_strCloseStatus;
	}else
	{
		strRet=MBase::GetStateName();
	}
	return(strRet);
}

void MValve::LoadMachineData(CADOConnection * pC,bool bAllChildsLoad)
{
	MBase::LoadMachineData(pC, bAllChildsLoad);
}
void MValve::SaveMachineData(CADOConnection * pC,bool bAllChildsSave)
{
	MBase::SaveMachineData(pC, bAllChildsSave);
}
bool MValve::OffValve()
{

	return true;
}
bool MValve::OpenValve(ValveMode vmMode)
{
	if (DoStep(STEP::StartOpen))
	{
		m_vmMode=vmMode;
		m_TestResult=false;
		return true;
	}	
	return false;
}
bool MValve::Repeat()
{
	if (DoStep(STEP::RepeatOpen))
	{
		m_vmMode = ValveMode::vmForce;
		return true;
	}
	return false;
}
bool MValve::CloseValve(ValveMode vmMode)
{
	if (DoStep(STEP::StartClose))
	{
		m_TestResult=false;
		m_vmMode=vmMode;
		return true;
	}	
	return false;
}
bool MValve::Inverse(ValveMode vmMode)
{
	if (m_State==MBase::STATE::IDLE && !m_isInterLock && !m_isEStop){
		if (isOpen())
		{
			m_Step=STEP::StartClose;
		}else{
			m_Step=STEP::StartOpen;
		}
		m_State=MBase::STATE::ACTION;
		m_TestResult=false;
		m_vmMode=vmMode;
		return true;
	}	
	return false;
}
bool MValve::ValveOnOff(ValveMode vmMode)
{
	if (m_State==MBase::STATE::IDLE){
		if (isOpen())
		{
			OffValve();
		}else{
			m_Step=STEP::StartOpen;
			m_State=MBase::STATE::ACTION;
		}
		m_TestResult=false;
		m_vmMode=vmMode;
		return true;
	}	
	return false;
}
bool MValve::GetAllowToPause()
{
	if (m_State==MBase::STATE::IDLE)
	{
		return MBase::GetAllowToPause();
	}
	return false;
}




// MValve 訊息處理常式
