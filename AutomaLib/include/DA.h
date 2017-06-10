#pragma once
#include "base.h"

namespace AutoLib{

class MDA : public MBase
{
public:
	MDA(MBase *pB,CString strID,CString strC,int ChannelID,CString LineID,int Max,int Min);
	virtual ~MDA(void);
	
	static std::map<CString,MDA*>	m_Members;
	
	virtual double	GetValue();
	virtual bool	SetValue(double dblValue){return false;};
	
	void	SaveMachineData(CADOConnection * pC, bool bAllChildsSave = true);
	void	LoadMachineData(CADOConnection * pC, bool bAllChildsLoad = true);
	
public:
	CString		m_LineID;
	int			m_ChannelID;

protected:
	double		m_dblValue;
	double		m_dblMax,m_dblMin;
};


}//AutoLib
