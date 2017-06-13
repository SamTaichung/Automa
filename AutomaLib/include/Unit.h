#pragma once

#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
// MCard
#include "Base.h"				
namespace AutoLib{
	AUTOMALIB_API class MUnit : public MBase
	{
	public:
		enum AutoRunMode {
			DryRun,		//空跑模式
			NormalRun,	//正常模式
		};
		AUTOMALIB_API MUnit(MUnit *pParent,CString strID,CString strName);
		AUTOMALIB_API virtual ~MUnit();
		AUTOMALIB_API virtual bool AutoStart();
		AUTOMALIB_API virtual void AutoStop();
		AUTOMALIB_API virtual void SetAutoRunMode(AutoRunMode mode);

	protected:
		bool m_bAutoEnd;	//自動生產結束
		CArray<MUnit*> m_Units;
		std::map<void *,CString> m_mapDataName; //參數指標對應名稱
		AutoRunMode m_AutoRunMode;
	};

}//namespace AutoLib
