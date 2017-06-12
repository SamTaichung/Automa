#pragma once
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
// MCard
#include "Base.h"				
namespace AutoLib{
	class MIO;
	AUTOMALIB_API class MIOLib : public MBase
	{
	public:
		AUTOMALIB_API MIOLib(MBase *pParent,CString strID,CString strName);
		AUTOMALIB_API virtual ~MIOLib();
		AUTOMALIB_API virtual bool GetValue(MIO* pIO);
		AUTOMALIB_API virtual void SetValue(MIO* pIO, bool bValue);
	protected:
	};

}//namespace AutoLib
