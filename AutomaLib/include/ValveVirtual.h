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
// MValveVirtual 命令目標
namespace AutoLib{ 
AUTOMALIB_API class MValveVirtual : public MValve
{
public:
	AUTOMALIB_API MValveVirtual(MBase *,CString strID,CString strName,
		CString strOpen=_T("Open/開"),CString strClose=_T("Close/閉"),ValveStyle vsStyle=ValveStyle::CylinderValve);
	AUTOMALIB_API virtual ~MValveVirtual();
	//--------------------------------------------------------------------
	AUTOMALIB_API virtual bool isOpen();
	AUTOMALIB_API virtual bool isClose();
	AUTOMALIB_API virtual bool OffValve();

	AUTOMALIB_API virtual bool DoOpen();		//實作Open動作
	AUTOMALIB_API virtual void OnOpening();		//Open過程中
	AUTOMALIB_API virtual bool DoClose();		//實作Close動作
	AUTOMALIB_API virtual void OnClosing();		//Close過程中
	//-----------------------------------------------------------------------------------
	//m_pIOOpen:Open動作點
	//m_pIOClose:Close動作點
	//m_pIOOpenSR:Open動作到位的SR
	//m_pIOCloseSR:Close動作到位的SR
	//m_pIOOpenSWSR:Open動作中如果m_pIOOpenSWSR就把m_pIOOpenSW設為On
	//m_pIOCloseSWSR:Close動作中如果m_pIOCloseSWSR就把m_pIOCloseSW設為On
	//------------------------------------------------------------------------------------
	MIO *m_pIOOpenSR,*m_pIOCloseSR,*m_pIOOpenSWSR,*m_pIOCloseSWSR,*m_pIOOpen,
		*m_pIOClose,*m_pIOOpenSW,*m_pIOCloseSW;
};

}//namespace AutoLib