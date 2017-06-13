#pragma once

#include "Card.h"

// MCardMacHSL
namespace AutoLib{
	class MCardMacHSL : public MCard
	{
		DECLARE_DYNAMIC(MCardMacHSL)

	public:
		MCardMacHSL(MBase *p_B,CString strNodeName,CString strEName,CString strCName,int intErrBase);
		virtual ~MCardMacHSL();
		virtual bool Open();
		virtual void Close();
		virtual bool InitialCard();
		unsigned short  m_lCardID;
		int m_TotalCard;
	protected:
		DECLARE_MESSAGE_MAP()
	};

}//namespace AutoLib
