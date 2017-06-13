
#pragma once
#include "Unit.h"
#include "IO.h"
#include "DataStruct.h"

// MFeeder 命令目標
using namespace AutoLib;
class MPPArm;
class MFeeder : public MUnit
{
public:
	MFeeder(MUnit *pParent,CString strID,CString strName);
	virtual ~MFeeder();
	void StepCycle(const double dblTime);
	void LoadMachineData(CADOConnection * pC, bool bAllChildsLoad);
	void SaveMachineData(CADOConnection * pC, bool bAllChildsSave);
	bool GetReadyToFeed();
	CString m_strComponentID; //此料器供的物料
	MIO *m_pIO_OK;
	MIO *m_pIO_NG;
	MIO *m_pIO_Alarm;
	MIO *m_pIO_Empty;
	DCoordinate m_cdBase;	//Feeder的取料座標
	CArray<MPPArm*> m_SetUsedArmArray;
};


