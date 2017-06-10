
#pragma once


#include "MRGBLight.h"
namespace AutoLib{

class MHSLRGBLight : public MRGBLight
{
	DECLARE_DYNAMIC(MHSLRGBLight)

public:
	MHSLRGBLight(MBase *pB,CString LightID,CString strN,CString strC,int Connection,int SlaveID,int channel,int intErrBase);
	virtual ~MHSLRGBLight();

	bool	SetOn(bool blOn);
	bool	Active();
	bool	GetValue(int *R,int *G,int *B);

	double	m_MaxCurrent;
	int		m_Channel;
	int		m_nConnection;
	int		m_nSlaveID;

protected:
	DECLARE_MESSAGE_MAP()
};
};// AutoLib


