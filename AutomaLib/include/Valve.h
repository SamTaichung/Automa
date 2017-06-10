#pragma once
#include <windows.h>
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
#include "IO.h"
#include "timer.h"
#include "Base.h"
// MValve �R�O�ؼ�
namespace AutoLib{ 
AUTOMALIB_API class MValve : public MBase
{
	DECLARE_DYNAMIC(MValve)
public:
	enum ValveStyle{
		CylinderValve,
		VacValve,
	};
	AUTOMALIB_API MValve(MBase *,CString strID,CString strName,
		CString strOpen=_T("Open/�}"),CString strClose=_T("Close/��"),
		ValveStyle vsStyle = ValveStyle::CylinderValve);
	AUTOMALIB_API virtual ~MValve();
	//--------------------------------------------------------------------
	AUTOMALIB_API virtual void StepCycle(const double dblTime);
	AUTOMALIB_API virtual void SaveMachineData(CADOConnection * pC, bool bAllChildsSave = true);
	AUTOMALIB_API virtual void LoadMachineData(CADOConnection * pC, bool bAllChildsLoad = true);
	AUTOMALIB_API virtual CString GetStateName();
	AUTOMALIB_API virtual ValveStyle GetValveStyle(){return m_vsStyle;}; //�Ǧ^�q�ϻ֫���
	AUTOMALIB_API virtual bool DoOpen() { return false; };		//��@Open�ʧ@
	AUTOMALIB_API virtual void OnOpening() {};					//Open�L�{��
	AUTOMALIB_API virtual bool DoClose() { return false; };		//��@Close�ʧ@
	AUTOMALIB_API virtual void OnClosing() {};					//Close�L�{��
	//--------------------------------------------------------------------
	enum ValveMode{
		vmNormal,	  //���`�Ҧ�(���\�Τ����\�����|OffValve)
		vmForce,	  //�j��Ҧ�(���|�޵o���~�]���|OffValve)
		vmTest,		  //���ռҦ��A���G��GetResult���o
		vmAutoOff,    //�p�G����(���\�Τ����\)�N�۰���(OffValve)
		vmOKThenOff,  //�p�G���\�N�۰���(OffValve)
		vmNGThenOff,  //�p�G�����\�N�۰���(OffValve)
	};
	virtual bool Inverse(ValveMode vmMode=ValveMode::vmNormal); //bForce �j��}�ҫᤣ�ˬdSR,bTest �}�Ҵ���,bAutoOFF 
	virtual bool OpenValve(ValveMode vmMode=ValveMode::vmNormal); //bForce �j��}�ҫᤣ�ˬdSR,bTest �}�Ҵ���,bAutoOFF 
	virtual bool isOpen() { return false; };
	virtual bool CloseValve(ValveMode vmMode=ValveMode::vmNormal);
	virtual bool isClose() { return false; };
	virtual bool OffValve();
	virtual bool Repeat();
	virtual bool ValveOnOff(ValveMode vmMode=ValveMode::vmNormal);//��}�үu�Ů�,�A���@���|�۰�����,���|�}�u�� 
	virtual bool GetAllowToPause();				//�O�_�i�H�Ȱ��ʧ@
	bool GetTestResult();	//�Ǧ^���յ��G
	//-----------------------------------------------------------------------------------
	//m_pIOOpen:Open�ʧ@�I
	//m_pIOClose:Close�ʧ@�I
	//m_pIOOpenSR:Open�ʧ@��쪺SR
	//m_pIOCloseSR:Close�ʧ@��쪺SR
	//m_pIOOpenSWSR:Open�ʧ@���p�Gm_pIOOpenSWSR�N��m_pIOOpenSW�]��On
	//m_pIOCloseSWSR:Close�ʧ@���p�Gm_pIOCloseSWSR�N��m_pIOCloseSW�]��On
	//------------------------------------------------------------------------------------
	MTimer *m_pTMOpenTimeOut,*m_pTMCloseTimeOut,*m_pTMOpenStable,*m_pTMCloseStable;
	MTimer *m_pTMRepeat;
	bool m_isPassSensor;
	bool m_bIsVac;
	enum STEP
	{
		StartOpen,
		WaitOpen,
		WaitOpenStable,
		StartClose,
		WaitClose,
		WaitCloseStable,
		ActionFinish,
		RepeatOpen,
		RepeatWaitOpen,
		RepeatClose,
		RepeatWaitClose,
	} ;
	enum ErrorCode
	{
		ErrNoError,
		ErrOpenTimeOut,
		ErrCloseTimeOut,
		ErrNotSafe,		
	};
	CString m_strOpenStatus,m_strCloseStatus;
protected:
	MError* OnError(ErrorCode code, CString strError, int RetryStep, CString strReason);
	int m_RetryStep;	
	ValveMode m_vmMode;
	ValveStyle m_vsStyle;
	bool m_isRepeat;
	ErrorCode m_ErrorCode;
	bool m_TestResult;
};

}//namespace AutoLib