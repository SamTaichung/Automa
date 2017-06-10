// Error.cpp : 實作檔
//

#include "stdafx.h"
#include "Error.h"


// MError
using namespace AutoLib;

IMPLEMENT_DYNAMIC(MError, CCmdTarget)
MError::MError(MBase *pBase,CString strDescript,int intErrorCode,CString strReason)
{
	m_intErrorCode=intErrorCode;
	m_Happener=pBase;
	m_strDescript=strDescript;
	m_strReason = strReason;
	m_pSelectedSolution=NULL;
	m_isStop=false;
	m_pPara=NULL;
}

MError::~MError()
{
	MSolution *pSolution;
	std::vector<MSolution*>::iterator itSolution;
	for(itSolution=m_Solutions.begin();itSolution!=m_Solutions.end();itSolution++)
	{
		pSolution=*itSolution;
		delete pSolution;
	}
	m_Solutions.clear();
}

void MError::AddSolution(MSolution *pSolution)
{
	m_Solutions.push_back(pSolution);
}
BEGIN_MESSAGE_MAP(MError, CCmdTarget)
END_MESSAGE_MAP()



// MError 訊息處理常式
