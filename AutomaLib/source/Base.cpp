// Base.cpp : ��@��
//
#include "stdafx.h"
#include "Base.h"
using namespace AutoLib;

// MBase
//pParent:�W�h����(Parent)
//strID:�����󪺥N�X
//strCName:�����󤧦W��
//intErrBase:��������~�N�X��
MBase::MBase(MBase *pParent, CString strID, CString strName) :
	m_pParent(pParent), m_ID(strID), m_strName(strName), m_pStringProvider(NULL)
{
	m_bLog = false;
	m_strRecordMsg = _T("");
	if (pParent != NULL)
	{
		m_pStringProvider = pParent->GetStringProvider();
		m_strName = m_pStringProvider->GetString(_T("Name"), m_ID, m_strName);
	}else{
		m_pStringProvider = NULL;
	}
	m_strStepName = _T("UnKnow");
	m_Mode = MODE::BASE;
	m_isInterLock = false;
	m_isEStop = false;
	m_State = STATE::INIT;
	m_bRecordLog = false;
	if (pParent != NULL)
	{
		pParent->AddChild(this);
	}
}

MBase::~MBase()
{
	ReleaseChilds();
	RemoveSolutions();
}
//----------------------------------�ˬd�ثe���c�O�_�o�Ϳ��~-------------------------------------
//�Ϊk�G�ˬd�ثe���c�Ψ�l���O�_�o�Ϳ��~�C
//�ѼơG�L
//�^�ǡGtrue(���~�o�ͤ�),false(�L���~)
//�`�N�G�p�G�n�мg��Virtual Function���`�N�^�I���h����GetErrorHappen
//-----------------------------------------------------------------------------------------------
bool MBase::GetErrorHappen()
{
	bool bRet=false;
	std::map<CString,MBase *>::iterator itChild;
	if (m_State==STATE::ERRHAPPEN)
	{
		bRet=true;
	}else{
		for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
		{
			MBase * p_C=(*itChild).second;
			if (p_C->GetErrorHappen())
			{
				bRet=true;
				break;
			}
		}
	}
	return bRet;
}
//---------------------------------------���o�n�J�ϥΪ������禡----------------------------------------------
//�Ϊk�G�q�`�u�b�̤W�h����(Machine)�мg���禡�A�ç⥦��@��^�ǨϥΪ������C
//�ѼơG�L
//�^�ǡGLOGINTYPE(��Base.h)
//�`�N�G�L
//-----------------------------------------------------------------------------------------------
MBase::LOGINTYPE MBase::GetLoginType()
{
	if (m_pParent!=NULL)
	{
		return m_pParent->GetLoginType();
	}else{
		return UNDEF;
	}
}
//---------------------------------------�D�`���禡----------------------------------------------
//�Ϊk�G�I�s�Ҧ��U�h����Cycle�Υثe����p�G�bAction��State�N�I�sStepCycle
//�p�G�O�bHome��State�N�I�sHomeCycle�C
//�ѼơGdouble dblTime(�W��Cycle�쥻��Cycle�����j�ɶ��A���Gms)
//�^�ǡG�L
//�`�N�G�q�`���禡�ä����мg�A�p�G�n�мg��Virtual Function���`�N�^�I���h����Cycle
//-----------------------------------------------------------------------------------------------
void MBase::Cycle(const double dblTime)	
{
	int intPreStep,intPreState;
	intPreStep = m_Step;
	intPreState = m_State;
	std::map<CString,MBase *>::iterator itChild;

	if (m_State==STATE::ACTION || m_State == STATE::HOME)
	{
		StepCycle(dblTime);
		if (m_bRecordLog)
		{
			MMessage *pMsg = new MMessage();
			pMsg->MsgType = MMessage::MESSAGETYPE::LogToFile;
			pMsg->strMessage = m_strName + _T(" [") + GetModeName() + _T("] ") + m_strStepName;
			ShowMessage(pMsg);
			m_bRecordLog = false;
		}		
		if (m_bLog)
		{
			if (m_State == STATE::ACTION)
			{
				if (intPreStep != m_Step || m_State!=intPreState) //Step Change
				{
					m_bRecordLog = true;
				}
			}
		}
	}
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->Cycle(dblTime);
	}
}
//---------------------------------------����ʧ@-----------------------------------------------
//�Ϊk�G���ʧ@����������ʧ@���B�J�g�bStepCycle���A�ô��Ѥ@�Ӥ����禡�A�b�禡���N�O�HDoStep�h
//		����Y��StepCycle��Step
//�ѼơGint intStep(�n���檺Step)
//�^�ǡGtrue(���榨�\),false(���楢��)
//�`�N�G�q�`���禡�ä����мg
//-----------------------------------------------------------------------------------------------
bool MBase::DoStep(int intStep)
{
	if (m_State==STATE::IDLE && !m_isInterLock && !m_isEStop)
	{
		m_State=STATE::ACTION;
		m_Step=intStep;
		m_bRecordLog = m_bLog;
		return true;
	}
	return false;
}
//---------------------------------------�_�k�ʧ@-----------------------------------------------
//�Ϊk�G���ݭn�_�k�������_�k�ʧ@���B�J�g�bHomeCycle���A�ô��Ѥ@�Ӥ����禡�A�b�禡���N�O�HDoHome
//		�h����Y��HomeCycle��Step
//�ѼơGint intStep(�n���檺Step)
//�^�ǡGtrue(���榨�\),false(���楢��)
//�`�N�G�q�`���禡�ä����мg
//-----------------------------------------------------------------------------------------------
bool MBase::DoHome(int intStep)
{
	if (m_State==STATE::IDLE && !m_isInterLock && !m_isEStop)
	{
		m_State=STATE::HOME;
		m_Step=intStep;
		return true;
	}
	return false;
}
//---------------------------------------��l�ƪ���----------------------------------------------
//�Ϊk�G���ݭn��l�ƪ�������l���мg�bInit���C
//�ѼơG�L
//�^�ǡGtrue(��l�Ʀ��\),false(��l�ƥ���)
//�`�N�G�p�G�n�мg��Virtual Function���`�N�^�I���h����Init
//-----------------------------------------------------------------------------------------------
bool MBase::Init()
{
	bool bRet=true;
	std::map<CString,MBase *>::iterator itChild;
	m_State=STATE::IDLE;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		if (!p_C->Init())
		{
			ShowMessage(this,p_C->GetFullName()+_T(" Init failure!"),MMessage::MSGIMG::Warn);
			bRet=false;
		}
	}
	return bRet;
}
//---------------------------------------�Ȱ�����ʧ@--------------------------------------------
//�Ϊk�G���禡���Ѫ���Ȱ��������A����O�_�B��i�Ȱ����p�h��GetAllowToPause()�P�_�C
//�ѼơG�L
//�^�ǡGtrue(�Ȱ����\�Τw�B��Ȱ���),false(�ثe���i�Ȱ�)
//�`�N�G�q�`���禡�ä����мg�A�p�G�n�мg����ƶ��`�N�l�h����]�n�Q�Ȱ�
//-----------------------------------------------------------------------------------------------
bool MBase::Pause()
{
	bool bRet=true; 
	std::map<CString,MBase *>::iterator itChild;
	if (m_State!=STATE::PAUSE)
	{
		if (GetAllowToPause() && (m_State==STATE::ACTION || m_State==STATE::HOME || m_State==STATE::IDLE))
		{
			//-------------------�l�h����]�n�Q�Ȱ�-------------------------
			for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
			{
				MBase * p_C=(*itChild).second;
				p_C->Pause();
			}
			m_PauseState=m_State;
			m_State=STATE::PAUSE;
		}
	}
	return (m_State==STATE::PAUSE);
}
//------------------------------------�O�_���\�Ȱ�����ʧ@---------------------------------------
//�Ϊk�G���禡���Ѫ���Ȱ��������A����O�_�B��i�Ȱ����p�h��GetAllowToPause()�P�_�C
//�ѼơG�L
//�^�ǡGtrue(���\�Ȱ�),false(���i�Ȱ�)
//�`�N�G�q�`���禡�ä����мg�A�p�G�n�мg����ƶ��`�N�^�I���h����GetAllowToPause()
//-----------------------------------------------------------------------------------------------
bool MBase::GetAllowToPause()
{
	bool bRet=true; 
	std::map<CString,MBase *>::iterator itChild;
	if (m_State==STATE::ACTION || m_State==STATE::HOME || m_State==STATE::IDLE || m_State==STATE::PAUSE)
	{
		for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
		{
			MBase * p_C=(*itChild).second;
			if (p_C->m_State!=STATE::PAUSE)
			{
				if (!p_C->GetAllowToPause())
				{
					bRet=false;
				}
			}
		}
	}else{ 
		bRet=false;
	}
	return bRet;
}
//----------------------------------------�~��Ȱ�����ʧ@---------------------------------------
//�Ϊk�G���禡���Ѫ����~��Ȱ�����ʧ@�����A�æP�ɩI�s��l�h�����~��C
//�ѼơG�L
//�^�ǡGtrue(�Ȱ����\�Τw�B��Ȱ���),false(�ثe���i�Ȱ�)
//�`�N�G�q�`���禡�ä����мg�A�p�G�n�мg����ƶ��`�N�^�I���h����Continue()
//-----------------------------------------------------------------------------------------------
void MBase::Continue()
{
	std::map<CString,MBase *>::iterator itChild;
	if (m_State==STATE::PAUSE)
	{
		m_State=m_PauseState;
	}
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->Continue();
	}
}
//------------------------------------����`������-----------------------------------------------
//�Ϊk�G����ʧ@�q�`���O�{�Ǵ`���A���禡���ѳQ�~���q������`���B�æP�ɩI�s��l�h���󰱤�`���C
//�ѼơG�L
//�^�ǡG�L
//�`�N�G�q�`���禡�u�Q���W�h�����мg�A�p�G�n�мg����ƶ��`�N�^�I���h����CycleStop()
//-----------------------------------------------------------------------------------------------
void MBase::CycleStop()
{
	std::map<CString,MBase *>::iterator itChild;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->CycleStop();
	}
}
//------------------------------------����`���~��-----------------------------------------------
//�Ϊk�G����ʧ@�q�`���O�{�Ǵ`���A���禡���ѳQ�~���q������`���B�æP�ɩI�s��l�h���󰱤�`���C
//�ѼơG�L
//�^�ǡG�L
//�`�N�G�q�`���禡�u�Q���W�h�����мg�A�p�G�n�мg����ƶ��`�N�^�I���h����CycleStop()
//-----------------------------------------------------------------------------------------------
void MBase::CycleContinue()
{
	std::map<CString,MBase *>::iterator itChild;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->CycleContinue();
	}
}
//------------------------------------�R���l�h����-----------------------------------------------
//�Ϊk�G���禡�D�n�O�C�|�X�Ҧ��l�h����çR���C
//�ѼơG�L
//�^�ǡG�L
//�`�N�G���禡�D�n����MBase�Ѻc�ɥΡA�����мg
//-----------------------------------------------------------------------------------------------
void MBase::ReleaseChilds()
{
	std::map<CString,MBase *>::iterator itChild;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->ReleaseChilds();
		delete p_C;
	}
	m_Childs.clear();
}
//------------------------------------�[�J�ѨM���-----------------------------------------------
//�Ϊk�G���禡�q�`���ѵ����h��I�s���h�Υ��h���ݤl�h�ʧ@�A�Y�o�Ϳ��~�ɱa�X�C�Ҫ��[���ѨM��׷|�@��
//		��Q�I�sRemoveSolutions()��Stop()��~�|�����C
//�ѼơGMSolution *pS(�n���[���ѨM��׫���)
//�^�ǡG�L
//�`�N�G���禡�D�n���Ѧ@�Υ\��A�����мg
//-----------------------------------------------------------------------------------------------
void MBase::AddSolution(MSolution *pS)
{
	m_ParentSolutions.push_back(pS);
}
//------------------------------------���[�ѨM��׵����~����-------------------------------------
//�Ϊk�G���禡�q�`�b�o�Ϳ��~�ɡA�Ψӧ�W�h���Ѫ��ѨM��ת��[����~����A�ϥΩ�ErrorHappen()�C
//�ѼơGMError * pError(�n�Q���[�����~�������)
//�^�ǡG�L
//�`�N�G���禡�D�n����MBase�ϥΡA�����мg
//-----------------------------------------------------------------------------------------------
void MBase::AddSolutionsTo(MError * pError)
{
	MSolution *pSolution,*pS;
	std::vector<MSolution*>::iterator itSolution;
	if (!m_ParentSolutions.empty())
	{
		for(itSolution=m_ParentSolutions.begin();itSolution!=m_ParentSolutions.end();itSolution++)
		{
			pSolution=*itSolution;
			pS=new MSolution(pSolution);
			pError->AddSolution(pS);
		}
	}
}
//------------------------------------�����Q���[���ѨM���---------------------------------------
//�Ϊk�G���禡�q�`�b���h�I�s�ʧ@�e�|�A���Ѫ��ѨM��סA�Y�O�����ʧ@�᥼�޵o���~�A�ΨӲ����ʧ@�e��
//		���[���ѨM��סC
//�ѼơG�L
//�^�ǡG�L
//�`�N�G���禡�D�n���Ѧ@�Υ\��A�����мg
//-----------------------------------------------------------------------------------------------
void MBase::RemoveSolutions()
{
	MSolution *pSolution;
	std::vector<MSolution*>::iterator itSolution;
	if (!m_ParentSolutions.empty())
	{
		for(itSolution=m_ParentSolutions.begin();itSolution!=m_ParentSolutions.end();itSolution++)
		{
			pSolution=*itSolution;
			delete pSolution;
		}
		m_ParentSolutions.clear();
	}
}
//------------------------------------�j��󰱤�ʧ@-------------------------------------------
//�Ϊk�G�j��ثe���A�אּIDLE�éI�s�Ҧ��l�hStop()�A�B�����ʧ@�e�Ҫ��[���ѨM��סC
//�ѼơG�L
//�^�ǡG�L
//�`�N�G�p�G�n�мg���禡�ɡA�����`�N�^�I���h����Stop
//-----------------------------------------------------------------------------------------------
void MBase::Stop()
{	
	std::map<CString,MBase *>::iterator itChild;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->Stop();
	}
	RemoveSolutions();	//�M���W�hSolution
	m_State=STATE::IDLE;
}
//------------------------------------�氱(�o��/�Ѱ�)--------------------------------------------
//�Ϊk�G�j��ثe���A�אּEMSTOP��IDLE�éI�s�Ҧ��l�hEStop�A�B�����ʧ@�e�Ҫ��[���ѨM��סC
//�ѼơGbool isES(true��氱�o��,false��氱�Ѱ�)
//�^�ǡG�L
//�`�N�G�p�G�n�мg���禡�ɡA�����`�N�^�I���h����EStop
//-----------------------------------------------------------------------------------------------
void MBase::EStop(const bool isES)
{	
	std::map<CString,MBase *>::iterator itChild;
	m_isEStop=isES;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->EStop(isES);
	}
	if (isES)
	{
		m_State=STATE::EMSTOP;
		RemoveSolutions();  //�M���W�hSolution
	}else{
		m_State=STATE::IDLE;
	}
}
//------------------------------------��w(�o��/�Ѱ�)--------------------------------------------
//�Ϊk�G�O����w�o�ͩθѰ��éI�s�Ҧ��l�hInterLock�C
//�ѼơGbool isInterLock(true����w�o��,false����w�Ѱ�)
//�^�ǡG�L
//�`�N�G�p�G�n�мg���禡�ɡA�����`�N�^�I���h����InterLock
//-----------------------------------------------------------------------------------------------
void MBase::InterLock(const bool isInterLock)
{
	
	std::map<CString,MBase *>::iterator itChild;
	m_isInterLock=isInterLock;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->InterLock(isInterLock);
	}
}
//------------------------------------���ثe�ϥΪ��y��-----------------------------------------
//�Ϊk�G�O���y���ϥΤ���έ^��éI�s�Ҧ��l�hChangeLanguage�C
//�ѼơGbool isEnglish(true��^��,false����)
//�^�ǡG�L
//�`�N�G�p�G�n�мg���禡�ɡA�����`�N�^�I���h����ChangeLanguage
//-----------------------------------------------------------------------------------------------
void MBase::ChangeLanguage()
{
	std::map<CString,MBase *>::iterator itChild;
	for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
	{
		MBase * p_C=(*itChild).second;
		p_C->ChangeLanguage();
	}
	if (m_pStringProvider!= NULL)
	{
		m_strName = m_pStringProvider->GetString(_T("Name"), m_ID, m_strName);
	}
}
//---------------------------�Ǧ^�ثe����Ψ�l����O�_���B��IDLE--------------------------------
//�Ϊk�G�O���y���ϥΤ���έ^��éI�s�Ҧ��l�hChangeLanguage�C
//�ѼơG�L
//�^�ǡGtrue(�ثe���bIDLE),false(�ثe�����bIDLE)
//�`�N�G���禡�D�n���Ѧ@�Υ\��A�����мg
//-----------------------------------------------------------------------------------------------
bool MBase::isIDLE()
{
	bool ret=true;
	std::map<CString,MBase *>::iterator itChild;
	if (m_State==STATE::IDLE && !m_isInterLock && !m_isEStop)
	{
		for (itChild=m_Childs.begin();itChild!=m_Childs.end();++itChild)
		{
			MBase * p_C=(*itChild).second;
			if(!p_C->isIDLE())
			{
				ret=false;
				break;
			}
		}
	}else{
		ret=false;
	}
	return(ret);
}
//----------------------------------------�[�J�l�h����-------------------------------------------
//�Ϊk�G�[�J�@�Ӥl�h����쥻�h�C
//�ѼơGMBase * p_C(�n�[�J���l�����)
//�^�ǡG�Q�[�J���l�����
//�`�N�G���禡�D�n���Ѧ@�Υ\��A�����мg
//-----------------------------------------------------------------------------------------------
MBase* MBase::AddChild(MBase * p_C)
{
	m_Childs.insert(std::map<CString,MBase*>::value_type(p_C->m_ID,p_C));
	p_C->m_ErrorCodeBase = m_Childs.size()*ERROR_CODE_SPACE + m_ErrorCodeBase;
	return(p_C);
}
//----------------------------------------���o�l�h����-------------------------------------------
//�Ϊk�G���o���l�h����N�X��ID��������СC
//�ѼơGCString ID(�n���o���l��N�X)
//�^�ǡG�Y���n���o���l��N�X�N�Ǧ^�Ӥl����СA�_�h�Ǧ^NULL
//�`�N�G���禡�D�n���Ѧ@�Υ\��A�����мg
//-----------------------------------------------------------------------------------------------
MBase * MBase::GetChild(CString ID)
{
	std::map<CString,MBase *>::iterator itChild;
	itChild=m_Childs.find(ID);
	if(itChild!=m_Childs.end())
	{
		return((*itChild).second);
	}
	return(NULL);	
}
//----------------------------------------�R���l�h����-------------------------------------------
//�Ϊk�G��l�h����N�X��ID������q�l�h�R��(�D�Ѻc)�C
//�ѼơGCString ID(�n�R�����l��N�X)
//�^�ǡGtrue(���n�R�����l��N�X�A�çR�����\),false(�䤣��N�X)
//�`�N�G���禡�D�n���Ѧ@�Υ\��A�����мg
//-----------------------------------------------------------------------------------------------
bool MBase::DelChild(CString ID)
{
	std::map<CString,MBase *>::iterator itChild;
	itChild=m_Childs.find(ID);
	if(itChild!=m_Childs.end())
	{
		m_Childs.erase(ID);
		return(true);
	}
	return(false);	
}
//------------------------------------------���~�o��---------------------------------------------
//�Ϊk�G�Y�޵o���~�����󬰥�����h�⪬�A�אּERRHAPPEN�A��W�h�Ҫ��[���ѨM��ץ[����~�é����h�޵o
//		���~�C
//�ѼơGMError * pError(�o�ͪ����~����)
//�^�ǡG�L
//�`�N�G
//-----------------------------------------------------------------------------------------------
void MBase::ErrorHappen(MError * pError)
{
	if (pError->m_Happener==this)
	{
		m_State=STATE::ERRHAPPEN;
	}
	if (m_pParent!=NULL) {
		AddSolutionsTo(pError);				//�[�J�W�h�ѨM���
		m_pParent->ErrorHappen(pError);		
	}
}
//------------------------------------------���~�B�z---------------------------------------------
//�Ϊk�G�B�z�@�ӸѨM��סC
//�ѼơGMSolution *pSolution(�n�B�z���ѨM���)
//�^�ǡG�L
//�`�N�G
//-----------------------------------------------------------------------------------------------
void MBase::ErrorProcess(STATE state,int intStep,int intMode)
{
	if (!m_isEStop)
	{
		m_State = (STATE)state;
		m_Step = intStep;
		m_Mode = (MODE)(intMode);
	}
}
//------------------------------------------�d�ߦw��---------------------------------------------
//�Ϊk�G�D�n�Ω�ʧ@�e�d�߬O�_���w���C
//�ѼơGMBase *pFrom(�n�ʧ@���������)
//		int intState(�n����ʧ@�����A�G�q�`�OACTION��HOME)
//		int intStep(�n����ʧ@��Step)
//		int intMode(�n����ʧ@��mode)
//		CString *pstrCDiscript(��d�ߤ��w���ɡA���ܻ���������r��)
//		CString *pstrEDiscript(��d�ߤ��w���ɡA���ܻ������^��r��)
//�^�ǡGtrue(�w��),false(���w��)
//�`�N�G�p�G�n�мg���禡�ɡA�����`�N�^�I���h����QuerySafe
//-----------------------------------------------------------------------------------------------
bool MBase::QuerySafe(MBase *pFrom,int intState,int intStep,int intMode,CString *pstrReason)
{
	if (m_pParent!=NULL) {
		return m_pParent->QuerySafe(pFrom,intState,intStep,intMode, pstrReason);		//�V�W�h�߰ݦw��
	}
	return true;
}
//------------------------------------------�T���q��(�r�ꫬ��)-----------------------------------
//�Ϊk�G�D�n�Ω��̤W�h����q���T���A�̤W�h�A��T��Post��Mainfrm�h�B�z�C
//�ѼơGMBase *pFrom(�o�X�T�����������)
//		CString strMsg(�T�����e)
//		MMessage::MSGIMG img(�T�����O)
//�^�ǡG�L
//�`�N�G���禡�D�n���Ѧ@�Υ\��A�����мg
//-----------------------------------------------------------------------------------------------
void MBase::ShowMessage(MBase *pFrom,CString strMsg,MMessage::MSGIMG img)
{
	if (m_pParent!=NULL) {
		m_pParent->ShowMessage(pFrom,strMsg,img);
	}
}
//------------------------------------------�T���q��(���󫬦�)------------------------------------
//�Ϊk�G�D�n�Ω��̤W�h����q���T���A�̤W�h�A��T��Post��Mainfrm�h�B�z�C
//�ѼơGMMessage* pMsg(�n�q�����T���������)
//�^�ǡG�L
//�`�N�G���禡�D�n���Ѧ@�Υ\��A�����мg
//-----------------------------------------------------------------------------------------------

