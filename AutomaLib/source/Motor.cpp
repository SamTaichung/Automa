// Motor.cpp : 實作檔
//

#include "stdafx.h"
#include "Motor.h"

//啟動軟體極限檢查 有#define是啟動
#define __SOFT_LIMIT_ENABLE__
using namespace AutoLib;
// MMotor
MMotor::MMotor(MBase *pB,CString MotorID,CString strName, MMotionLib* pMotion,CString strU):
	MBase(pB,MotorID,strName),m_Unit(strU), m_pMotionLib(pMotion),
	m_CardID(0), m_ConnectID(0), m_StationID(0), m_AxisID(0),m_UnitRev(0),m_PulseRev(1),
	m_HomeMode(0),m_HiSpeed(5),m_LoSpeed(1),m_HomeSpeed(1),m_HiAccTime(1),m_HiDesTime(1),
	m_LoAccTime(10),m_LoDesTime(10),m_HomeAccTime(1),m_HomeDesTime(1),m_HomeOffset(0),
	m_PMax(0),m_MMax(0),m_P1(0),m_P2(0),m_CountSource(0),m_PulseMode(0),m_Delay(0),m_EZCount(1)
{
	std::map<CString,MMotor*>::iterator itMotor;
	m_RepeatCount=0;
	m_intSpeedSelect=0;
	m_JogPitch=0.1;
	m_intPitchSelect=3;
	m_Repeat=false;
	m_RepeatDir=false;
	m_SVOnLogic=true;
	m_ALMLogic = true;
	m_ALMMode = 1;
	m_SDLogic = true;
	m_SDMode = 0;
	m_LTCLogic = true;
	m_LTCMode = 0;
	m_INPLogic = true;
	m_isSVOn=false;
	m_bHomeComplete=false;
	m_pMotorCalculate=NULL;
	m_pTMHome=NULL;
	m_nJogDir=0;
}
MMotor::~MMotor()
{

}
bool MMotor::Repeat()
{
	if (DoStep(STEP::STARTREPEAT))
	{
		m_RepeatCount=0;
		return true;
	}
	return false;
}
void MMotor::Stop()
{
	m_pMotionLib->Stop(this, m_HiDesTime);
	MBase::Stop();
}

