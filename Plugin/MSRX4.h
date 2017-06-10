
#pragma once
#include "IO.h"

// MFeeder ©R¥O¥Ø¼Ð
using namespace AutoLib;
class MSRX4 : public MIO
{
public:
	MSRX4(MBase *pParent,CString strID,CString strName):
		MIO(pParent, strID, strName,NULL, false, true)
	{
		m_pIOSR1 = NULL;
		m_pIOSR2 = NULL;
		m_pIOSR3 = NULL;
		m_pIOSR4 = NULL;
	};
	virtual ~MSRX4() {};
	bool GetValue() { return m_pIOSR1->GetValue() && m_pIOSR2->GetValue() && 
					m_pIOSR3->GetValue() && m_pIOSR4->GetValue(); };
	MIO *m_pIOSR1;
	MIO *m_pIOSR2;
	MIO *m_pIOSR3;
	MIO *m_pIOSR4;
};


