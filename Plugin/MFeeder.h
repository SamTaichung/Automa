
#pragma once
#include "Unit.h"
#include "IO.h"
#include "DataStruct.h"

// MFeeder �R�O�ؼ�
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
	CString m_strComponentID; //���ƾ��Ѫ�����
	MIO *m_pIO_OK;
	MIO *m_pIO_NG;
	MIO *m_pIO_Alarm;
	MIO *m_pIO_Empty;
	DCoordinate m_cdBase;	//Feeder�����Ʈy��
	CArray<MPPArm*> m_SetUsedArmArray;
};


