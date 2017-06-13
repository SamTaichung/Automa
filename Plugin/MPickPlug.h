
#pragma once
#include "Unit.h"
#include "IO.h"
#include "Valve.h"
#include "Motor.h"
#include "MConveyor.h"
#include "MPPArm.h"
#include "MFeeder.h"
#include "DataStruct.h"
// MPickPlug �R�O�ؼ�
using namespace AutoLib;
class MPickPlug : public MUnit
{
public:
	MPickPlug(MUnit *pParent,CString strID,CString strName,MConveyor *pConveyor);
	virtual ~MPickPlug();
	enum STEP {
		StartHome,
		StartAuto,
		StartMoveToMark,
		WaitMoveToMark,
		PCBStart,
		PreArrangePickArm,
		PreArrangePickFeeder,
		StartToFeeder,
		WaitArmUpToFeeder,
		WaitMoveToFeeder,
		StartToPick,
		WaitPickComponent,
		StartAlignComponent,
		WaitAlignStartPos,
		WaitAlignEndPos,
		StartPlugin,
		MoveToPluginPos,
		WaitToPluginPos,
		StartArmDownToPlug,
		WaitArmToPlug,
		PlugComplete,
		PCBEnd,
	};
	//------------------------MBase------------------------------------
	void StepCycle(const double dblTime);
	void LoadRecipeData(CADOConnection * pC, bool bAllChildsLoad);
	void SaveRecipeData(CADOConnection * pC, bool bAllChildsSave);
	//------------------------MUnit------------------------------------
	bool AutoStart();
	void SetPCBRecipe(PCBRecipe *pPCB);
	PCBRecipe* GetPCBRecipe();
	bool MoveToPCBMark(int index);			//����Mark��
	bool AlignPCBMark();					//����Mark Alignment
	MMotor *m_pXMotor;		//X Motor
	MMotor *m_pYMotor;		//Y Motor
	MPPArm *m_pArm[6];		//6�� PPArm
	MFeeder *m_pFeeder[8];	//8�� Feeder
	DCoordinate m_cdFeederPos; //Feeder��Ǧ�
	DCoordinate m_cdFeederCCD;	//CCD��Ǧ�
protected:
	void AllArmsUp();		//�Ҧ����u�W�ɨ�w����
	bool AllArmsAreIdle();	//�Ҧ����u�ثe�O�_��IDLE
	int GetPickCount();
	int GetArmIDforRecipe(int intRecipeIndex); //���o�p�G�n��RecipeIndex����A�n�Ψ���Arm
	void ArrangePickArm(); //�w�w�ƥ����Ƥ�PPArm�n��������RecipeID
	int GetArrangeCount(); //�p��ثe�w�w�ƥ����ƪ�Arm�ƶq
	MConveyor *m_pConveyor;
	PCBRecipe *m_pPCB;			//�ثe�bConveyor�W��PCB
	int m_intMarkIndex;
	bool m_bAlignPCBMark;
	int m_PickFeederIndex;
	int m_PickArmIndex;
	int m_RecipeIndex;
	CArray<int> m_PlugArmArray;
};