void MBase::ShowMessage(MMessage* pMsg)
{
	if (m_pParent!=NULL)
	{
		m_pParent->ShowMessage(pMsg);
	}else{
		delete pMsg;
	}
}
//------------------------------------------���o���󧹾�W��-------------------------------------
//�Ϊk�G�D�n�Ω��̤W�h����q���T���A�̤W�h�A��T��Post��Mainfrm�h�B�z�C
//�ѼơGMMessage* pMsg(�n�q�����T���������)
//�^�ǡG�L
//�`�N�G���禡�D�n���Ѧ@�Υ\��A�����мg
//-----------------------------------------------------------------------------------------------
CString MBase::GetFullName()
{
	CString strRet;
	strRet=m_strName;
	if(m_pParent!=NULL && m_pParent->GetFullName() !=_T(""))
	{
		strRet=m_pParent->GetFullName()+"_"+strRet;
	}
	return strRet;
}

//--------------------------------------���J���x�Ѽ�---------------------------------------------
//�Ϊk�G�p�G���󦳾��x�Ѽƭn�ۦ�s�����мg���禡�A���禡�ѳ̤W�h����}�l�I�s�C
//�ѼơGCADOConnection * p_C(�s����x�Ѽƪ���Ʈw�s�u����)
//�^�ǡG�L
//�`�N�G�p�G�n�мg���禡�ɡA�����`�N�^�I���h����LoadMachineData
//�Ƶ��G���x�ѼƬO���|�H���P���x���ܡA�Ӥ��H���~�������ܪ���ơA�Ҧp�G��Ǧ�
//-----------------------------------------------------------------------------------------------
void MBase::LoadMachineData(CADOConnection * p_C,bool bAllChildsLoad)
{
	if (bAllChildsLoad)
	{
		std::map<CString, MBase *>::iterator itChild;
		for (itChild = m_Childs.begin();itChild != m_Childs.end();++itChild)
		{
			MBase * pChild = (*itChild).second;
			pChild->LoadMachineData(p_C,bAllChildsLoad);
		}
	}
}
//--------------------------------------�x�s���x�Ѽ�---------------------------------------------
//�Ϊk�G�p�G���󦳾��x�Ѽƭn�ۦ�s�����мg���禡�A���禡�ѳ̤W�h����}�l�I�s�C
//�ѼơGCADOConnection * p_C(�s����x�Ѽƪ���Ʈw�s�u����)
//�^�ǡG�L
//�`�N�G�p�G�n�мg���禡�ɡA�����`�N�^�I���h����SaveMachineData
//�Ƶ��G���x�ѼƬO���|�H���P���x���ܡA�Ӥ��H���~�������ܪ���ơA�Ҧp�G��Ǧ�
//-----------------------------------------------------------------------------------------------
void MBase::SaveMachineData(CADOConnection * p_C,bool bAllChildsSave)
{
	if (bAllChildsSave)
	{
		std::map<CString, MBase *>::iterator itChild;
		for (itChild = m_Childs.begin();itChild != m_Childs.end();itChild++)
		{
			MBase * pChild = (*itChild).second;
			pChild->SaveMachineData(p_C, bAllChildsSave);
		}
	}
}

