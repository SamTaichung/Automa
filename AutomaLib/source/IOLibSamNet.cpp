#include "stdafx.h"
#include "IOLibSamNet.h"
#include "IO.h"
#include "SamNet.h"
#ifdef _M_X64
#pragma comment(lib, "SamNetDLL_X64D.lib") 
#else	
#pragma comment(lib, "SamNetDLL_W32D.lib") 
#endif
using namespace AutoLib;

MIOLibSamNet::MIOLibSamNet(MIOLib *pB,CString strID,CString strName):
	MIOLib(pB,strID,strName)
{
}
MIOLibSamNet::~MIOLibSamNet()
{
}
bool MIOLibSamNet::GetValue(MIO* pIO)
{
	bool bValue=false;
	if (pIO->GetIsOut())
	{
		SamNet_ReadOutChannel(pIO->m_CardID, pIO->m_ConnectID, pIO->m_StationID,pIO->m_PinID, &bValue);
	}
	else{
		SamNet_ReadInChannel(pIO->m_CardID, pIO->m_ConnectID, pIO->m_StationID, pIO->m_PinID, &bValue);
	}
	return bValue;
}
void MIOLibSamNet::SetValue(MIO* pIO, bool bValue)
{	
	if (pIO->GetIsOut())
	{
		SamNet_WriteOutChannel(pIO->m_CardID, pIO->m_ConnectID, pIO->m_StationID, pIO->m_PinID, bValue);
	}
}

