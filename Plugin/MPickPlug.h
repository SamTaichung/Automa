
#pragma once
#include "Unit.h"
#include "IO.h"
#include "Valve.h"
#include "Motor.h"
#include "MConveyor.h"
#include "MPPArm.h"
#include "MFeeder.h"
#include "DataStruct.h"
// MPickPlug 命令目標
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
	bool MoveToPCBMark(int index);			//移到Mark位
	bool AlignPCBMark();					//執行Mark Alignment
	MMotor *m_pXMotor;		//X Motor
	MMotor *m_pYMotor;		//Y Motor
	MPPArm *m_pArm[6];		//6組 PPArm
	MFeeder *m_pFeeder[8];	//8組 Feeder
	DCoordinate m_cdFeederPos; //Feeder基準位
	DCoordinate m_cdFeederCCD;	//CCD基準位
protected:
	void AllArmsUp();		//所有取臂上升到安全位
	bool AllArmsAreIdle();	//所有取臂目前是否都IDLE
	int GetPickCount();
	int GetArmIDforRecipe(int intRecipeIndex); //取得如果要取RecipeIndex元件，要用那個Arm
	void ArrangePickArm(); //預安排未取料之PPArm要對應那個RecipeID
	int GetArrangeCount(); //計算目前已安排未取料的Arm數量
	MConveyor *m_pConveyor;
	PCBRecipe *m_pPCB;			//目前在Conveyor上的PCB
	int m_intMarkIndex;
	bool m_bAlignPCBMark;
	int m_PickFeederIndex;
	int m_PickArmIndex;
	int m_RecipeIndex;
	CArray<int> m_PlugArmArray;
};