//--------------------------------------���J���~�Ѽ�---------------------------------------------
//�Ϊk�G�p�G���󦳲��~�Ѽƭn�ۦ�s�����мg���禡�A���禡�ѳ̤W�h����}�l�I�s�C
//�ѼơGCADOConnection * p_C(�s�񲣫~�Ѽƪ���Ʈw�s�u����)
//�^�ǡG�L
//�`�N�G�p�G�n�мg���禡�ɡA�����`�N�^�I���h����LoadRecipeData
//�Ƶ��G���~�ѼƬO���|�H���P���~�������ܡA�Ӥ��H���x���ܪ���ơA�Ҧp�G���~���B�e�B�p�ι��O��
//		�y�е�
//-----------------------------------------------------------------------------------------------
void MBase::LoadRecipeData(CADOConnection * pC,bool bAllChildsLoad)
{
	if (bAllChildsLoad)
	{
		std::map<CString, MBase *>::iterator itChild;
		for (itChild = m_Childs.begin();itChild != m_Childs.end();itChild++)
		{
			MBase * p_C = (*itChild).second;
			p_C->LoadRecipeData(pC, bAllChildsLoad);
		}
	}
}

//--------------------------------------�x�s���~�Ѽ�---------------------------------------------
//�Ϊk�G�p�G���󦳲��~�Ѽƭn�ۦ�s�����мg���禡�A���禡�ѳ̤W�h����}�l�I�s�C
//�ѼơGCADOConnection * p_C(�s�񲣫~�Ѽƪ���Ʈw�s�u����)
//�^�ǡG�L
//�`�N�G�p�G�n�мg���禡�ɡA�����`�N�^�I���h����SaveRecipeData
//�Ƶ��G���~�ѼƬO���|�H���P���~�������ܡA�Ӥ��H���x���ܪ���ơA�Ҧp�G���~���B�e�B�p�ι��O��
//		�y�е�
//-----------------------------------------------------------------------------------------------
void MBase::SaveRecipeData(CADOConnection * pC,bool bAllChildsSave)
{
	if (bAllChildsSave)
	{
		std::map<CString, MBase *>::iterator itChild;
		for (itChild = m_Childs.begin();itChild != m_Childs.end();itChild++)
		{
			MBase * p_C = (*itChild).second;
			p_C->SaveRecipeData(pC, bAllChildsSave);
		}
	}
}

