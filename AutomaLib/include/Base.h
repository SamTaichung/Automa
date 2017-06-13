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
	AUTOMALIB_API MBase(MBase *pParent,CString strID, CString strName);					//英文名字同ID
	//----------------------------------------------------------------------------------------
	enum STATE{		//物件狀態
		INIT,			//初始中
		IDLE,			//動作完成
		ACTION,			//m_Step     動作中
		HOME,			//m_HomeStep 動作中
		ERRHAPPEN,		//錯誤引發中
		EMSTOP,			//急停中
		PAUSE,			//暫停中
		SUSPEND,		//暫時不掃描StepCycle
		RESUME,			//重新執行StepCycle
	};

	enum MODE{		//物件操作模式
		BASE,			//基楚元件
		AUTO,			//自動
		SEMIAUTO,		//半自動
		MANUAL,			//手動
		MAINTAIN,		//維護
	};

	enum LOGINTYPE{
		UNDEF,
		OPERATOR,
		ENGINEER,
		MAKER,
	};
	
	//----------------------------------------------------------------------------------------
	AUTOMALIB_API virtual ~MBase();					//解構
	AUTOMALIB_API virtual bool Init();				//機構初始化,當讀完Machine Data後應該要初始化機構
	AUTOMALIB_API virtual void ErrorHappen(MError * pError);				//錯誤異常丟出
	AUTOMALIB_API virtual void ErrorProcess(STATE state, int intStep, int intMode);			//錯誤處理
	AUTOMALIB_API virtual bool QuerySafe(MBase * pFrom,int intState,int intStep,int intMode,CString *pstrReason=NULL);
																			//往上層查詢動作安全
	AUTOMALIB_API virtual void AddSolution(MSolution *pS);
	AUTOMALIB_API virtual void AddSolutionsTo(MError * pError);				//把上層Solution加給Error
	AUTOMALIB_API virtual void RemoveSolutions();							//清除上層Slution
	AUTOMALIB_API virtual void Cycle(const double dblTime);					//系統Cycle
	AUTOMALIB_API virtual void StepCycle(const double dblTime){};			//Step執行，由各層實作
	AUTOMALIB_API virtual bool DoStep(int intStep);							//開始執行Step，由各層實作
	AUTOMALIB_API virtual bool DoHome(int intStep);							//開始執行Home，由各層實作
	AUTOMALIB_API virtual bool isIDLE();									//物件及其子物件是否動作完成
	AUTOMALIB_API virtual void EStop(const bool isES);						//急停通知
	AUTOMALIB_API virtual void InterLock(const bool isInterLock);			//鎖定通知
	AUTOMALIB_API virtual void Stop();
	AUTOMALIB_API virtual void CycleStop();
	AUTOMALIB_API virtual void CycleContinue();
	AUTOMALIB_API virtual bool GetAllowToPause();							//是否可以暫停動作
	AUTOMALIB_API virtual bool Pause();										//暫停動作
	AUTOMALIB_API virtual void Continue();									//Continue動作
	AUTOMALIB_API virtual void ChangeLanguage();							//語言切換通知
	AUTOMALIB_API virtual void ShowMessage(MBase * pFrom,CString strMsg,MMessage::MSGIMG img=MMessage::MSGIMG::Information);
	AUTOMALIB_API virtual void ShowMessage(MMessage* pMsg); //傳送訊息及
	AUTOMALIB_API virtual void LoadRecipeData(CADOConnection * pC,bool bAllChildsLoad=true);
	AUTOMALIB_API virtual void SaveRecipeData(CADOConnection * pC, bool bAllChildsSave = true);
	AUTOMALIB_API virtual void LoadMachineData(CADOConnection * p_C, bool bAllChildsLoad = true);
	AUTOMALIB_API virtual void SaveMachineData(CADOConnection * p_C, bool bAllChildsSave = true);
	AUTOMALIB_API virtual CString GetStateName();					//取得目前此物件的狀態名稱
	AUTOMALIB_API virtual CString GetStepName();					//取得目前此物件的步序名稱
	AUTOMALIB_API virtual CString GetModeName();					//取得目前此物件的模式名稱
	AUTOMALIB_API virtual bool GetErrorHappen(); //此機構(含其子項)是否處於錯誤狀態中
	AUTOMALIB_API virtual CString GetFullName();
	AUTOMALIB_API virtual LOGINTYPE GetLoginType();

	AUTOMALIB_API MBase* AddChild(MBase * p_C);	//加入下層物件
	AUTOMALIB_API bool   DelChild(CString ID);	//刪除下層物件
	AUTOMALIB_API MBase* GetChild(CString ID);	//取得下層物件
	AUTOMALIB_API MBase* GetTopParent();		//取得最上層物件(一般是Machine)
	AUTOMALIB_API MBase* GetParent();			//取得此物件上層物件
	AUTOMALIB_API void ReleaseChilds();			//delete所有下層物件(一般是解構時用)
	AUTOMALIB_API CStringProvider* GetStringProvider();	//取得文字庫物件
	AUTOMALIB_API void SetLog(bool bLog);		//設定Log記錄器
	CString m_ID;								//參數存檔的代號
	CString m_strName;							//此機構名稱
	int m_Step;									//此物件目前執行動作的步序號碼
	std::map<CString,MBase *> m_Childs;			//此物件的子物件集合
	STATE m_State;								//此物件的狀態
	MODE m_Mode;								//此物件目前執行動作的模式
protected:
	MBase * m_pParent;								//此物件之上層物件
	bool m_bLog;								//動作是否記錄到LOG檔
	int m_ErrorCodeBase;						//此物件的錯誤碼基底
	STATE m_PauseState;							//此物件暫停時的狀態
	bool m_isInterLock,m_isEStop;				//鎖定,急停 Latch用的變數
	CString m_strStepName;						//目前執行步驟的名稱
	CStringProvider* m_pStringProvider;			//字串索引用物件:主要是翻不同國文字用
	std::vector<MSolution *> m_ParentSolutions; //上層加入的Solution集合
	bool m_bRecordLog;
	CString m_strRecordMsg;
};
}//namespace AutoLib

