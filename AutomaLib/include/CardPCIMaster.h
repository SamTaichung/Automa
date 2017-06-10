#pragma once

// MCard
#include "Card.h"				
namespace AutoLib{
	AUTOMALIB_API class MCardPCIMaster : public MCard
	{
	public:
		AUTOMALIB_API MCardPCIMaster(MBase *pParent,CString strID,CString strName);
		AUTOMALIB_API ~MCardPCIMaster();
		AUTOMALIB_API bool Open();
		AUTOMALIB_API void Close();
		AUTOMALIB_API int GetCardCount();
	protected:
		int m_intCardCount;
	};

}//namespace AutoLib