//-------------------------------���o�̤W�h����(�@��OMachine)-----------------------------------
//�Ϊk�G�p�G����n���o�̤W�h������дN�H���禡�I�s�C
//�ѼơG�L
//�^�ǡGMBase *(�̤W�h�������)
//�`�N�G���禡�D�n���Ѧ@�Υ\��A�����мg
//-----------------------------------------------------------------------------------------------
MBase * MBase::GetTopParent()
{
	if (m_pParent != NULL)
	{
		return(m_pParent->GetTopParent());
	}
	else
	{
		return(this);
	}
}
//-------------------------------���o�W�h����-----------------------------------
//�Ϊk�G�p�G����n���o�̤W�h������дN�H���禡�I�s�C
//�ѼơG�L
//�^�ǡGMBase *(�̤W�h�������)
//�`�N�G���禡�D�n���Ѧ@�Υ\��A�����мg
//-----------------------------------------------------------------------------------------------
MBase * MBase::GetParent()
{
		return(m_pParent);
}
CString MBase::GetStateName()
{
	CString strRet[9] = { _T("INIT"),
			_T("IDLE"),
			_T("ACTION"),
			_T("HOME"),
			_T("ERROR"),
			_T("ESTOP"),
			_T("PAUSE"),
			_T("SUSPEND"),
			_T("RESUME") };
	return strRet[m_State];
}
CString MBase::GetStepName()
{
	if (m_State == STATE::ACTION || m_State == STATE::HOME)
	{
		return m_strStepName;
	}
	return _T("");
}
CString MBase::GetModeName()
{
	CString strRet[4] = { _T("BASE"),
		_T("AUTO"),
		_T("MANUAL"),
		_T("MAINTAIN")};
	return strRet[m_Mode];
}
CStringProvider* MBase::GetStringProvider()
{
	return m_pStringProvider;
}
void MBase::SetLog(bool bLog)
{
	std::map<CString, MBase *>::iterator itChild;
	m_bLog = bLog;
	for (itChild = m_Childs.begin(); itChild != m_Childs.end(); itChild++)
	{
		MBase * p_C = (*itChild).second;
		p_C->SetLog(bLog);
	}
}
// MBase �T���B�z�`��
