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
		AUTOMALIB_API virtual bool isInp(MMotor* pMotor);				//���F�O�_Inposition(�w��T��On)
		AUTOMALIB_API virtual bool isOrg(MMotor* pMotor);				//���F�O�_�b���I�W
		AUTOMALIB_API virtual bool isEMG(MMotor* pMotor);				//���F�O�_��氱�
		AUTOMALIB_API virtual bool isEZ(MMotor* pMotor);				//���F�O�_�bEZ�T���W
		AUTOMALIB_API virtual bool isPLim(MMotor* pMotor);				//���F�O�_�b�������T���W
		AUTOMALIB_API virtual bool isMLim(MMotor* pMotor);				//���F�O�_�b�t�����T���W
		AUTOMALIB_API virtual bool isSVOn(MMotor* pMotor);				//���F�O�_Servo On
		AUTOMALIB_API virtual bool isALM(MMotor* pMotor);				//�O�_ALM
		AUTOMALIB_API virtual bool isMotion(MMotor* pMotor);			//�O�_���F���b��	
		AUTOMALIB_API virtual bool GetSpeed(MMotor* pMotor, double *pSpeed);
		AUTOMALIB_API virtual bool GetPosition(MMotor* pMotor, double *pSpeed);
		AUTOMALIB_API virtual void SetPosition(MMotor* pMotor, double Pos);	//�]�w��m
		AUTOMALIB_API virtual void ResetALM(MMotor* pMotor, bool bValue);
		AUTOMALIB_API virtual WORD GetIOStatus(MMotor* pMotor);
		AUTOMALIB_API virtual bool SetSVOn(MMotor* pMotor, bool isOn);	//�]�w�O�_�nServer On

	};
}
