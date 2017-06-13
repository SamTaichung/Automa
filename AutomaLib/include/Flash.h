
#pragma once


// MFlash 命令目標
#include "Timer.h"
#include "IO.h"
namespace AutoLib {
class MFlash : public MBase
{
	DECLARE_DYNAMIC(MFlash)

public:
	MFlash(MBase *pB,CString strID,CString strName);
	~MFlash();
	//-------------------override--------------------------
	void Cycle(const double dblTime);
	virtual bool isIDLE();
	void Stop();
	void EStop(const bool isES);
	//-------------------member function--------------------
	void DoOn(void);
	void DoOff(void);
	void DoBlink(void);

	MTimer *m_pTMOn;	//控制Blink On時間用的Timer
	MTimer *m_pTMOff;	//控制Blink Off時間用的Timer
	MIO *m_pIO;		//用來控制的I/O
protected:
	DECLARE_MESSAGE_MAP()
private:
	bool blNow,isDoBlink;
};
}//namespace MacLib

