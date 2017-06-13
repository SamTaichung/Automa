#pragma once
// MCard
#include "Base.h"				
namespace AutoLib{
	AUTOMALIB_API class MCard : public MBase
	{
	public:
		MCard(MBase *pParent,CString strID,CString strName);
		virtual ~MCard();
		virtual bool Open();
		virtual void Close();
	protected:
		bool m_isOpen;
	};

}//namespace AutoLib
