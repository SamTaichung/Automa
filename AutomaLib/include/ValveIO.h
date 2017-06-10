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
// MValveIO 命令目標
namespace AutoLib{ 
AUTOMALIB_API class MValveIO : public MValve
{
public:
	AUTOMALIB_API MValveIO(MBase *,CString strID,CString strName,
		CString strOpen=_T("Open/開"),CString strClose=_T("Close/閉"),
		ValveStyle vsStyle=ValveStyle::CylinderValve);
	AUTOMALIB_API virtual ~MValveIO();
	//--------------------------------------------------------------------
	AUTOMALIB_API virtual void SaveMachineData(CADOConnection * pC, bool bAllChildsSave = true);
	AUTOMALIB_API virtual void LoadMachineData(CADOConnection * pC, bool bAllChildsLoad = true);
	AUTOMALIB_API virtual CString GetStateName();
	AUTOMALIB_API virtual ValveStyle GetValveStyle(){return m_vsStyle;}; //傳回電磁閥型式
	//--------------------------------------------------------------------
	enum ValveMode{
		vmNormal,	  //正常模式(成功或不成功都不會OffValve)
		vmForce,	  //強制模式(不會引發錯誤也不會OffValve)
		vmTest,		  //測試模式，結果用GetResult取得
		vmAutoOff,    //如果完成(成功或不成功)就自動關(OffValve)
		vmOKThenOff,  //如果成功就自動關(OffValve)
		vmNGThenOff,  //如果不成功就自動關(OffValve)
	};
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