#pragma once
#pragma warning( disable :4091)

#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
#include <map>
namespace AutoLib {
	class MBase;
	AUTOMALIB_API class MMessage
	{
	public:
		AUTOMALIB_API MMessage(void);
		AUTOMALIB_API virtual ~MMessage(void);
		enum MESSAGETYPE
		{
			StepLog,			//�B�ǰO��
			ShowError,			//��ܿ��~
			ShowWarning,		//���ĵ�i
			ShowMessage,		//��ܰT��
			MachineComplete,	//���x�{���Ұʧ���
			Connection,			//�t�αҰʧ����A�I�s�g��s�u
			UserMessage,		//�ϥΪ̦ۭqMessage
			ChangeLanguage,		//�t�Χ��y��
			ActionComplete,		//�ʧ@����
			HomeStart,			//�_�k�}�l
			HomeComplete,		//�_�k����
			HomeFail,			//�_�k����
			EM_Stop,			//�氱�q��
			AutoCycStop,		//�۰ʵ{�ǰ���
			ChangeType,			//��﫬���q��
			PassWord,			//�K�X���
			UserLogin,			//�ϥΪ̵n�J���\
		} MsgType;

		enum MSGIMG
		{
			Information,
			Notify,
			Warn,
			Normal,
		};

		LPVOID			m_pPara;	//�]�w�ϥέn�a������
		int				m_index;	//�]�w�ϥΪ̦۩w��index�A��msg Type�OUserMessage�ɡA�i�Φ���ۭq�T�����A
		CString			strMessage;
		MBase			*pFrom;
		MSGIMG			MsgLevel;
		UINT			uMessage;
		
		MMessage operator =(MMessage Msg)
		{
			MsgType=Msg.MsgType;
			m_pPara=Msg.m_pPara;
			m_index=Msg.m_index;
			MsgLevel=Msg.MsgLevel;
			pFrom=Msg.pFrom;
			strMessage=Msg.strMessage;
			return *this;
		};
	};
}//namespace AutoLib