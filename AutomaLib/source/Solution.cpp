// Solution.cpp : ¹ê§@ÀÉ
//

#include "stdafx.h"
#include "Solution.h"
#include "Base.h"

// MSolution
using namespace AutoLib;

MSolution::MSolution(CString strD,MBase *pP,int intState,
	int intStep,int intMode,MBase* pRemoveFrom)
	:m_strDescription(strD),m_pProcess(pP),m_State(intState),
	m_Step(intStep),m_Mode(intMode),m_pRemoveErrorsFrom(pRemoveFrom)
{
}
MSolution::MSolution(MSolution *pSolution)
{
	m_strDescription =pSolution->m_strDescription;
	m_pProcess=pSolution->m_pProcess;
	m_State=pSolution->m_State;
	m_Step=pSolution->m_Step;
	m_Mode=pSolution->m_Mode;
	m_pRemoveErrorsFrom=pSolution->m_pRemoveErrorsFrom;
}
MSolution::~MSolution()
{
}
CString MSolution::GetDescription()
{
	return m_strDescription;
}
void MSolution::DoProcess()
{
	m_pProcess->ErrorProcess((MBase::STATE)m_State, m_Step, m_Mode);
}