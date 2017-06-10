
#pragma once
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
#include "Base.h"				
#include "IOLib.h"				
namespace AutoLib{
AUTOMALIB_API class MIO : public MBase
{
	friend MIOLib;
public:
	AUTOMALIB_API MIO(MBase *pB,CString strID,CString strName,MIOLib *pIOLib,bool bisOut,bool bLogic);
	AUTOMALIB_API virtual ~MIO();
	// Member function
	AUTOMALIB_API virtual bool GetValue(void);
	AUTOMALIB_API virtual void SetValue(bool blV);
	AUTOMALIB_API virtual void Inverse();
	AUTOMALIB_API virtual bool GetIsOut();
	AUTOMALIB_API virtual void SaveMachineData(CADOConnection * pC, bool bAllChildsSave = true);
	AUTOMALIB_API virtual void LoadMachineData(CADOConnection * pC, bool bAllChildsLoad = true);
	MIOLib *m_pIOLib;
protected:
	bool m_Value, m_isOut;
	bool m_Logic; //true:¥¿ÅÞ¿è false:­tÅÞ¿è
};
}//namespace AutoLib

