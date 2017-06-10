#pragma once
#include "MIO.h"
#include "hsl.h"
#include "hslerr.h"

// MIOHSL:實作淩華HighSpeedLink 7851/52的Remote IO
namespace AutoLib{
class MIOHSL : public MIO
{
	DECLARE_DYNAMIC(MIOHSL)

public:
	enum HSLType
	{
		I8,
		O8,
		I16,
		O16,
		I32,
		O32,
		I8O8,
		I16O16,
		IO32,
	};
	MIOHSL(MBase *pB,CString strID,CString strN,CString strC,int intErrBase,U16 SetID,U16 SlaveID,U16 Pin,bool bisOut=true,int iType=HSLType::IO32,bool bLogic=true);
	virtual ~MIOHSL();
	//-----------------Override function-----------------------
	MBase * AddChild(MBase *p_C);	//須覆寫AddChild以收集SlaveID
	void LoadSystemData();
	void SaveSystemData();
	void LoadMachineData(CADOConnection * pC);
	void SaveMachineData(CADOConnection * pC);
	//--------------member variable----------------------------
	virtual bool GetValue(void);
	virtual void SetValue(bool blV);
	virtual void Inverse();
	U16 m_ConnectIndex,m_SlaveID,m_Pin;	//使用的SetID,SlaveID及Pin腳
	HSLType	m_HslType;
protected:
	DECLARE_MESSAGE_MAP()
public:
	void Change2I16O16(void);
};

}//namespace AutoLib
