#pragma once
#include "ADOTool.h"
#include "ADOConnection.h"
#include "Unit.h"
#include "Pilot.h"
#include "Error.h"
#include "Motor.h"
#include "Valve.h"
#include "IO.h"
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
#define WM_MACHINEMESSAGE WM_USER+100

using namespace std;
namespace AutoLib {
	AUTOMALIB_API class MachineBase :
		public MUnit
	{
	public:
		AUTOMALIB_API MachineBase(CWnd *pWnd);
		AUTOMALIB_API ~MachineBase();
		AUTOMALIB_API void ShowMessage(MMessage *pMessage);
		AUTOMALIB_API void ErrorHappen(MError *pError);
		AUTOMALIB_API void Cycle(const double dblTime);
		//-----------建立主Thread以提供所有物件步序----------------
		static UINT DoThread(void *);
		//------------以下是Machine共通的成員-------
		AUTOMALIB_API virtual void SetShutDown();
		AUTOMALIB_API virtual double GetScanTime();
		AUTOMALIB_API virtual bool isEStop() { return false; };
		AUTOMALIB_API virtual bool isInterLock() { return false; };
		AUTOMALIB_API virtual MMotor* AddMotor(MMotor* pMotor);
		AUTOMALIB_API virtual MValve* AddValve(MValve* pValve);
		AUTOMALIB_API virtual MIO* AddIO(MIO* pIO);
		AUTOMALIB_API virtual MTimer* AddTimer(MTimer* pTimer);
		AUTOMALIB_API virtual int GetMotorCount();
		AUTOMALIB_API virtual int GetValveCount();
		AUTOMALIB_API virtual int GetIOCount();
		AUTOMALIB_API virtual int GetTimerCount();
		AUTOMALIB_API virtual MMotor* GetMotor(int index);
		AUTOMALIB_API virtual MValve* GetValve(int index);
		AUTOMALIB_API virtual MIO* GetIO(int index);
		AUTOMALIB_API virtual MTimer* GetTimer(int index);
		AUTOMALIB_API virtual CString GetAppPath();
		AUTOMALIB_API virtual bool LoadSystemData();
		AUTOMALIB_API virtual bool OpenMachineMDB();
		AUTOMALIB_API virtual bool OpenProductionMDB();
		AUTOMALIB_API virtual bool OpenRecipeMDB();
		AUTOMALIB_API virtual bool CloseMachineMDB();
		AUTOMALIB_API virtual bool CloseProductionMDB();
		AUTOMALIB_API virtual bool CloseRecipeMDB();
		AUTOMALIB_API virtual bool SaveMachineData(MBase *pC,bool bAllChildsSave = false);
		AUTOMALIB_API virtual bool LoadMachineData(MBase *pC,bool bAllChildsSave = false);
		AUTOMALIB_API virtual bool SaveRecipeData(MBase *pC, bool bAllChildsSave = false);
		AUTOMALIB_API virtual bool LoadRecipeData(MBase *pC, bool bAllChildsSave = false);
		AUTOMALIB_API virtual void SetMachineState(MPilot::PilotState state);
		AUTOMALIB_API virtual void MachineStop();	//給UI呼叫停機
		AUTOMALIB_API virtual MPilot* GetPilot();	//取Pilot
		AUTOMALIB_API MBase::LOGINTYPE CheckPassword(CADOConnection* pCnn, CString szPWDtoCheck);
		AUTOMALIB_API bool RenewPassword(CADOConnection* pCnn, CString szOldPwd, CString szNewPwd, MBase::LOGINTYPE nUserType);
		AUTOMALIB_API bool WritePassword(CADOConnection* pCnn, CString szNewPWD, MBase::LOGINTYPE UserType);
		AUTOMALIB_API bool SetShellAsMyApp(CADOConnection* pCnnPasswordMDB, CString szPassword, bool bUseMyApp);
	protected:
		AUTOMALIB_API void CreateIO() {};
		AUTOMALIB_API void CreateMotor() {};
		AUTOMALIB_API void CreateValve() {};
		AUTOMALIB_API void CreateTimer() {};

		bool m_bShutDown;		//是否關機
		bool m_bErrorHappen;	//是否有異常發生
		bool m_bMachineStop;	//UI是否呼叫停機
		double m_dblScanTime;
		CString m_strMachineMDB;
		CString m_strRecipeMDB;
		vector<MError *> m_Errors;
		CArray<MMotor*> m_Motors;
		CArray<MValve*> m_Valves;
		CArray<MIO*> m_IOs;
		CArray<MTimer*> m_Timers;
		CADOConnection m_cnnMachine;
		CADOConnection m_cnnRecipe;
		CADOConnection m_cnnProduction;
		MPilot *m_pPilot;
		CWnd* m_pMainFrame;
		CIniFile m_iniLang;
		MBase::LOGINTYPE m_USERTYPE;
		CADOTool m_AdoTool;
	private:
		DWORD m_pThread;
	};
}
