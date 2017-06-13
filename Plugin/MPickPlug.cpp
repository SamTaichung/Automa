// MConveyor.cpp : 實作檔
//

#include "stdafx.h"
#include "Plugin.h"
#include "MPickPlug.h"


// MConveyor

MPickPlug::MPickPlug(MUnit *pParent, CString strID, CString strName,MConveyor* pConveyor):
	MUnit(pParent,strID,strName),m_pConveyor(pConveyor)
{
	m_pPCB = NULL;
	int count;
	m_intMarkIndex = 0;
	count= sizeof(m_pArm)/sizeof(void*);
	for (int i = 0; i < count; i++)
	{
		CString strID, strName;
		strID.Format(_T("ARM%.2d"), i + 1);
		strName.Format(_T("PickPlugArm%d"), i + 1);
		m_pArm[i] = new MPPArm(this, strID, strName);
	}
	count = sizeof(m_pFeeder) / sizeof(void*);
	for (int i = 0; i < count; i++)
	{
		CString strID, strName;
		strID.Format(_T("FD%.2d"), i + 1);
		strName.Format(_T("Feeder%d"), i + 1);
		m_pFeeder[i] = new MFeeder(this, strID, strName);
	}
}

MPickPlug::~MPickPlug()
{
}
void MPickPlug::StepCycle(const double dblTime)
{
	DCoordinate dcPos;
	switch (m_Step)
	{
	case STEP::StartHome:
		m_strStepName = _T("Start Home");
		break;
	case STEP::StartAuto:
		m_strStepName = _T("Start Auto");
		if (m_pConveyor->IsPCBInReady())
		{
			m_intMarkIndex = 0;
			m_bAlignPCBMark = true;
			m_pPCB = m_pConveyor->GetPCBRecipe();
		}
		m_Step = STEP::StartMoveToMark;
		break;
	case STEP::StartMoveToMark:
		m_strStepName.Format(_T("Start Move To Mark%d"), m_intMarkIndex + 1);
		if (m_pXMotor->isIDLE() && m_pYMotor->isIDLE())
		{
			dcPos = m_pConveyor->GetPCBRecipe()->m_cdPCBMark[m_intMarkIndex];
			m_pXMotor->AMove(dcPos.x);
			m_pYMotor->AMove(dcPos.y);
			m_Step = STEP::WaitMoveToMark;
		}
		break;
	case STEP::WaitMoveToMark:
		m_strStepName.Format(_T("Wait Move To Mark%d"), m_intMarkIndex + 1);
		if (m_pXMotor->isIDLE() && m_pYMotor->isIDLE())
		{
			if (!m_bAlignPCBMark)
			{
				m_State = MBase::STATE::IDLE;
			}
			else {
				if (m_intMarkIndex < 3)
				{
					m_intMarkIndex++;
					m_Step = STEP::StartMoveToMark;
				}
				else { //End of Alignment
					m_intMarkIndex = 0;
					if (m_Mode == MBase::MODE::MANUAL)
					{
						m_State = MBase::STATE::IDLE;
					}
					else {
						m_Step = STEP::PCBStart;
					}
				}
			}
		}
		break;
	case STEP::PCBStart:
		{
			m_strStepName.Format(_T("PCB Start(Nx:%d Ny:%d)"), m_pPCB->m_intXIndex, m_pPCB->m_intYIndex);
			m_Step = STEP::PreArrangePickArm;
		}
		break;
	case STEP::PreArrangePickArm:
		{
			m_strStepName = _T("PreArrange PickArm");
			ArrangePickArm();
			if (GetArrangeCount() > 0) //有已安排但未取料的Arm
			{
				m_Step = STEP::PreArrangePickFeeder;
			}
			else {
				if (GetPickCount() > 0) //目前取臂上有取料
				{
					m_Step = STEP::StartAlignComponent;
				}
				else {
					m_Step = STEP::PCBEnd;
				}
			}
		}
		break;
	case STEP::PreArrangePickFeeder:
		{
			m_strStepName = _T("PreArrange Feeder");
			int count = sizeof(m_pArm) / sizeof(void*);
			m_PickArmIndex = -1;
			m_PickFeederIndex = -1;
			for (int i = 0; i < count; i++)
			{
				if (m_pArm[i]->GetPickRecipe() == -1)
				{
					if (m_pArm[i]->GetArmType() == MPPArm::ArmType::ClampArm ||
						m_pArm[i]->GetArmType() == MPPArm::ArmType::VacNozzle)
					{
						if (m_pArm[i]->m_intArrangeForRecipe >= 0)
						{
							m_PickArmIndex = i;
							m_PickFeederIndex = m_pArm[i]->GetFeeder();
							break;
						}
					}
				}
			}
			if (m_PickArmIndex < 0)
			{
				m_Step = STEP::StartAlignComponent;
			}
			else {
				m_Step = STEP::StartToFeeder;
			}
		}
		break;
	case STEP::StartToFeeder:
		{
			m_strStepName.Format(_T("Start Arm%d To Feeder%d"), m_PickArmIndex+1, m_PickFeederIndex + 1);

			if (AllArmsAreIdle())
			{
				m_Step = STEP::WaitArmUpToFeeder;
			}
		}
		break;
	case STEP::WaitArmUpToFeeder:
		{
			m_strStepName.Format(_T("Wait all Arm Up To Feeder%d"), m_PickFeederIndex + 1);
			
			if (AllArmsAreIdle() && m_pXMotor->isIDLE() && m_pYMotor->isIDLE())
			{
				DCoordinate cdPos;
				cdPos = m_cdFeederPos;
				cdPos.x = cdPos.x + m_pArm[m_PickArmIndex]->m_dblXBase;
				cdPos.x = cdPos.x + m_pFeeder[m_pArm[m_PickArmIndex]->GetFeeder()]->m_cdBase.x;
				m_pXMotor->AMove(cdPos.x);	//移到Feeder位
				m_pYMotor->AMove(cdPos.y);	//移到Feeder位
				m_Step = STEP::WaitMoveToFeeder;
			}
		}
		break;
	case STEP::WaitMoveToFeeder:
		{
			m_strStepName.Format(_T("Wait Move To Feeder%d"), m_PickFeederIndex + 1);
			if (m_pXMotor->isIDLE() && m_pYMotor->isIDLE())
			{
				if (m_Mode == MODE::MANUAL) 
				{
					m_State = STATE::IDLE;
				}
				else {
					m_Step = STEP::StartToPick;
				}
			}
		}
		break;
	case STEP::StartToPick:
		{
			m_strStepName.Format(_T("Start Arm%d Pick Feeder%d"), m_PickArmIndex + 1, m_PickFeederIndex + 1);
			double dblZ;
			dblZ = m_pFeeder[m_PickFeederIndex]->m_cdBase.z;

			if (m_pArm[m_PickArmIndex]->PickComponent(dblZ))
			{
				m_Step = STEP::WaitPickComponent;
			}
		}
		break;
	case STEP::WaitPickComponent:
		{
			m_strStepName.Format(_T("Wait Arm%d Pick [Recipe%d:%s]"), m_PickArmIndex + 1,
				m_pArm[m_PickArmIndex]->m_intArrangeForRecipe+1,
				m_pFeeder[m_PickFeederIndex]->m_strComponentID);
			if (m_pArm[m_PickArmIndex]->isIDLE())
			{
				m_pArm[m_PickArmIndex]->SetPickRecipe(m_pArm[m_PickArmIndex]->m_intArrangeForRecipe);
				m_Step = STEP::PreArrangePickFeeder;
			}
		}
		break;
	case STEP::StartAlignComponent:
		{
			m_strStepName=_T("Start Align Component");
			m_Step = STEP::WaitAlignStartPos;
		}
		break;
	case STEP::WaitAlignStartPos:
		{
			m_strStepName = _T("Wait Align StartPos");
			m_Step = STEP::WaitAlignEndPos;
		}
		break;
	case STEP::WaitAlignEndPos:
		{
			m_strStepName = _T("Wait Align EndPos");
			m_Step = STEP::StartPlugin;
		}
		break;
	case STEP::StartPlugin:
		{
			m_strStepName = _T("Start Plugin");
			m_PickArmIndex = -1;
			m_RecipeIndex = -1;
			//---------------檢查下個插件的Recipe編號對應的Arm------------------
			for (int i = 0; i < m_pPCB->GetPlugPointCount(); i++)
			{
				PluginPoint *pPoint;
				pPoint = m_pPCB->GetPlugPoint(i);
				if (pPoint->m_intArrangeArm >= 0) //此點要作業
				{
					m_RecipeIndex = i;							//設定要作業的Recipe編號
					m_PickArmIndex = pPoint->m_intArrangeArm;	//指定要作業的ARM
					break;
				}
			}
			if (m_RecipeIndex < 0) //取臂上已無可以打件的料
			{
				m_Step = STEP::PreArrangePickArm;
			}
			else {
				m_Step = STEP::MoveToPluginPos;
			}
		}
		break;
	case STEP::MoveToPluginPos:
		{
			PluginPoint *pPoint;
			pPoint = m_pPCB->GetPlugPoint(m_RecipeIndex);
			dcPos = m_pPCB->GetPlugPos(m_RecipeIndex);
			m_strStepName.Format(_T("Move To PluginPos [Recipe%d] Arm%d X:%7.3f Y:%7.3f"),
				m_RecipeIndex+1, m_PickArmIndex+1, dcPos.x, dcPos.y	);
			if (m_pXMotor->isIDLE() && m_pYMotor->isIDLE())
			{
				m_pXMotor->AMove(dcPos.x);
				m_pYMotor->AMove(dcPos.y);
				m_Step = STEP::WaitToPluginPos;
			}
		}
		break;
	case STEP::WaitToPluginPos:
		{
			m_strStepName = _T("Wait To PluginPos");
			if (m_pXMotor->isIDLE() && m_pYMotor->isIDLE())
			{
				m_Step = STEP::StartArmDownToPlug;
			}
		}
		break;
	case STEP::StartArmDownToPlug:
		{
			m_strStepName = _T("Start Arm Down To Plug");
			double dblZ;
			dblZ = m_pPCB->GetPlugPoint(m_RecipeIndex)->m_cdPlugPosition.z;
			if (m_pArm[m_PickArmIndex]->PlugComponent(dblZ))
			{
				m_Step = STEP::WaitArmToPlug;
			}
		}
		break;
	case STEP::WaitArmToPlug:
		{
			m_strStepName.Format(_T("Check Plug Component [Recipe%d] Arm%d"),
				m_RecipeIndex + 1,
				m_PickArmIndex + 1);
			if (m_pArm[m_PickArmIndex]->isIDLE())
			{
				m_Step = STEP::PlugComplete;
			}
		}
		break;
	case STEP::PlugComplete:
		{
			m_strStepName.Format(_T("Plug Recipe%d Complete"),m_RecipeIndex);
			//---------------------------插入成功----------------------------
			PluginPoint *pPoint;
			pPoint = m_pPCB->GetPlugPoint(m_RecipeIndex);
			pPoint->m_bPlugFinish = true;
			pPoint->m_intArrangeArm = -1;
			m_pArm[m_PickArmIndex]->SetPickRecipe(-1);
			m_Step = STEP::StartPlugin; //啟動下個插件工作
		}
		break;
	case STEP::PCBEnd:
	{
		m_strStepName = _T("PCB End");
		if (m_pPCB->IsLastPCB())
		{
			m_pPCB->ResetData();
			MMessage *pMsg = new MMessage();
			pMsg->MsgType = MMessage::MESSAGETYPE::ShowMessage;
			pMsg->strMessage = _T("PCB Plug Complete!");
			ShowMessage(pMsg);
			m_State = STATE::IDLE;
		}
		else {
			m_pPCB->NextPCB();
			m_Step = STEP::PCBStart;
		}
	}
	break;

	}
}
void MPickPlug::SetPCBRecipe(PCBRecipe *pPCB)
{
	m_pPCB = pPCB;
	return;
}
PCBRecipe* MPickPlug::GetPCBRecipe()
{
	return m_pPCB;
}
bool MPickPlug::AllArmsAreIdle()
{
	int count = sizeof(m_pArm) / sizeof(void*);
	bool bAllArmIdle = true;
	for (int i = 0; i < count; i++)
	{
		if (!m_pArm[i]->isIDLE())
		{
			bAllArmIdle = false;
		}
	}
	return bAllArmIdle;
}
void MPickPlug::AllArmsUp()
{
	int count = sizeof(m_pArm) / sizeof(void*);
	for (int i = 0; i < count; i++)
	{
		m_pArm[i]->UpToSafty();
	}
	return ;
}
void MPickPlug::LoadRecipeData(CADOConnection * pC, bool bAllChildsLoad)
{
	MUnit::LoadRecipeData(pC, bAllChildsLoad);
}
void MPickPlug::SaveRecipeData(CADOConnection * pC, bool bAllChildsSave)
{
	MUnit::SaveRecipeData(pC, bAllChildsSave);
}
int MPickPlug::GetArrangeCount()
{
	int bRet = 0;
	int count = sizeof(m_pArm) / sizeof(void*);
	for (int i = 0; i < count; i++)
	{
		if (m_pArm[i]->GetPickRecipe() == -1)
		{
			if (m_pArm[i]->GetArmType() == MPPArm::ArmType::ClampArm ||
				m_pArm[i]->GetArmType() == MPPArm::ArmType::VacNozzle)
			{
				if (m_pArm[i]->m_intArrangeForRecipe >= 0)
				{
					bRet++;
				}
			}
		}
	}
	return bRet;
}
int MPickPlug::GetPickCount()
{
	int bRet = 0;
	int count = sizeof(m_pArm) / sizeof(void*);
	for (int i = 0; i < count; i++)
	{
		if (m_pArm[i]->GetPickRecipe() >=0)
		{
			bRet++;
		}
	}
	return bRet;
}
void MPickPlug::ArrangePickArm()
{
	bool bPicked;
	int ArmID = -1;
	int count;
	int ArangeCount = 0;
	int TotalofAvaiableArm = 0; //目前可以用的ARM
	count = sizeof(m_pArm) / sizeof(void*);
	for (int i = 0; i < count; i++)
	{
		if (m_pArm[i]->GetPickRecipe() == -1) //目前未取零件
		{
			if (m_pArm[i]->GetArmType() == MPPArm::ArmType::ClampArm ||
				m_pArm[i]->GetArmType() == MPPArm::ArmType::VacNozzle)
			{
				TotalofAvaiableArm++;
			}
			m_pArm[i]->m_intArrangeForRecipe = -1; //清除所有預安排設定
		}
	}

	for (int i = 0; i < m_pPCB->GetPlugPointCount(); i++)
	{
		PluginPoint *pPoint;
		pPoint = m_pPCB->GetPlugPoint(i);
		if (pPoint->m_bPlugFinish) {
			pPoint->m_intArrangeArm = -1;
		}
		else { 
			//------------------------------檢查是否該點已有PickArm取得元件----------------------
			bPicked = false;
			for (ArmID = 0; ArmID < count; ArmID++)
			{
				if (m_pArm[ArmID]->GetPickRecipe() == i) //此ARM已取得此點元件
				{
					bPicked = true;
					pPoint->m_intArrangeArm = ArmID;
					break;
				}
			}

			if (!bPicked) //如果此點未完成插件，元件也還沒被PPArm Pick
			{
				ArmID = GetArmIDforRecipe(i);
				if (ArmID >= 0)
				{
					ArangeCount++;
					m_pArm[ArmID]->m_intArrangeForRecipe = i; //預安排
					pPoint->m_intArrangeArm = ArmID;
				}
			}
		}
	}
}
int MPickPlug::GetArmIDforRecipe(int intRecipeIndex)
{
	int count,intFeeder,ret;
	ret = -1;
	PluginPoint *pPoint;
	pPoint = m_pConveyor->GetPCBRecipe()->GetPlugPoint(intRecipeIndex);
	count = sizeof(m_pArm) / sizeof(void*);
	for (int i = 0; i < count; i++)
	{
		if (m_pArm[i]->GetArmType() == MPPArm::ArmType::ClampArm || m_pArm[i]->GetArmType() == MPPArm::ArmType::VacNozzle)
		{
			if (m_pArm[i]->GetPickRecipe() <0 && m_pArm[i]->m_intArrangeForRecipe<0) //表示此Arm未取元件,也未預排
			{				//有指定可用的ARM
				intFeeder = m_pArm[i]->GetFeeder();
				if (m_pFeeder[intFeeder]->m_strComponentID == pPoint->m_strComponentID)
				{
					ret = i; //找到可以用的ARM
					break;
				}
			}
		}
	}
	return ret;
}
bool MPickPlug::MoveToPCBMark(int index)
{
	if (DoStep(STEP::StartMoveToMark))
	{
		m_Mode = MBase::MODE::MANUAL;
		m_intMarkIndex = index;
		m_bAlignPCBMark = false;
		return true;
	}
	return false;
}
bool MPickPlug::AlignPCBMark()
{
	if (DoStep(STEP::StartMoveToMark))
	{
		m_Mode = MBase::MODE::MANUAL;
		m_intMarkIndex = 0;
		m_bAlignPCBMark = true;
		return true;
	}
	return false;
}
bool MPickPlug::AutoStart() 
{
	if (MUnit::AutoStart())
	{
		if (DoStep(STEP::StartAuto))
		{
			m_Mode = MBase::MODE::AUTO;
			return true;
		}
	}
	return false;

}