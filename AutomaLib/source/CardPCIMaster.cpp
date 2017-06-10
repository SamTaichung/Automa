// Card.cpp : ¹ê§@ÀÉ
//

#include "stdafx.h"
#include "CardPCIMaster.h"
#include "SamNet.h"
#ifdef _M_X64
#pragma comment(lib, "SamNetDLL_X64D.lib") 
#else	
#pragma comment(lib, "SamNetDLL_W32D.lib") 
#endif

using namespace AutoLib;

MCardPCIMaster::MCardPCIMaster(MBase *pB,CString strID,CString strName):
	MCard(pB,strID,strName)
{
	m_intCardCount = 0;
}
MCardPCIMaster::~MCardPCIMaster()
{
}
bool MCardPCIMaster::Open()
{
	m_isOpen = false;
	m_intCardCount = SamNet_OpenCard();
	if (m_intCardCount > 0)
	{
		m_isOpen = true;
	}
	return(m_isOpen);
}
void MCardPCIMaster::Close()
{
	SamNet_CloseCard();
	m_intCardCount = 0;
}
int MCardPCIMaster::GetCardCount()
{
	return m_intCardCount;
}

