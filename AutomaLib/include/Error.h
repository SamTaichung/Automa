#pragma once
#pragma warning( disable :4091)

// MError ©R¥O¥Ø¼Ð
#include <vector>
#include "Solution.h"
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
namespace AutoLib {
	class MBase;
	AUTOMALIB_API class MError : public CCmdTarget
	{
		DECLARE_DYNAMIC(MError)
	public:
		AUTOMALIB_API MError(MBase *pBase=NULL,CString strDescript=_T(""),int intErrorCode=0,CString strReason= _T(""));
		AUTOMALIB_API virtual ~MError();
		AUTOMALIB_API void	AddSolution(MSolution *pSolution);
		int	m_intErrorCode;
		CString m_strDescript;
		CString m_strReason;
		std::vector<MSolution *> m_Solutions;
		MBase		*m_Happener;
		MBase		*m_pPara;
		MSolution	*m_pSelectedSolution;
		bool	m_isStop;
		CTime	tHappen;
	protected:
		DECLARE_MESSAGE_MAP()
	};

}//namespace AutoLib
