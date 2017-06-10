#pragma once
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
#include "Base.h"	
// MLock
namespace AutoLib {
	AUTOMALIB_API class MLock:public CCmdTarget
	{
	public:
		AUTOMALIB_API MLock();
		AUTOMALIB_API virtual ~MLock();
		AUTOMALIB_API virtual bool Lock(CObject *pLocker);
		AUTOMALIB_API virtual void UnLock(CObject *pLocker);
		AUTOMALIB_API virtual CObject* GetLocker();
	protected:
		CRITICAL_SECTION m_csLock;
		CObject *m_pLocker;
	};

}
