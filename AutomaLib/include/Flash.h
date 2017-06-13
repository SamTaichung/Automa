
#pragma once


// MFlash �R�O�ؼ�
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

	MTimer *m_pTMOn;	//����Blink On�ɶ��Ϊ�Timer
	MTimer *m_pTMOff;	//����Blink Off�ɶ��Ϊ�Timer
	MIO *m_pIO;		//�Ψӱ��I/O
protected:
	DECLARE_MESSAGE_MAP()
private:
	bool blNow,isDoBlink;
};
}//namespace MacLib

