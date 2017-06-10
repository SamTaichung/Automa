
#pragma once
#include "IO.h"

// MFeeder ©R¥O¥Ø¼Ð
using namespace AutoLib;
class MSRX2 : public MIO
{
public:
	MSRX2(MBase *pParent, CString strID, CString strName):
		MIO(pParent,strID,strName,NULL,false,true)
		{
			m_pIOSR1 = NULL;
			m_pIOSR2 = NULL;
		};
	virtual ~MSRX2() {};
	bool GetValue() { return m_pIOSR1->GetValue() && m_pIOSR2->GetValue(); };
	MIO *m_pIOSR1;
	MIO *m_pIOSR2;
};


