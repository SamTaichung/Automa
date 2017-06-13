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
	//---------------------�w�q��l�ƨB��--------------------------------
	enum InitStep {
		isCardOpen,
		isLoadSystemData,
		isLoadMachineData,
		isLoadRecipeData,
		isMachineComplete,
		isInitMachine,
	}m_InitStep;
	//---------------------�w�q�_�k�Φ۰ʨB��--------------------------------
	enum STEP {
		CheckBeforeAuto,
	};
	//---------------------�ݩ�Machine������-------------------------------
	MIO *m_pIO_EMG;				//�氱���s
	MIO *m_pIO_SysCDA;			//�t�Ϊ���
	MConveyor *m_pConveyor;		//��e���c
	MPickPlug *m_pPickPlug;		//�������c
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
	CADOConnection m_cnnComponents; //���󤸥��Ʈw
protected:
	CString m_strComponentMDB;
	map<CString,ComponentData*> m_mapComponents;
	void CreateMotor();
	void CreateValve();
	void CreateTimer();
	void CreateIO();
};

