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
		AUTOMALIB_API virtual bool Stop(MMotor* pMotor, double dblDesTime) { return false; }; //��t����
		AUTOMALIB_API virtual bool EStop(MMotor* pMotor) { return false; }; //��氱��
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
		AUTOMALIB_API virtual bool GotoLimit(MMotor* pMotor,bool isP, bool bSlow = true)//isP=true ���쥿����; false ��t����
		{
			return false;
		};
		AUTOMALIB_API virtual bool isMotion(MMotor* pMotor);			//�O�_���F���b��	
		AUTOMALIB_API virtual bool isHoming(MMotor* pMotor);			//���F�O�_�b�_�k��
		AUTOMALIB_API virtual bool isInp(MMotor* pMotor);				//���F�O�_Inposition(�w��T��On)
		AUTOMALIB_API virtual bool isOrg(MMotor* pMotor);				//���F�O�_�b���I�W
		AUTOMALIB_API virtual bool isEMG(MMotor* pMotor);				//���F�O�_��氱�
		AUTOMALIB_API virtual bool isEZ(MMotor* pMotor);				//���F�O�_�bEZ�T���W
		AUTOMALIB_API virtual bool isPLim(MMotor* pMotor);				//���F�O�_�b�������T���W
		AUTOMALIB_API virtual bool isMLim(MMotor* pMotor);				//���F�O�_�b�t�����T���W
		AUTOMALIB_API virtual bool isSVOn(MMotor* pMotor);				//���F�O�_Servo On
		AUTOMALIB_API virtual bool isALM(MMotor* pMotor);				//�O�_ALM
		AUTOMALIB_API virtual bool GetSpeed(MMotor* pMotor, double *pSpeed)=0;//�ثe�t��
		AUTOMALIB_API virtual bool GetPosition(MMotor* pMotor, double *pSpeed)=0;//�ثe��m
		AUTOMALIB_API virtual void SetPosition(MMotor* pMotor, double Pos);	//�]�w��m
		AUTOMALIB_API virtual bool SetSVOn(MMotor* pMotor,bool isOn);	//�]�w�O�_�nServer On
		AUTOMALIB_API virtual int GetALMNo(MMotor* pMotor);
		AUTOMALIB_API virtual WORD GetIOStatus(MMotor* pMotor);
		AUTOMALIB_API virtual void ResetALM(MMotor* pMotor);
		AUTOMALIB_API virtual int GetStatusCode(MMotor* pMotor);		//���o���A�X

	};

}//namespace AutoLib