void MMotor::EStop(const bool isES)
{
	MBase::EStop(isES);
	if (isES)
	{
		m_pMotionLib->EStop(this);
		MBase::Stop();	
	}
}
MError* MMotor::OnError(ErrorCode code, CString strError, int RetryStep, CString strReason)
{
	MError *pError;
	MSolution *pSolution;
	m_ErrorCode = code;
	CString strID, strDescript;
	strID.Format(_T("ErrorCode%d"), m_ErrorCode);
	strDescript = m_pStringProvider->GetString(_T("Motor"), strID, strError);
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
void MMotor::StepCycle(const double dblTime)
{
	CString strReason;
	double dblMove,dblSpeed, dblAcctime, dblDectime;
	if (isALM())
	{
		OnError(ErrorCode::errAlarm, _T("Driver of motor is Alarm"), m_Step);
		return;
	}
	if (!isSVOn())
	{
		OnError(ErrorCode::errNotSVon, _T("Motor is not SERVO ON"), m_Step);
		return;
	}

	switch (m_Step)
	{
	case STEP::STARTHOME:
		m_strStepName = _T("Start to home");
		if (QuerySafe(this, m_State, m_Step, m_Mode, &strReason))
		{
			m_HomeErrorCode = 0;
			dblSpeed = abs(m_HomeSpeed / m_Coefficient)*(m_HomeDir?1:-1);
			if (m_pMotionLib->Home(this, 0, m_HomeAccTime, dblSpeed, 0))
			{
				if (m_pTMHome != NULL)
				{
					m_pTMHome->Start();
				}
				m_Step = STEP::WAITHOME;
			}else{
				OnError(ErrorCode::errHomeError, _T("Motor home error"), m_Step);
			}
		}else{
			OnError(ErrorCode::errNotSafe, _T("Motion of motor is not safe"), m_Step);
		}
		break;
	case STEP::WAITHOME:
		m_strStepName = _T("Wait home");
		if (!isHoming())
		{
			if (m_HomeErrorCode == 0)
			{
				if (m_HomeOffset == 0)
				{
					m_Step = STEP::WAITHOMEOFFSET;
				}
				else {
					dblSpeed = m_HomeSpeed /m_Coefficient;
					dblAcctime = m_LoAccTime;
					dblDectime = m_LoDesTime;
					if (m_pMotorCalculate != NULL)
					{
						dblMove = m_pMotorCalculate->CalRMove(this, m_HomeOffset)/m_Coefficient;
					}
					else {
						dblMove = m_HomeOffset / m_Coefficient;
					}
					if (m_pMotionLib->RMove(this, 0, dblAcctime, dblSpeed, dblDectime, dblMove))
					{
						m_Step = STEP::WAITHOMEOFFSET;
					}
					else {
						OnError(ErrorCode::errRMoveError, _T("Motor RMove error"), m_Step);
					}
				}
			}else{
				OnError(ErrorCode::errHomeError, _T("Motor home error"), STEP::STARTHOME);
			}
		}
		else if (m_pTMHome != NULL) {
			if (m_pTMHome->isTimeOut())
			{
				OnError(ErrorCode::errHomeTimeOut, _T("Motor home error"), STEP::STARTHOME);
			}
		}
		break;
	case STEP::WAITHOMEOFFSET:
		m_strStepName = _T("Wait homeoffset");
		if (!isMotion() && isInp())//當開回路時要有驅動器Inpos授才會準
		{
			m_intSpeedSelect = 0;
			SetPosition(0);
			m_bHomeComplete = true;//Completed
			m_State = MBase::STATE::IDLE;
		}
		break;
	case STEP::STARTRPulse:
		m_strStepName.Format(_T("Start RPulse %d Pulse"), m_intR);
		if (QuerySafe(this,m_State,m_Step,m_Mode,&strReason))
		{
			m_dblRMove=m_intR*m_Coefficient;
			if(SoftLimitCheck(m_dblRMove +GetPosition()))
			{
				dblSpeed = abs((m_intSpeedSelect ? m_LoSpeed : m_HiSpeed) / m_Coefficient);
				dblAcctime = (m_intSpeedSelect ? m_LoAccTime : m_HiAccTime);
				dblDectime = (m_intSpeedSelect ? m_LoDesTime : m_HiDesTime);				
				if (m_pMotionLib->RMove(this, 0, dblAcctime, dblSpeed, dblDectime, m_intR))
				{
					m_Step=STEP::WAITRPulse;
				}else{
					OnError(ErrorCode::errRMoveError, _T("Motor RMove error"), m_Step);
				}				
			}
		}else{
			OnError(ErrorCode::errNotSafe, _T("Motion of motor is not safe"), m_Step);
		}
		break;
	case STEP::WAITRPulse:
		m_strStepName = _T("Wait RPulse");
		if (!isMotion() && isInp())
		{
			m_State=MBase::STATE::IDLE;
		}
		break;	
	case STEP::STARTRMOVE:	
		m_strStepName.Format(_T("Start RMove %12.5f %s"),m_dblRMove,m_Unit);
		if (QuerySafe(this,m_State,m_Step,m_Mode,&strReason))
		{
			if(SoftLimitCheck(m_dblRMove +GetPosition()))
			{
				if (m_dblSpeed == 0)
				{
					dblSpeed = abs((m_intSpeedSelect ? m_LoSpeed : m_HiSpeed) / m_Coefficient);
				}
				else {
					dblSpeed = abs(m_dblSpeed / m_Coefficient);
				}
				dblAcctime = (m_intSpeedSelect ? m_LoAccTime : m_HiAccTime);
				dblDectime = (m_intSpeedSelect ? m_LoDesTime : m_HiDesTime);				
				if (m_pMotorCalculate!=NULL)
				{
					dblMove =m_pMotorCalculate->CalRMove(this,m_dblRMove) / m_Coefficient;
				}else{
					dblMove =m_dblRMove / m_Coefficient;
				}
				if (m_pMotionLib->RMove(this, 0, dblAcctime, dblSpeed, dblDectime, dblMove))
				{
					m_Step=STEP::WAITRMOVE;
				}else{
					OnError(ErrorCode::errRMoveError, _T("Motor RMove error"),m_Step);
				}
			}
		}else{
			OnError(ErrorCode::errNotSafe, _T("Motion of motor is not safe"), m_Step);
		}
		break;
	case STEP::WAITRMOVE:
		m_strStepName = _T("Wait RMove");
		if (!isMotion() && isInp())
		{
			m_State=MBase::STATE::IDLE;
		}
		break;	
	case STEP::STARTVMOVE:
		m_strStepName = _T("Start VMove");
		if (QuerySafe(this,m_State,m_Step,m_Mode,&strReason))
		{
			if(m_pMotionLib->VMove(this, bMoveDir,m_dblVMoveSpeed))
			{
				m_Step=STEP::WAITVMOVE;
			}
		}else{
			OnError(ErrorCode::errNotSafe, _T("Motion of motor is not safe"), m_Step);
		}
		break;
	case STEP::WAITVMOVE:
		m_strStepName = _T("Wait VMove");
		{
			m_State=MBase::STATE::IDLE;
		}
		break;	
	case STEP::STARTAMOVE:	
		m_strStepName.Format(_T("Start AMove To %12.5f %s"),m_dblAMove, m_Unit);
		if (QuerySafe(this,m_State,m_Step,m_Mode,&strReason))
		{
			if(SoftLimitCheck(m_dblAMove))
			{
				if (m_dblSpeed == 0)
				{
					dblSpeed = abs((m_intSpeedSelect ? m_LoSpeed : m_HiSpeed) / m_Coefficient);
				}
				else {
					dblSpeed = abs(m_dblSpeed / m_Coefficient);
				}
				dblAcctime = (m_intSpeedSelect ? m_LoAccTime : m_HiAccTime);
				dblDectime = (m_intSpeedSelect ? m_LoDesTime : m_HiDesTime);
				if (m_pMotorCalculate!=NULL)
				{
					dblMove =m_pMotorCalculate->CalAMove(this, m_dblAMove)/m_Coefficient;
				}else{
					dblMove = m_dblAMove/m_Coefficient;
				}
				if (m_pMotionLib->AMove(this,0,dblAcctime,dblSpeed,dblDectime,dblMove))
				{
					m_Step=STEP::WAITAMOVE;
				}else{
					OnError(ErrorCode::errAMoveError, _T("Motor RMove error"), m_Step);
				}
			}
		}else{
			OnError(ErrorCode::errNotSafe, _T("Motion of motor is not safe"), m_Step);
		}
		break;
	case STEP::WAITAMOVE:
		m_strStepName = _T("Wait AMove");
		if (!isMotion() && isInp())
		{
			m_State=MBase::STATE::IDLE;
		}
		break;
	case STEP::STARTREPEAT:
		m_strStepName = _T("Start Repeat");
		if (QuerySafe(this,m_State,m_Step,m_Mode,&strReason))
		{
			if (m_dblSpeed == 0)
			{
				dblSpeed = abs((m_intSpeedSelect ? m_LoSpeed : m_HiSpeed) / m_Coefficient);
			}
			else {
				dblSpeed = abs(m_dblSpeed / m_Coefficient);
			}
			dblAcctime = (m_intSpeedSelect ? m_LoAccTime : m_HiAccTime);
			dblDectime = (m_intSpeedSelect ? m_LoDesTime : m_HiDesTime);
			if (m_pMotorCalculate!=NULL)
			{
				dblMove =m_pMotorCalculate->CalRMove(this,(m_RepeatDir?m_P1:m_P2)) / m_Coefficient;
			}else{
				dblMove =(m_RepeatDir?m_P1:m_P2)/m_Coefficient;
			}
			if (m_pMotionLib->AMove(this, 0, dblAcctime, dblSpeed, dblDectime, dblMove))
			{
				m_Step=STEP::WAITPOS;
			}
		}else{
			OnError(ErrorCode::errNotSafe, _T("Motion of motor is not safe"), m_Step);
		}
		break;
	case STEP::WAITPOS:
		m_strStepName = _T("Wait Position");
		if (!isMotion())
		{
			if (m_pTMRepeat != NULL)
			{
				m_pTMRepeat->Start();
			}
			m_Step=STEP::WAITTIMER;
		}
		break;
	case STEP::WAITTIMER:
		m_strStepName = _T("Wait Timer");
		if (m_pTMRepeat == NULL)
		{
			m_RepeatCount++;
			m_RepeatDir = !m_RepeatDir;
			m_Step = STEP::STARTREPEAT;
		}
		else {
			if (m_pTMRepeat->isTimeOut())
			{
				m_RepeatCount++;
				m_RepeatDir = !m_RepeatDir;
				m_Step = STEP::STARTREPEAT;
			}
		}
		
		break;
	case STEP::GOTOP:	//以低速到正極限
		m_strStepName = _T("Goto P-Limit");
		if (QuerySafe(this,m_State,m_Step,m_Mode,&strReason))
		{
			if (isPLim())
			{
				m_State=MBase::STATE::IDLE;
			}else{
				if (m_pMotionLib->GotoLimit(this,true,m_bGotoMSlow))
				{
					m_Step=STEP::WAITLIMITP;
				}else{
					OnError(ErrorCode::errGoLimitError, _T("Motor go limit error"), m_Step);
				}
			}
		}else{
			OnError(ErrorCode::errNotSafe, _T("Motion of motor is not safe"), m_Step);
		}
		break;
	case STEP::WAITLIMITP:
		m_strStepName = _T("Wait P-Limit");
		if (isPLim())	//If status And IIf(AxisDir, &H1, &H2) Then '-----PEL or MEL
		{
			Stop();
		}
		break;
	case STEP::GOTOM: //以低速到負極限
		m_strStepName = _T("Goto M-Limit");
		if (QuerySafe(this,m_State,m_Step,m_Mode,&strReason))
		{
			if (isMLim())
			{
				m_State=MBase::STATE::IDLE;
			}else{
				if (m_pMotionLib->GotoLimit(this,false,m_bGotoMSlow))
				{
					m_Step=STEP::WAITLIMITM;
				}else{
					OnError(ErrorCode::errGoLimitError, _T("Motor go limit error"), m_Step);
				};
			}
		}else{
			OnError(ErrorCode::errNotSafe, _T("Motion of motor is not safe"), m_Step);
		}
		break;
	case STEP::WAITLIMITM:
		m_strStepName = _T("Wait M-Limit");
		if (isMLim())	//If status And IIf(AxisDir, &H1, &H2) Then '-----PEL or MEL
		{
			Stop();
		}
		break;
	case STEP::EXITLIMIT:
		m_strStepName = _T("Exit Limit");
		if (QuerySafe(this,m_State,m_Step,m_Mode,&strReason))
		{
			if (m_pMotionLib->VMove(this,isMLim()))
			{
				m_Step=STEP::WAITEXITLIMIT;
			}else{
				OnError(ErrorCode::errExitLimitError, _T("Motor exit limit error"), m_Step);
			}
		}else{
			OnError(ErrorCode::errNotSafe, _T("Motion of motor is not safe"), m_Step);
		}
		break;
	case STEP::WAITEXITLIMIT:
		m_strStepName = _T("Wait Exit Limit");
		if (!(isPLim() || isMLim()))
		{
			Stop();
		}
		break;
	}
}
bool MMotor::SetSVOn(bool isOn)
{
	return m_pMotionLib->SetSVOn(this, isOn);
}
bool MMotor::AMove(double p,double sp)
{
	if (DoStep(STEP::STARTAMOVE))
	{
		m_dblAMove =p;
		m_dblSpeed=sp;
		return true;
	}
	return false;
}
bool MMotor::VMove(double p,bool bMove)
{
	if (DoStep(STEP::STARTVMOVE))
	{
		m_dblVMoveSpeed=p;
		bMoveDir=bMove;
		/*dblAMove=p;
		dblSpeed=sp;*/
		return true;
	}
	return false;
} 
bool MMotor::RMove(double r,double sp)
{	
	if (DoStep(STEP::STARTRMOVE))
	{
		m_dblRMove =r;
		m_dblSpeed=sp;
		return true;
	}
	return false;
}
bool MMotor::RPulse(int r)
{
	if (DoStep(STEP::STARTRPulse))
	{
		m_intR=r;
		return true;
	}
	return false;
}
bool MMotor::GotoP(bool bSlow)	//0831
{	
	if (DoStep(STEP::GOTOP))
	{
		m_bGotoMSlow=bSlow;
		return true;
	}
	return false;
}
bool MMotor::GotoM(bool bSlow)	//0831
{	
	if (DoStep(STEP::GOTOM))
	{
		m_bGotoMSlow=bSlow;
		return true;
	}
	return false;
}

bool MMotor::ExitLimit()	//0831
{	
	return DoStep(STEP::EXITLIMIT);
}

double MMotor::GetPosition()
{
	double dblTmp;
	m_pMotionLib->GetPosition(this, &dblTmp);
	dblTmp = dblTmp*m_Coefficient;
	if (m_pMotorCalculate!=NULL)
	{
		dblTmp=m_pMotorCalculate->GetPosition(this);
	}
	return dblTmp;
}
void MMotor::SetPosition(double p)
{
	double dblTmp;
	if (m_pMotorCalculate!=NULL)
	{
		dblTmp=m_pMotorCalculate->CalPosition(this,p);
	}else{
		dblTmp=p;
	}
	dblTmp = dblTmp / m_Coefficient;
	m_pMotionLib->SetPosition(this,dblTmp);
}


bool MMotor::Home()
{
	if (DoHome(STEP::STARTHOME))
	{
		m_bHomeComplete=false;
		return true;
	}
	return false;
}
bool MMotor::isMotion()		//是否馬達正在動
{
	return(m_pMotionLib->isMotion(this));
}
bool MMotor::isHoming()		//馬達是否在復歸中
{
	return(m_pMotionLib->isHoming(this));
}
bool MMotor::isInp()		//馬達是否Inposition(定位訊號On)
{
	return(m_pMotionLib->isInp(this));
}
bool MMotor::isOrg()		//馬達是否在原點上
{
	return(m_pMotionLib->isOrg(this));
}
bool MMotor::isEMG()		//馬達是否緊急停止中
{
	return(m_pMotionLib->isEMG(this));
}
bool MMotor::isEZ()		//馬達是否在Index訊號上
{
	return(m_pMotionLib->isEZ(this));
}
bool MMotor::isPLim()		//馬達是否在正極限訊號上
{
	return(m_pMotionLib->isPLim(this));
}
bool MMotor::isMLim()		//馬達是否在負極限訊號上
{
	return(m_pMotionLib->isMLim(this));
}
bool MMotor::isSVOn()		//馬達是否Servo On
{
	return(m_pMotionLib->isSVOn(this));
}
bool MMotor::isALM()
{
	return(m_pMotionLib->isALM(this));
}
bool MMotor::Init() 
{
	m_Coefficient = 1;
	if (m_PulseRev != 0)
	{
		m_Coefficient = m_UnitRev / m_PulseRev*((m_AxisDir) ? 1 : -1);
	}
	return  m_pMotionLib->Init(this) && SetSVOn(true) && MBase::Init();
}
void MMotor::LoadMachineData(CADOConnection * pC, bool bAllChildsLoad)
{
	CADORecordset rsTmp,rs;
	_variant_t id,v; 
	CString strSQL;
	if (pC->GetTableIndex(_T("Motors"))<0)
	{
		strSQL = _T("Create Table Motors(ID nChar(10),Name nChar(40),");
		strSQL += _T("CardID	int not	null default 0,");
		strSQL += _T("ConnectID	int not	null default 0,");
		strSQL += _T("StationID	int not	null default 0,");
		strSQL += _T("AxisID	int not	null default 0,");
		strSQL += _T("UnitRev float	not null	default	1,");	//每轉行程
		strSQL += _T("PulseRev float not null default 1,");	//每轉Pulse數
		strSQL += _T("HomeMode int not null default 0,");
		strSQL += _T("AxisDir bit not null default -1,");
		strSQL += _T("HomeDir bit not null default -1,");
		strSQL += _T("SVOnLogic bit not null	default	-1,");
		strSQL += _T("ALMLogic bit not null default -1,");
		strSQL += _T("ALMMode bit not null default 0,");
		strSQL += _T("OrgLogic bit not null default -1,");
		strSQL += _T("INPLogic bit not null default -1,");
		strSQL += _T("INPEnable bit not null default 0,");
		strSQL += _T("SDLogic bit not null default -1,");
		strSQL += _T("SDMode int not null default 0,");
		strSQL += _T("LTCLogic bit not null default -1,");
		strSQL += _T("LTCMode int not null default 0,");
		strSQL += _T("EZLogic bit not null default -1,");
		strSQL += _T("EncoderLogic bit not null default -1,");
		strSQL += _T("EncoderMode int not null default 0,");
		strSQL += _T("PLimLogic bit not null	default	-1,");
		strSQL += _T("MLimLogic bit not null	default	-1,");
		strSQL += _T("HiSpeed float not null	default	0,");
		strSQL += _T("LoSpeed float not null	default	0,");
		strSQL += _T("HomeSpeed float not null default 0,");
		strSQL += _T("HiAccTime float not null default 0,");
		strSQL += _T("HiDesTime float not null default 0,");
		strSQL += _T("LoAccTime float not null default 0,");
		strSQL += _T("LoDesTime float not null default 0,");
		strSQL += _T("HomeAccTime float not null default 0,");
		strSQL += _T("HomeDesTime float not null default 0,");
		strSQL += _T("HomeOffset float not null default 0,");
		strSQL += _T("PMaxPos float not null	default	0,");
		strSQL += _T("MMaxPos float not null	default	0,");
		strSQL += _T("P1 float not null default 0,");
		strSQL += _T("P2 float not null default 0,");
		strSQL += _T("Delay float not null default 0,");
		strSQL += _T("Unit nChar(2),");
		strSQL += _T("CountSource int not null default 0,");
		strSQL += _T("PulseMode int not null	default	0,");
		strSQL += _T("Constraint PKMotors Primary Key(ID))");
		pC->BeginTrans();
		pC->ExecuteSQL(strSQL);
		pC->CommitTrans();
	}
	if (rsTmp.Open(_T("Select * From Motors Where ID='") +m_ID+_T("'"),pC))
	{
		if (rsTmp.isEOF()){
			pC->BeginTrans();
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"),m_ID);
			rsTmp.SetValue(_T("Name"),m_strName);
			rsTmp.Update();
			pC->CommitTrans();
			SaveMachineData(pC);
		}else{
			rsTmp.GetValue(_T("UnitRev"),m_UnitRev);
			rsTmp.GetValue(_T("PulseRev"),m_PulseRev);
			rsTmp.GetValue(_T("HomeMode"),m_HomeMode);
			rsTmp.GetValue(_T("CardID"), m_CardID);
			rsTmp.GetValue(_T("ConnectID"), m_ConnectID);
			rsTmp.GetValue(_T("StationID"), m_StationID);
			rsTmp.GetValue(_T("AxisID"), m_AxisID);
			rsTmp.GetValue(_T("AxisDir"),m_AxisDir);
			rsTmp.GetValue(_T("HomeDir"),m_HomeDir);
			rsTmp.GetValue(_T("SVOnLogic"),m_SVOnLogic);
			rsTmp.GetValue(_T("OrgLogic"),m_OrgLogic);
			rsTmp.GetValue(_T("PLimLogic"),m_PLimLogic);
			rsTmp.GetValue(_T("MLimLogic"),m_MLimLogic);
			rsTmp.GetValue(_T("INPLogic"), m_INPLogic);
			rsTmp.GetValue(_T("INPEnable"), m_INPEnable);
			rsTmp.GetValue(_T("ALMLogic"), m_ALMLogic);
			rsTmp.GetValue(_T("ALMMode"), m_ALMMode);
			rsTmp.GetValue(_T("LTCLogic"), m_ALMLogic);
			rsTmp.GetValue(_T("LTCMode"), m_ALMMode);
			rsTmp.GetValue(_T("SDLogic"), m_SDLogic);
			rsTmp.GetValue(_T("SDMode"), m_SDMode);
			rsTmp.GetValue(_T("HiSpeed"), m_HiSpeed);
			rsTmp.GetValue(_T("LoSpeed"),m_LoSpeed);
			rsTmp.GetValue(_T("HomeSpeed"),m_HomeSpeed);
			rsTmp.GetValue(_T("HiAccTime"),m_HiAccTime);
			rsTmp.GetValue(_T("HiDesTime"),m_HiDesTime);
			rsTmp.GetValue(_T("LoAccTime"),m_LoAccTime);
			rsTmp.GetValue(_T("LoDesTime"),m_LoDesTime);
			rsTmp.GetValue(_T("HomeAccTime"),m_HomeAccTime);
			rsTmp.GetValue(_T("HomeDesTime"),m_HomeDesTime);
			rsTmp.GetValue(_T("HomeOffset"),m_HomeOffset);
			rsTmp.GetValue(_T("PMaxPos"),m_PMax);
			rsTmp.GetValue(_T("MMaxPos"),m_MMax);
			rsTmp.GetValue(_T("P1"),m_P1);
			rsTmp.GetValue(_T("P2"),m_P2);
			rsTmp.GetValue(_T("Delay"),m_Delay);
			rsTmp.GetValue(_T("Unit"),m_Unit);
			rsTmp.GetValue(_T("CountSource"),m_CountSource);
			rsTmp.GetValue(_T("PulseMode"), m_PulseMode);
			rsTmp.GetValue(_T("EncoderMode"), m_EncoderMode);
			rsTmp.GetValue(_T("EncoderLogic"), m_EncoderLogic);
			rsTmp.GetValue(_T("EZLogic"), m_EZLogic);
		}
		rsTmp.Close();
	}
	MBase::LoadMachineData(pC,bAllChildsLoad);
}
void MMotor::SaveMachineData(CADOConnection * pC,bool bAllChildsSave)
{
	CADORecordset rsTmp,rs;
	_variant_t id,v;
	if (rsTmp.Open(_T("Select * From Motors Where ID='") +m_ID+_T("'"),pC))
	{
		if (rsTmp.isEOF()){
			pC->BeginTrans();
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"),m_ID);
			rsTmp.Update();
			pC->CommitTrans();
		}
		pC->BeginTrans();
		rsTmp.SetValue(_T("UnitRev"),m_UnitRev);
		rsTmp.SetValue(_T("PulseRev"),m_PulseRev);
		rsTmp.SetValue(_T("HomeMode"),m_HomeMode);
		rsTmp.SetValue(_T("AxisID"),m_AxisID);
		rsTmp.SetValue(_T("AxisDir"),m_AxisDir);
		rsTmp.SetValue(_T("HomeDir"),m_HomeDir);
		rsTmp.SetValue(_T("SVOnLogic"),m_SVOnLogic);
		rsTmp.SetValue(_T("OrgLogic"),m_OrgLogic);
		rsTmp.SetValue(_T("PLimLogic"),m_PLimLogic);
		rsTmp.SetValue(_T("MLimLogic"),m_MLimLogic);
		rsTmp.SetValue(_T("INPLogic"), m_INPLogic);
		rsTmp.SetValue(_T("INPEnable"), m_INPEnable);
		rsTmp.SetValue(_T("ALMLogic"), m_ALMLogic);
		rsTmp.SetValue(_T("ALMMode"), m_ALMMode);
		rsTmp.SetValue(_T("SDLogic"), m_SDLogic);
		rsTmp.SetValue(_T("SDMode"), m_SDMode);
		rsTmp.SetValue(_T("LTCLogic"), m_LTCLogic);
		rsTmp.SetValue(_T("LTCMode"), m_LTCMode);
		rsTmp.SetValue(_T("HiSpeed"), m_HiSpeed);
		rsTmp.SetValue(_T("LoSpeed"),m_LoSpeed);
		rsTmp.SetValue(_T("HomeSpeed"),m_HomeSpeed);
		rsTmp.SetValue(_T("HiAccTime"),m_HiAccTime);
		rsTmp.SetValue(_T("HiDesTime"),m_HiDesTime);
		rsTmp.SetValue(_T("LoAccTime"),m_LoAccTime);
		rsTmp.SetValue(_T("LoDesTime"),m_LoDesTime);
		rsTmp.SetValue(_T("HomeAccTime"),m_HomeAccTime);
		rsTmp.SetValue(_T("HomeDesTime"),m_HomeDesTime);
		rsTmp.SetValue(_T("HomeOffset"),m_HomeOffset);
		rsTmp.SetValue(_T("PMaxPos"),m_PMax);
		rsTmp.SetValue(_T("MMaxPos"),m_MMax);
		rsTmp.SetValue(_T("P1"),m_P1);
		rsTmp.SetValue(_T("P2"),m_P2);
		rsTmp.SetValue(_T("Delay"),m_Delay);
		rsTmp.SetValue(_T("Unit"),m_Unit);
		rsTmp.SetValue(_T("CountSource"),m_CountSource);
		rsTmp.SetValue(_T("PulseMode"), m_PulseMode);
		rsTmp.SetValue(_T("EncoderMode"), m_EncoderMode);
		rsTmp.SetValue(_T("EncoderLogic"), m_EncoderLogic);
		rsTmp.SetValue(_T("EZLogic"), m_EZLogic);
		rsTmp.Update();
		pC->CommitTrans();
		rsTmp.Close();
	}
	MBase::SaveMachineData(pC, bAllChildsSave);
}
double MMotor::GetSpeed()		//目前速度
{
	double dblSpeed;
	m_pMotionLib->GetSpeed(this,&dblSpeed);
	dblSpeed = fabs(dblSpeed*m_Coefficient);
	return dblSpeed;
}
WORD MMotor::GetIOStatus()
{
	return m_pMotionLib->GetIOStatus(this);
}
void MMotor::SetCalObject(MMotorCalculate *pMC)
{
	m_pMotorCalculate=pMC;
}
bool MMotor::GetMotorHomeComplete(void)
{
	return m_bHomeComplete;
}
void MMotor::SetMotorHomeComplete(bool bComplete)
{
	m_bHomeComplete=bComplete;
}
bool MMotor::GetAllowToPause()
{
	if (m_State==MBase::STATE::IDLE)
	{
		return MBase::GetAllowToPause();
	}
	return false;
}


//Soft of Limit Travel Check
bool MMotor::SoftLimitCheck(double dbPos)
{
#ifdef __SOFT_LIMIT_ENABLE__
	CString strReason;
	if((m_PMax==0)&&(m_MMax==0))
		return true;
	else if((dbPos>m_PMax)||(dbPos<m_MMax))
	{
		strReason.Format(_T("Min：%10.3f,Max：%10.3f,Target：%10.3f"),m_MMax,m_PMax,dbPos);
		OnError(ErrorCode::errLimitCheck, _T("Exceeded the soft limit"), m_Step, strReason);
		return false;
	}
#endif	
	return true;

}
