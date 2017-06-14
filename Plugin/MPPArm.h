
#pragma once
#include "Unit.h"
#include "DataStruct.h"
#include "Coordinate.h"
#include "IO.h"
#include "Valve.h"
#include "Motor.h"
#include "DataStruct.h"
#include "MFeeder.h"
// 元件取臂物件
using namespace AutoLib;
class MPPArm : public MUnit
{
public:
	MPPArm(MUnit *pParent,CString strID,CString strName);
	virtual ~MPPArm();
	
	enum STEP {
		StartHome,
		StartUpToSafty,
		WaitToSafty,
		StartPick,
		StartDownToFeeder,
		StartPickComponent,
		StartUpToCCDPos,
		WaitArmUp,
		WaitArmTurn,
		StartPlug,
		WaitPlugZDown,
		WaitPlugValveClose,
		WaitPlugZUpToSafty,
	};
	enum ArmType {
		Disable,		//不使用
		VacNozzle,		//真空吸嘴
		ClampArm,		//夾爪
		Dispensor,		//塗膠
	};
	//---------------------------------------------------------------
	void LoadRecipeData(CADOConnection * pC, bool bAllChildsLoad);
	void SaveRecipeData(CADOConnection * pC, bool bAllChildsSave);
	void LoadMachineData(CADOConnection * pC, bool bAllChildsLoad);
	void SaveMachineData(CADOConnection * pC, bool bAllChildsSave);
	void StepCycle(const double dblTime);
	//---------------------------------------------------------------
	bool UpToSafty();
	bool DownToFeeder();
	bool PlugComponent(double dblR,double dblZ);
	bool PickComponent(MFeeder*pFeeder, ComponentData *pComponent);
	//---------------------------------------------------------------
	void SetArmType(ArmType tp) { m_Type = tp; };
	ArmType GetArmType() { return m_Type; };
	void SetFeeder(int index) {
			m_intFeederUsed = index; 
		};	//設定供料器
	int GetFeeder() { return m_intFeederUsed ; };	
	DCoordinate GetPlugOffset() { return m_cdPlugOffset; }
	void SetPlugOffset(DCoordinate cdOffset) { m_cdPlugOffset = cdOffset; };
	MMotor *m_pZMotor;		//Z Motor
	MMotor *m_pRMotor;		//R Motor(旋轉馬達)
	MValve *m_pValve;		//吸嘴或夾頭,塗膠
	int GetPickRecipe();	//取得目前取臂上的PCB插件元件序號,-1表示未取
	void SetPickRecipe(int intPickRecipe);	//設定取臂上的PCB插件元件序號,-1表示未取
	int m_intArrangeForRecipe;	//預計安排此Arm給的PCB插件元件序號,-1表示未設定
	//----------------------------Machine-----------------------------
	double m_dblXBase;				//X基準位
	double m_dblZOffset;			//Z補正
	double m_dblZSafty;				//Z安全位
	double m_dblZCCDPos;
 protected:
	//----------------------------Recipe------------------------------
	int m_intPickRecipe;
	int m_intFeederUsed;			//供料器編號
	MFeeder *m_pFeeder;				//供料器
	ArmType m_Type;					//取臂型式
	DCoordinate m_cdPlugOffset;		//插件位偏移量
	//----------------------------------------------------------------
	double m_dblZ,m_dblR;
	ComponentData *m_pComponent;
};


