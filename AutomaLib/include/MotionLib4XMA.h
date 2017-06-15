#pragma once
using namespace std;
#include <math.h>					
#include "MotionLib.h"
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
namespace AutoLib {
	AUTOMALIB_API class MMotionLib4XMA :
		public MMotionLib
	{
	public:
		AUTOMALIB_API MMotionLib4XMA(MBase *pB, CString MotorID, CString strName);
		AUTOMALIB_API ~MMotionLib4XMA();
		AUTOMALIB_API bool Init(MMotor* pMotor);
		
	protected:
		AUTOMALIB_API virtual bool AMove(MMotor* pMotor,
			double dblStartSpeed, double dblAccTime,
			double dblMaxSpeed, double dblDesTime,double dblPos);
		AUTOMALIB_API virtual bool RMove(MMotor* pMotor,
			double dblStartSpeed, double dblAccTime,
			double dblMaxSpeed, double dblDesTime, double dblDistance);
		AUTOMALIB_API virtual bool VMove(MMotor* pMotor,
			double dblStartSpeed, double dblAccTime,
			double dblMaxSpeed);
		AUTOMALIB_API virtual bool Stop(MMotor* pMotor, double dblDesTime);
		AUTOMALIB_API virtual bool EStop(MMotor* pMotor);
		AUTOMALIB_API virtual bool Home(MMotor* pMotor,
			double dblStartSpeed, double dblAccTime,
			double dblMaxSpeed, double dblOffset);
		AUTOMALIB_API virtual bool isInp(MMotor* pMotor);				//馬達是否Inposition(定位訊號On)
		AUTOMALIB_API virtual bool isOrg(MMotor* pMotor);				//馬達是否在原點上
		AUTOMALIB_API virtual bool isEMG(MMotor* pMotor);				//馬達是否緊急停止中
		AUTOMALIB_API virtual bool isEZ(MMotor* pMotor);				//馬達是否在EZ訊號上
		AUTOMALIB_API virtual bool isPLim(MMotor* pMotor);				//馬達是否在正極限訊號上
		AUTOMALIB_API virtual bool isMLim(MMotor* pMotor);				//馬達是否在負極限訊號上
		AUTOMALIB_API virtual bool isSVOn(MMotor* pMotor);				//馬達是否Servo On
		AUTOMALIB_API virtual bool isALM(MMotor* pMotor);				//是否ALM
		AUTOMALIB_API virtual bool isMotion(MMotor* pMotor);			//是否馬達正在動	
		AUTOMALIB_API virtual bool GetSpeed(MMotor* pMotor, double *pSpeed);
		AUTOMALIB_API virtual bool GetPosition(MMotor* pMotor, double *pSpeed);
		AUTOMALIB_API virtual void SetPosition(MMotor* pMotor, double Pos);	//設定位置
		AUTOMALIB_API virtual void ResetALM(MMotor* pMotor, bool bValue);
		AUTOMALIB_API virtual WORD GetIOStatus(MMotor* pMotor);
		AUTOMALIB_API virtual bool SetSVOn(MMotor* pMotor, bool isOn);	//設定是否要Server On

	};
}
