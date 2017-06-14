#include "stdafx.h"
#include "Plugin.h"
#include "MPPArm.h"
#include "Machine.h"
using namespace AutoLib;
MPPArm::MPPArm(MUnit *pParent, CString strID, CString strName):
	MUnit(pParent,strID,strName)
{
	//-----------------------Recipe-----------------------------
	m_Type = ArmType::Disable;
	m_intFeederUsed = 0;
	m_dblXBase = 0;
	m_intPickRecipe = -1;
	m_intArrangeForRecipe = -1;
	m_dblZSafty = 100;
	m_dblZOffset = 0;
	//---------------------------------------------------------
	m_pComponent = NULL;
	m_pFeeder = NULL;
	m_dblZ = 0;
	m_dblR = 0;
}

MPPArm::~MPPArm()
{

}
int MPPArm::GetPickRecipe()	//取得目前取臂上的PCB插件元件序號,-1表示未取
{
	return m_intPickRecipe;
}
void MPPArm::SetPickRecipe(int intPickRecipe) //設定取臂上的PCB插件元件序號,-1表示未取
{
	m_intPickRecipe= intPickRecipe;
	return;
}
void MPPArm::LoadRecipeData(CADOConnection * pC, bool bAllChildsLoad)
{
	CADORecordset rsTmp;
	CString strSQL;
	_variant_t id, v;
	if (pC->GetTableIndex(_T("PickPlugArm"))<0)
	{
		strSQL = _T("Create Table PickPlugArm(ID nChar(20),");
		strSQL += _T("ArmType int not null default 0,");
		strSQL += _T("FeederUsed int not null default 0,");
		strSQL += _T("PlugOffsetX float not null default 0,");
		strSQL += _T("PlugOffsetY float not null default 0,");
		strSQL += _T("PlugOffsetZ float not null default 0,");
		strSQL += _T("PlugOffsetS float not null default 0,");
		strSQL += _T("Constraint PKPickPlugArm Primary Key(ID))");
		pC->BeginTrans();
		pC->ExecuteSQL(strSQL);
		pC->CommitTrans();
	}
	if (rsTmp.Open(_T("Select * From PickPlugArm Where ID='") + m_ID + _T("'"), pC))
	{
		if (rsTmp.isEOF()) {
			pC->BeginTrans();
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"), m_ID);
			rsTmp.Update();
			pC->CommitTrans();
		}
		else {
			rsTmp.GetValue(_T("ArmType"), (int&)m_Type);
			rsTmp.GetValue(_T("FeederUsed"), m_intFeederUsed);
			rsTmp.GetValue(_T("PlugOffsetX"), m_cdPlugOffset.x);
			rsTmp.GetValue(_T("PlugOffsetY"), m_cdPlugOffset.y);
			rsTmp.GetValue(_T("PlugOffsetZ"), m_cdPlugOffset.z);
			rsTmp.GetValue(_T("PlugOffsetS"), m_cdPlugOffset.s);
		}
		rsTmp.Close();
	}
	MUnit::LoadRecipeData(pC, bAllChildsLoad);
}
void MPPArm::SaveRecipeData(CADOConnection * pC, bool bAllChildsSave)
{
	CADORecordset rsTmp;
	_variant_t id, v;
	if (rsTmp.Open(_T("Select * From PickPlugArm Where ID='") + m_ID + _T("'"), pC))
	{
		pC->BeginTrans();
		if (rsTmp.isEOF()) {
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"), m_ID);
		}
		rsTmp.SetValue(_T("ArmType"), m_Type);
		rsTmp.SetValue(_T("FeederUsed"), m_intFeederUsed);
		rsTmp.SetValue(_T("PlugOffsetX"), m_cdPlugOffset.x);
		rsTmp.SetValue(_T("PlugOffsetY"), m_cdPlugOffset.y);
		rsTmp.SetValue(_T("PlugOffsetZ"), m_cdPlugOffset.z);
		rsTmp.SetValue(_T("PlugOffsetS"), m_cdPlugOffset.s);
		rsTmp.Update();
		pC->CommitTrans();
		rsTmp.Close();
	}
	MUnit::SaveMachineData(pC, bAllChildsSave);
}
void MPPArm::LoadMachineData(CADOConnection * pC, bool bAllChildsLoad)
{
	CADORecordset rsTmp;
	CString strSQL;
	_variant_t id, v;
	if (pC->GetTableIndex(_T("PickPlugArm"))<0)
	{
		strSQL = _T("Create Table PickPlugArm(ID nChar(20),");
		strSQL += _T("XBasePos float not null default 0,");
		strSQL += _T("ZOffset float not null default 0,");
		strSQL += _T("Constraint PKPickPlugArm Primary Key(ID))");
		pC->BeginTrans();
		pC->ExecuteSQL(strSQL);
		pC->CommitTrans();
	}
	if (rsTmp.Open(_T("Select * From PickPlugArm Where ID='") + m_ID + _T("'"), pC))
	{
		if (rsTmp.isEOF()) {
			pC->BeginTrans();
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"), m_ID);
			rsTmp.Update();
			pC->CommitTrans();
		}
		else {
			rsTmp.GetValue(_T("XBasePos"), m_dblXBase);
			rsTmp.GetValue(_T("ZOffset"), m_dblZOffset);
			rsTmp.GetValue(_T("ZCCDPos"), m_dblZCCDPos);
		}
		rsTmp.Close();
	}
	MUnit::LoadMachineData(pC, bAllChildsLoad);
}
void MPPArm::SaveMachineData(CADOConnection * pC, bool bAllChildsSave)
{
	CADORecordset rsTmp;
	_variant_t id, v;
	if (rsTmp.Open(_T("Select * From PickPlugArm Where ID='") + m_ID + _T("'"), pC))
	{
		pC->BeginTrans();
		if (rsTmp.isEOF()) {
			rsTmp.AddNew();
			rsTmp.SetValue(_T("ID"), m_ID);
		}
		rsTmp.SetValue(_T("XBasePos"), m_dblXBase);
		rsTmp.SetValue(_T("ZOffset"), m_dblZOffset);
		rsTmp.SetValue(_T("ZCCDPos"), m_dblZCCDPos);
		rsTmp.Update();
		pC->CommitTrans();
		rsTmp.Close();
	}
	MUnit::SaveMachineData(pC, bAllChildsSave);
}
void MPPArm::StepCycle(const double dblTime)
{
	double dblZ,dblR;
	switch (m_Step)
	{
	case STEP::StartHome:
		m_strStepName = _T("Start Home");

		break;
	case STEP::StartUpToSafty:
		m_strStepName = _T("Start Up To Safty");
		if (m_pZMotor->AMove(m_dblZSafty))
		{
			m_Step = STEP::WaitToSafty;
		}
		break;
	case STEP::WaitToSafty:
		m_strStepName = _T("Wait Z To Safty");
		if (m_pZMotor->isIDLE())
		{
			m_State = STATE::IDLE;
		}
		break;
	case STEP::StartPick:
		{
			if (m_pComponent == NULL)
			{
				m_strStepName = _T("Start Pick (Component=NULL)");
				dblR = 0;
			}
			else {
				m_strStepName.Format(_T("Start Pick (%s:%s)"),m_pComponent->m_strID,m_pComponent->m_strName);
				dblR = m_pComponent->m_dblPickAngle;
			}
			if (m_pRMotor->AMove(dblR))
			{
				m_Step = STEP::StartDownToFeeder;
			}
		}
		break;
	case STEP::StartDownToFeeder:
		{
			if (m_pFeeder == NULL)
			{
				m_strStepName=_T("Wait R motor then Down To Feeder(NULL)");
				dblZ = 0;
			}
			else {
				m_strStepName.Format(_T("Wait R motor then Down To Feeder(%s)"), m_pFeeder->m_strName);
				dblZ = m_pFeeder->m_cdBase.z;
				if (m_pComponent != NULL)
				{
					dblZ = dblZ + m_pComponent->m_dblHight;
				}
			}
			if (m_pRMotor->isIDLE())
			{
				if (m_pZMotor->AMove(dblZ))
				{
					m_Step = STEP::StartPickComponent;
				}
			}
		}
		break;
	case STEP::StartPickComponent:
		m_strStepName = _T("Wait Z then Pick Component");
		if (m_pZMotor->isIDLE())
		{
			if (m_Mode == MODE::MANUAL)
			{
				m_State = STATE::IDLE;
			}
			else {
				if (m_pValve->OpenValve())
				{
					m_Step = STEP::StartUpToCCDPos;
				}
			}
		}
		break;
	case STEP::StartUpToCCDPos:
		m_strStepName = _T("Wait Valve then UpTo CCDPos");
		if (m_pValve->isIDLE())
		{
			if (m_pZMotor->AMove(m_dblZCCDPos))
			{
				m_Step = STEP::WaitArmUp;
			}
		}
		break;
	case STEP::WaitArmUp:
		m_strStepName = _T("Wait Arm Up");
		if (m_pZMotor->isIDLE())
		{
			m_Step = STEP::WaitArmTurn;
		}
		break;
	case STEP::WaitArmTurn:
		m_strStepName = _T("Wait Arm Up");
		if (m_pZMotor->isIDLE())
		{
			m_State = STATE::IDLE;
		}
		break;
	case STEP::StartPlug:
		{
			m_strStepName = _T("Start to Plug Z down");
	
			dblZ = m_dblZ+m_dblZOffset;
			if (m_pZMotor->AMove(dblZ))
			{
				m_Step = STEP::WaitPlugZDown;
			}
		}
		break;
	case STEP::WaitPlugZDown:
		{
			m_strStepName = _T("Wait Z down then close valve");
			if (m_pZMotor->isIDLE())
			{
				if (m_pValve->CloseValve())
				{
					m_Step = STEP::WaitPlugValveClose;
				}
			}
		}
		break;
	case STEP::WaitPlugValveClose:
		{
			m_strStepName = _T("Wait valve close then Z up");
			if (m_pValve->isIDLE())
			{
				if (m_pZMotor->AMove(m_dblZSafty))
				{
					m_Step = STEP::WaitPlugZUpToSafty;
				}
			}
		}
	case STEP::WaitPlugZUpToSafty:
		{
			m_strStepName = _T("Wait Z up");
			if (m_pZMotor->isIDLE())
			{
				m_State = STATE::IDLE;
			}
		}
		break;
	}
}

bool MPPArm::UpToSafty()
{
	if (DoStep(STEP::StartUpToSafty))
	{
		return true;
	}
	return false;
}
bool MPPArm::DownToFeeder()
{
	if (DoStep(STEP::StartPick))
	{
		m_Mode = MBase::MODE::MANUAL;
		return true;
	}
	return false;
}
bool MPPArm::PickComponent(MFeeder *pF,ComponentData* pC)
{
	if (DoStep(STEP::StartPick))
	{
		m_pFeeder = pF;
		m_pComponent = pC;
		m_Mode = MBase::MODE::AUTO;
		return true;
	}
	return false;
}
bool MPPArm::PlugComponent(double dblR, double dblZ)
{
	if (DoStep(STEP::StartPlug))
	{
		m_dblR = dblR;
		m_dblZ = dblZ;
		m_Mode = MBase::MODE::AUTO;
		return true;
	}
	return false;
}