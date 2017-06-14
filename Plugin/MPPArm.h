
#pragma once
#include "Unit.h"
#include "DataStruct.h"
#include "Coordinate.h"
#include "IO.h"
#include "Valve.h"
#include "Motor.h"
#include "DataStruct.h"
#include "MFeeder.h"
// ������u����
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
		Disable,		//���ϥ�
		VacNozzle,		//�u�ŧl�L
		ClampArm,		//����
		Dispensor,		//�
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
		};	//�]�w�Ѯƾ�
	int GetFeeder() { return m_intFeederUsed ; };	
	DCoordinate GetPlugOffset() { return m_cdPlugOffset; }
	void SetPlugOffset(DCoordinate cdOffset) { m_cdPlugOffset = cdOffset; };
	MMotor *m_pZMotor;		//Z Motor
	MMotor *m_pRMotor;		//R Motor(���న�F)
	MValve *m_pValve;		//�l�L�Χ��Y,�
	int GetPickRecipe();	//���o�ثe���u�W��PCB���󤸥�Ǹ�,-1��ܥ���
	void SetPickRecipe(int intPickRecipe);	//�]�w���u�W��PCB���󤸥�Ǹ�,-1��ܥ���
	int m_intArrangeForRecipe;	//�w�p�w�Ʀ�Arm����PCB���󤸥�Ǹ�,-1��ܥ��]�w
	//----------------------------Machine-----------------------------
	double m_dblXBase;				//X��Ǧ�
	double m_dblZOffset;			//Z�ɥ�
	double m_dblZSafty;				//Z�w����
	double m_dblZCCDPos;
 protected:
	//----------------------------Recipe------------------------------
	int m_intPickRecipe;
	int m_intFeederUsed;			//�Ѯƾ��s��
	MFeeder *m_pFeeder;				//�Ѯƾ�
	ArmType m_Type;					//���u����
	DCoordinate m_cdPlugOffset;		//����찾���q
	//----------------------------------------------------------------
	double m_dblZ,m_dblR;
	ComponentData *m_pComponent;
};


