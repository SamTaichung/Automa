#pragma once
#pragma warning( disable :4091)

#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
namespace AutoLib {
class MBase;
AUTOMALIB_API class MSolution
{
public:
	AUTOMALIB_API MSolution(CString strD,MBase *pP,int intState,int intStep,int intMode,MBase* pRemoveFrom=NULL);
	AUTOMALIB_API MSolution(MSolution *pSolution);
	AUTOMALIB_API virtual ~MSolution();
	AUTOMALIB_API virtual CString GetDescription();
	AUTOMALIB_API virtual void DoProcess();
protected:
	CString m_strDescription;
	MBase * m_pProcess;
	MBase * m_pRemoveErrorsFrom; //�]�w���Φ����خɭn�M���P�ɤ޵o���~������Ψ�l���ҵo�����~
	int m_State,m_Step,m_Mode;
};

}//namespace AutoLib
