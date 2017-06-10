
#pragma once
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
#include "Base.h"
namespace AutoLib{
AUTOMALIB_API class MTimer : public MBase
{
public:
	AUTOMALIB_API MTimer(MBase *pB,CString strID,CString strName,double dblTime);
	AUTOMALIB_API virtual ~MTimer();
	//---------------------------------------------------------------------------
	AUTOMALIB_API void Cycle(const double dblTime);
	AUTOMALIB_API virtual void EStop(const bool isES);
	AUTOMALIB_API virtual void Stop();
	AUTOMALIB_API virtual void SaveMachineData(CADOConnection * pC, bool bAllChildsSave = true);
	AUTOMALIB_API virtual void LoadMachineData(CADOConnection * pC, bool bAllChildsLoad = true);
	//----------------------------------------------------------------------------
	AUTOMALIB_API bool isTimeOut(void);		//�ˬd��Timer�O�_�p�ɧ���
	AUTOMALIB_API void SetInterval(double dblInterval);		//�]�wTimer�ɶ�
	AUTOMALIB_API double GetInterval();		//�]�wTimer�ɶ�
	AUTOMALIB_API double GetRemanderTime();
	AUTOMALIB_API void Start();
protected:
	double m_dblInterval;			//�]�w�n�p�ɪ��ɶ�(���Gms)
private:
	bool m_bTimerStart;
	double dblTimeCount;
};

}//namespace AutoLib
