
#pragma once
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
using namespace std;
#include <math.h>
#include "Base.h"				
#include "Timer.h"				
#include "MotionLib.h"
#ifndef PI
	#define PI 3.1415926
#endif

namespace AutoLib{
	class MMotorCalculate;

	AUTOMALIB_API class MMotor : public MBase
	{
	public:
		friend MMotorCalculate;
		AUTOMALIB_API MMotor(MBase *pB,CString MotorID,CString strName,MMotionLib* pMotion,CString strU);
		AUTOMALIB_API virtual ~MMotor();
		//----------------------------------------------------------------------
		AUTOMALIB_API virtual void StepCycle(const double dblTime);
		AUTOMALIB_API virtual void EStop(const bool isES);
		AUTOMALIB_API virtual void Stop();
		AUTOMALIB_API virtual bool GetAllowToPause();				//是否可以暫停動作
		AUTOMALIB_API virtual void SaveMachineData(CADOConnection * pC, bool bAllChildsSave = true);
		AUTOMALIB_API virtual void LoadMachineData(CADOConnection * pC, bool bAllChildsLoad = true);
		//------------------------------------------------------------------------
		AUTOMALIB_API virtual void SetCalObject(MMotorCalculate *pMC);
		AUTOMALIB_API virtual MMotorCalculate* GetCalObject(){return m_pMotorCalculate;};
		AUTOMALIB_API virtual bool AMove(double p,double sp=0);  //sp:速度,預設0是看m_intSpeedSelect=0時m_HiSpeed,m_intSpeedSelect=1時為m_LoSpeed
		AUTOMALIB_API virtual bool RMove(double r,double sp=0);  //sp:速度,預設0是看m_intSpeedSelect=0時m_HiSpeed,m_intSpeedSelect=1時為m_LoSpeed
		AUTOMALIB_API virtual bool RPulse(int r=1);
		AUTOMALIB_API virtual bool GotoP(bool bSlow=true);//0831 移到 P Limit
		AUTOMALIB_API virtual bool GotoM(bool bSlow=true);//0831 移到 M Limit
		AUTOMALIB_API virtual bool ExitLimit();			//0831 離開 Limit
		AUTOMALIB_API virtual bool Init();				//馬達初始化
		AUTOMALIB_API virtual bool Home();				//復歸
		AUTOMALIB_API virtual bool isMotion();			//是否馬達正在動
		AUTOMALIB_API virtual bool isHoming();			//馬達是否在復歸中
		AUTOMALIB_API virtual bool isInp();				//馬達是否Inposition(定位訊號On)
		AUTOMALIB_API virtual bool isOrg();				//馬達是否在原點上
		AUTOMALIB_API virtual bool isEMG();				//馬達是否緊急停止中
		AUTOMALIB_API virtual bool isEZ();				//馬達是否在EZ訊號上
		AUTOMALIB_API virtual bool isPLim();				//馬達是否在正極限訊號上
		AUTOMALIB_API virtual bool isMLim();				//馬達是否在負極限訊號上
		AUTOMALIB_API virtual bool isSVOn();				//馬達是否Servo On
		AUTOMALIB_API virtual bool isALM();				//是否ALM
		AUTOMALIB_API virtual double GetPosition();		//目前位置
		AUTOMALIB_API virtual double GetSpeed();			//目前速度
		AUTOMALIB_API virtual void SetPosition(double);	//設定位置
		AUTOMALIB_API virtual bool SetSVOn(bool isOn);	//設定是否要Server On
		AUTOMALIB_API virtual WORD GetStatus();
		AUTOMALIB_API virtual int GetALMNo() { return 0; };
		AUTOMALIB_API virtual void ResetALM(){};
		AUTOMALIB_API virtual bool GetMotorHomeComplete(void);
		AUTOMALIB_API virtual void SetMotorHomeComplete(bool bComplete);
		AUTOMALIB_API virtual bool ResetCommand(){return false;};
		AUTOMALIB_API virtual bool ResetFeedBack(){return false;};
		AUTOMALIB_API virtual bool VMove(double p, bool bMove);

		AUTOMALIB_API bool Repeat();
		MTimer *m_pTMHome;	
		AUTOMALIB_API bool SoftLimitCheck(double dbPos);
        bool bMoveDir;
		double m_HiSpeed,m_LoSpeed,m_HomeSpeed,m_HiAccTime,m_P1,m_P2,m_Delay,m_JogPitch,
				m_HiDesTime,m_LoAccTime,m_LoDesTime,m_HomeAccTime,m_HomeDesTime,m_HomeOffset,
				m_Repeat,m_RepeatDir,m_PMax,m_MMax,m_UnitRev,m_PulseRev,m_Coefficient;
		bool m_AxisDir,m_HomeDir,m_SVOnLogic,m_ALMLogic,m_OrgLogic,m_PLimLogic,m_MLimLogic
			,m_INPLogic,m_INPEnable;
		long m_intSpeedSelect,m_intPitchSelect,m_HomeMode,m_RepeatCount;
		unsigned short m_CardID,m_ConnectID,m_StationID,m_AxisID;
		int m_PulseMode,m_CountSource,m_AlarmMode,m_EZCount;
		CString m_Unit;
		enum STEP {
					STARTHOME,
					WAITHOME,
					WAITHOMEOFFSET,
					HomeNotSafe,
					STARTRPulse,
					WAITRPulse,
					STARTRMOVE,
					WAITRMOVE,
					STARTAMOVE,
					WAITAMOVE,
					STARTVMOVE,
					WAITVMOVE,
					STARTREPEAT,
					WAITPOS,
					WAITTIMER,
					GOTOP,
					WAITLIMITP,
					GOTOM,
					WAITLIMITM,
					EXITLIMIT,
					WAITEXITLIMIT,
		};	//0830 增加 GOTOP(M), WAITLIMITP(M), EXITLIMIT, WAITEXITLIMIT
		enum ErrorCode
		{
			errNoError,
			errAMoveError,
			errRMoveError,
			errGoLimitError,
			errExitLimitError,
			errHomeError,
			errHomeTimeOut,
			errLimitCheck,
			errNotSafe,
			errAlarm,
			errNotSVon,
		};
	protected:
		MMotionLib *m_pMotionLib; //此軸使用的Motion Lib
		ErrorCode m_ErrorCode;
		int m_HomeErrorCode;
		double m_dblAMove,m_dblRMove,m_dblSpeed,m_dblVMoveSpeed;
		int m_nJogDir;//吋動方向
		int m_intR;
		bool m_isSVOn,m_bHomeComplete,m_bGotoMSlow;

		MTimer *m_pTMRepeat;
		MMotorCalculate *m_pMotorCalculate;
		CString m_CLastError,m_ELastError;
	private:
		MError* OnError(ErrorCode code, CString strError, int RetryStep,CString strReason=_T(""));
	};

