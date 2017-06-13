
#pragma once
#include "Unit.h"
#include "IO.h"
#include "Valve.h"
#include "DataStruct.h"
// MConveyor 命令目標
using namespace AutoLib;
class MConveyor : public MUnit
{

public:
	MConveyor(MUnit *pParent,CString strID,CString strName);
	virtual ~MConveyor();
	enum STEP {
		StartHome,
		StartAuto,
	};
	void StepCycle(const double dblTime);
	void LoadRecipeData(CADOConnection * pC, bool bAllChildsLoad);
	void SaveRecipeData(CADOConnection * pC, bool bAllChildsSave);
	bool IsPCBInReady() { return true; }
	PCBRecipe* GetPCBRecipe() { return &m_PCB; }
	//-----------------------------------------------------------------------
	MIO *m_pIOMotor;			//DC24V直流馬達2個同動
	MIO *m_pIOInPosSR;			//入料位SR
	MIO *m_pIOPlugPosSR;		//插件位SR
	MIO *m_pIOOutPosSR;			//出料位SR
	MValve *m_pInStopper;		//入料位阻檔氣缸
	MValve *m_pPlugStopper;		//插件位阻檔氣缸
	MValve *m_pLockCylinder;	//上頂鎖定氣缸
protected:
	struct PCBRecipe m_PCB;					//PCB Recipe
};


