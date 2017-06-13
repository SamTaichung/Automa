#pragma once
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
#include "Base.h"				
namespace AutoLib{
	class MMotor;
	AUTOMALIB_API class MMotionLib : public MBase
	{
	public:
		AUTOMALIB_API MMotionLib(MBase *pParent,CString strID,CString strName);
		AUTOMALIB_API virtual ~MMotionLib();
		AUTOMALIB_API virtual bool Init() { return MBase::Init(); };
		AUTOMALIB_API virtual bool Init(MMotor* pMotor) { return false; };
		AUTOMALIB_API virtual bool Stop(MMotor* pMotor, double dblDesTime) { return false; }; //減速停止
		AUTOMALIB_API virtual bool EStop(MMotor* pMotor) { return false; }; //緊急停止
		AUTOMALIB_API virtual bool AMove(MMotor* pMotor,
				double dblStartSpeed,double dblAccTime, 
				double dblMaxSpeed,double dblDesTime,double dblPos) { return false; };
		AUTOMALIB_API virtual bool RMove(MMotor* pMotor,
			double dblStartSpeed, double dblAccTime,
			double dblMaxSpeed, double dblDesTime, double dblMove) {
			return false;
		};
		AUTOMALIB_API virtual bool VMove(MMotor* pMotor,bool isP,double sp = 0) { return false; };
		AUTOMALIB_API virtual bool Home(MMotor* pMotor) { return false; };
		AUTOMALIB_API virtual bool GotoLimit(MMotor* pMotor,bool isP, bool bSlow = true)//isP=true 移到正極限; false 到負極限
		{
			return false;
		};
		AUTOMALIB_API virtual bool isMotion(MMotor* pMotor);			//是否馬達正在動	
		AUTOMALIB_API virtual bool isHoming(MMotor* pMotor);			//馬達是否在復歸中
		AUTOMALIB_API virtual bool isInp(MMotor* pMotor);				//馬達是否Inposition(定位訊號On)
		AUTOMALIB_API virtual bool isOrg(MMotor* pMotor);				//馬達是否在原點上
		AUTOMALIB_API virtual bool isEMG(MMotor* pMotor);				//馬達是否緊急停止中
		AUTOMALIB_API virtual bool isEZ(MMotor* pMotor);				//馬達是否在EZ訊號上
		AUTOMALIB_API virtual bool isPLim(MMotor* pMotor);				//馬達是否在正極限訊號上
		AUTOMALIB_API virtual bool isMLim(MMotor* pMotor);				//馬達是否在負極限訊號上
		AUTOMALIB_API virtual bool isSVOn(MMotor* pMotor);				//馬達是否Servo On
		AUTOMALIB_API virtual bool isALM(MMotor* pMotor);				//是否ALM
		AUTOMALIB_API virtual bool GetSpeed(MMotor* pMotor, double *pSpeed)=0;//目前速度
		AUTOMALIB_API virtual bool GetPosition(MMotor* pMotor, double *pSpeed)=0;//目前位置
		AUTOMALIB_API virtual void SetPosition(MMotor* pMotor, double Pos);	//設定位置
		AUTOMALIB_API virtual bool SetSVOn(MMotor* pMotor,bool isOn);	//設定是否要Server On
		AUTOMALIB_API virtual int GetALMNo(MMotor* pMotor);
		AUTOMALIB_API virtual WORD GetIOStatus(MMotor* pMotor);
		AUTOMALIB_API virtual void ResetALM(MMotor* pMotor);
		AUTOMALIB_API virtual int GetStatusCode(MMotor* pMotor);		//取得狀態碼

	};

}//namespace AutoLib