	class MMotorCalculate
	{
	public:
		MMotorCalculate(void){};
		~MMotorCalculate(void){};
		virtual double GetPosition(MMotor* pM);	//傳入馬達換算出位置
		virtual double CalPosition(MMotor* pM,double p){return p;};			//傳入位置換算出馬達量
		virtual double CalAMove(MMotor* pM,double p){return p;};			//計算要AMove的位置
		virtual double CalRMove(MMotor* pM,double r){return r;};			//計算要RMove的位置
	};
	//-------------------------角度----------------------------------------------

	class MMotorSita:public MMotorCalculate
	{
	public:
		MMotorSita(double r):m_dblR(r){};
		~MMotorSita(void){};
		virtual double GetPosition(MMotor* pM)
			{return 180/PI*atan(MMotorCalculate::GetPosition(pM)/m_dblR);};		//傳入馬達換算出位置
		virtual double CalPosition(MMotor* pM,double p)
			{return tan(p/180*PI)*m_dblR;};										//傳入位置換算出馬達量
		virtual double CalAMove(MMotor* pM,double p)
			{return CalPosition(pM,p);};										//計算要AMove的位置
		virtual double CalRMove(MMotor* pM,double r)
			{return CalPosition(pM,r+GetPosition(pM))-MMotorCalculate::GetPosition(pM);};//計算要RMove的位置
	private:
		double m_dblR;
	};
}//namespace AutoLib
