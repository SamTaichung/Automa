#pragma once
#include "MachineBase.h"
#include "MotionLibVirtual.h"
#include "MotionLib4XMA.h"
#include "CardPCIMaster.h"
#include "ValveVirtual.h"
#include "IOLib.h"
#include "MConveyor.h"
#include "MPickPlug.h"
#include "MSRX2.h"
#include "MSRX4.h"
#include "DataStruct.h"
#define ValveTranslation(pV) (MValveVirtual*)(pV)
using namespace AutoLib;


class Machine :	public MachineBase
{
public:
	Machine(CWnd *pWnd);
	~Machine();
	void Cycle(const double dblTime);
	//---------------------定義初始化步序--------------------------------
	enum InitStep {
		isCardOpen,
		isLoadSystemData,
		isLoadMachineData,
		isLoadRecipeData,
		isMachineComplete,
		isInitMachine,
	}m_InitStep;
	//---------------------定義復歸及自動步序--------------------------------
	enum STEP {
		CheckBeforeAuto,
	};
	//---------------------屬於Machine的物件-------------------------------
	MIO *m_pIO_EMG;				//急停按鈕
	MIO *m_pIO_SysCDA;			//系統空壓
	MConveyor *m_pConveyor;		//輸送機構
	MPickPlug *m_pPickPlug;		//取插機構
	MMotionLib *m_pMotionLib;
	MMotionLib *m_pMotionLib2;
	MIOLib *m_pIOLib;
	MCard	*m_pPCIMaster;
	bool LoadSystemData();
	bool OpenComponentMDB();
	bool CloseComponentMDB();
	ComponentData* GetComponent(CString strFindID);
	bool AddComponent(ComponentData* pComponent);
	bool DelComponent(ComponentData* pComponent);
	bool UpdateComponent(CString strOldID, ComponentData* pComponent);
	bool LoadComponentDatas();
	map<CString, ComponentData*>* GetComponentDatas() { return &m_mapComponents; };
	CADOConnection m_cnnComponents; //插件元件資料庫
protected:
	CString m_strComponentMDB;
	map<CString,ComponentData*> m_mapComponents;
	void CreateMotor();
	void CreateValve();
	void CreateTimer();
	void CreateIO();
};

