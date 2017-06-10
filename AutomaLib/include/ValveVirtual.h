#pragma once
#include <windows.h>
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
#include "IO.h"
#include "timer.h"
#include "Valve.h"
// MValveVirtual �R�O�ؼ�
namespace AutoLib{ 
AUTOMALIB_API class MValveVirtual : public MValve
{
public:
	AUTOMALIB_API MValveVirtual(MBase *,CString strID,CString strName,
		CString strOpen=_T("Open/�}"),CString strClose=_T("Close/��"),ValveStyle vsStyle=ValveStyle::CylinderValve);
	AUTOMALIB_API virtual ~MValveVirtual();
	//--------------------------------------------------------------------
	AUTOMALIB_API virtual bool isOpen();
	AUTOMALIB_API virtual bool isClose();
	AUTOMALIB_API virtual bool OffValve();

	AUTOMALIB_API virtual bool DoOpen();		//��@Open�ʧ@
	AUTOMALIB_API virtual void OnOpening();		//Open�L�{��
	AUTOMALIB_API virtual bool DoClose();		//��@Close�ʧ@
	AUTOMALIB_API virtual void OnClosing();		//Close�L�{��
	//-----------------------------------------------------------------------------------
	//m_pIOOpen:Open�ʧ@�I
	//m_pIOClose:Close�ʧ@�I
	//m_pIOOpenSR:Open�ʧ@��쪺SR
	//m_pIOCloseSR:Close�ʧ@��쪺SR
	//m_pIOOpenSWSR:Open�ʧ@���p�Gm_pIOOpenSWSR�N��m_pIOOpenSW�]��On
	//m_pIOCloseSWSR:Close�ʧ@���p�Gm_pIOCloseSWSR�N��m_pIOCloseSW�]��On
	//------------------------------------------------------------------------------------
	MIO *m_pIOOpenSR,*m_pIOCloseSR,*m_pIOOpenSWSR,*m_pIOCloseSWSR,*m_pIOOpen,
		*m_pIOClose,*m_pIOOpenSW,*m_pIOCloseSW;
};

}//namespace AutoLib