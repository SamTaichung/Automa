
#pragma once


// MPulseIO ©R¥O¥Ø¼Ð
#include "IO.h"
#include "Timer.h"
namespace AutoLib{
class MPulseIO : public MBase
{
	DECLARE_DYNAMIC(MPulseIO)

public:
	MPulseIO(MBase *pB,CString strID,CString strName);
	MIO * m_pIO;
	MTimer *m_pOnTimer;
	void Pulse();
	void StepCycle(const double dblTime);
	bool GetIOValue();
	void SetIOValue(bool blV);
	void Stop();
	void EStop(const bool isES);
	void SetOnTimer(double dbTimer);
	virtual ~MPulseIO();
	
protected:
	DECLARE_MESSAGE_MAP()
};

}//namespace AutoLib
