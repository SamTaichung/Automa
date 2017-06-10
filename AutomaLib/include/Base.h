#pragma once
#include <map>
#include "Error.h"
#include "StringProvider.h"
#include "ADOConnection.h"
#include "ADORecordset.h"
#include "Message.h"
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
#define ERROR_CODE_SPACE 10000
namespace AutoLib{
AUTOMALIB_API class MBase:public CCmdTarget
{
public:
	AUTOMALIB_API MBase(MBase *pParent,CString strID, CString strName);					//�^��W�r�PID
	//----------------------------------------------------------------------------------------
	enum STATE{		//���󪬺A
		INIT,			//��l��
		IDLE,			//�ʧ@����
		ACTION,			//m_Step     �ʧ@��
		HOME,			//m_HomeStep �ʧ@��
		ERRHAPPEN,		//���~�޵o��
		EMSTOP,			//�氱��
		PAUSE,			//�Ȱ���
		SUSPEND,		//�Ȯɤ����yStepCycle
		RESUME,			//���s����StepCycle
	};

	enum MODE{		//����ާ@�Ҧ�
		BASE,			//�򷡤���
		AUTO,			//�۰�
		SEMIAUTO,		//�b�۰�
		MANUAL,			//���
		MAINTAIN,		//���@
	};

	enum LOGINTYPE{
		UNDEF,
		OPERATOR,
		ENGINEER,
		MAKER,
	};
	
	//----------------------------------------------------------------------------------------
	AUTOMALIB_API virtual ~MBase();					//�Ѻc
	AUTOMALIB_API virtual bool Init();				//���c��l��,��Ū��Machine Data�����ӭn��l�ƾ��c
	AUTOMALIB_API virtual void ErrorHappen(MError * pError);				//���~���`��X
	AUTOMALIB_API virtual void ErrorProcess(STATE state, int intStep, int intMode);			//���~�B�z
	AUTOMALIB_API virtual bool QuerySafe(MBase * pFrom,int intState,int intStep,int intMode,CString *pstrReason=NULL);
																			//���W�h�d�߰ʧ@�w��
	AUTOMALIB_API virtual void AddSolution(MSolution *pS);
	AUTOMALIB_API virtual void AddSolutionsTo(MError * pError);				//��W�hSolution�[��Error
	AUTOMALIB_API virtual void RemoveSolutions();							//�M���W�hSlution
	AUTOMALIB_API virtual void Cycle(const double dblTime);					//�t��Cycle
	AUTOMALIB_API virtual void StepCycle(const double dblTime){};			//Step����A�ѦU�h��@
	AUTOMALIB_API virtual bool DoStep(int intStep);							//�}�l����Step�A�ѦU�h��@
	AUTOMALIB_API virtual bool DoHome(int intStep);							//�}�l����Home�A�ѦU�h��@
	AUTOMALIB_API virtual bool isIDLE();									//����Ψ�l����O�_�ʧ@����
	AUTOMALIB_API virtual void EStop(const bool isES);						//�氱�q��
	AUTOMALIB_API virtual void InterLock(const bool isInterLock);			//��w�q��
	AUTOMALIB_API virtual void Stop();
	AUTOMALIB_API virtual void CycleStop();
	AUTOMALIB_API virtual void CycleContinue();
	AUTOMALIB_API virtual bool GetAllowToPause();							//�O�_�i�H�Ȱ��ʧ@
	AUTOMALIB_API virtual bool Pause();										//�Ȱ��ʧ@
	AUTOMALIB_API virtual void Continue();									//Continue�ʧ@
	AUTOMALIB_API virtual void ChangeLanguage();							//�y�������q��
	AUTOMALIB_API virtual void ShowMessage(MBase * pFrom,CString strMsg,MMessage::MSGIMG img=MMessage::MSGIMG::Information);
	AUTOMALIB_API virtual void ShowMessage(MMessage* pMsg); //�ǰe�T����
	AUTOMALIB_API virtual void LoadRecipeData(CADOConnection * pC,bool bAllChildsLoad=true);
	AUTOMALIB_API virtual void SaveRecipeData(CADOConnection * pC, bool bAllChildsSave = true);
	AUTOMALIB_API virtual void LoadMachineData(CADOConnection * p_C, bool bAllChildsLoad = true);
	AUTOMALIB_API virtual void SaveMachineData(CADOConnection * p_C, bool bAllChildsSave = true);
	AUTOMALIB_API virtual CString GetStateName();					//���o�ثe�����󪺪��A�W��
	AUTOMALIB_API virtual CString GetStepName();					//���o�ثe�����󪺨B�ǦW��
	AUTOMALIB_API virtual CString GetModeName();					//���o�ثe�����󪺼Ҧ��W��
	AUTOMALIB_API virtual bool GetErrorHappen(); //�����c(�t��l��)�O�_�B����~���A��
	AUTOMALIB_API virtual CString GetFullName();
	AUTOMALIB_API virtual LOGINTYPE GetLoginType();

	AUTOMALIB_API MBase* AddChild(MBase * p_C);	//�[�J�U�h����
	AUTOMALIB_API bool   DelChild(CString ID);	//�R���U�h����
	AUTOMALIB_API MBase* GetChild(CString ID);	//���o�U�h����
	AUTOMALIB_API MBase* GetTopParent();		//���o�̤W�h����(�@��OMachine)
	AUTOMALIB_API MBase* GetParent();			//���o������W�h����
	AUTOMALIB_API void ReleaseChilds();			//delete�Ҧ��U�h����(�@��O�Ѻc�ɥ�)
	AUTOMALIB_API CStringProvider* GetStringProvider();	//���o��r�w����
	AUTOMALIB_API void SetLog(bool bLog);		//�]�wLog�O����
	CString m_ID;								//�ѼƦs�ɪ��N��
	CString m_strName;							//�����c�W��
	int m_Step;									//������ثe����ʧ@���B�Ǹ��X
	std::map<CString,MBase *> m_Childs;			//�����󪺤l���󶰦X
	STATE m_State;								//�����󪺪��A
	MODE m_Mode;								//������ثe����ʧ@���Ҧ�
protected:
	MBase * m_pParent;								//�����󤧤W�h����
	bool m_bLog;								//�ʧ@�O�_�O����LOG��
	int m_ErrorCodeBase;						//�����󪺿��~�X��
	STATE m_PauseState;							//������Ȱ��ɪ����A
	bool m_isInterLock,m_isEStop;				//��w,�氱 Latch�Ϊ��ܼ�
	CString m_strStepName;						//�ثe����B�J���W��
	CStringProvider* m_pStringProvider;			//�r����ޥΪ���:�D�n�O½���P���r��
	std::vector<MSolution *> m_ParentSolutions; //�W�h�[�J��Solution���X
	bool m_bRecordLog;
	CString m_strRecordMsg;
};
}//namespace AutoLib

