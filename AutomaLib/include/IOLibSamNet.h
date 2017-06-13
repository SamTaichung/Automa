#pragma once
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
#include "IOLib.h"				
namespace AutoLib{
	class MIO;
	AUTOMALIB_API class MIOLibSamNet : public MIOLib
	{
	public:
		AUTOMALIB_API MIOLibSamNet(MIOLib *pParent,CString strID,CString strName);
		AUTOMALIB_API virtual ~MIOLibSamNet();
		AUTOMALIB_API virtual bool GetValue(MIO* pIO);
		AUTOMALIB_API virtual void SetValue(MIO* pIO, bool bValue);
	protected:
	};

}//namespace AutoLib
