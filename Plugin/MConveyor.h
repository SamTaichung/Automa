
#pragma once
#include "Unit.h"
#include "IO.h"
#include "Valve.h"
#include "DataStruct.h"
// MConveyor �R�O�ؼ�
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
	MIO *m_pIOMotor;			//DC24V���y���F2�ӦP��
	MIO *m_pIOInPosSR;			//�J�Ʀ�SR
	MIO *m_pIOPlugPosSR;		//�����SR
	MIO *m_pIOOutPosSR;			//�X�Ʀ�SR
	MValve *m_pInStopper;		//�J�Ʀ���ɮ��
	MValve *m_pPlugStopper;		//�������ɮ��
	MValve *m_pLockCylinder;	//�W����w���
protected:
	struct PCBRecipe m_PCB;					//PCB Recipe
};


