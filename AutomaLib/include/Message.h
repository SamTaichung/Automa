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
			StepLog,			//步序記錄
			ShowError,			//顯示錯誤
			ShowWarning,		//顯示警告
			ShowMessage,		//顯示訊息
			MachineComplete,	//機台程式啟動完成
			Connection,			//系統啟動完成，呼叫週邊連線
			UserMessage,		//使用者自訂Message
			ChangeLanguage,		//系統更改語言
			ActionComplete,		//動作完成
			HomeStart,			//復歸開始
			HomeComplete,		//復歸完成
			HomeFail,			//復歸失敗
			EM_Stop,			//急停通知
			AutoCycStop,		//自動程序停止
			ChangeType,			//更改型號通知
			PassWord,			//密碼驗証
			UserLogin,			//使用者登入成功
		} MsgType;

		enum MSGIMG
		{
			Information,
			Notify,
			Warn,
			Normal,
		};

		LPVOID			m_pPara;	//設定使用要帶的物件
		int				m_index;	//設定使用者自定的index，當msg Type是UserMessage時，可用此欄自訂訊息型態
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