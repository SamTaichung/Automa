// Card.cpp : 實作檔
//

#include "stdafx.h"
#include "Card.h"


// MCard 介面卡 IO卡，軸卡等
using namespace AutoLib;

MCard::MCard(MBase *pB,CString strID,CString strName):
	MBase(pB,strID,strName)
{
}
MCard::~MCard()
{
}
bool MCard::Open()
{
	m_isOpen=true;
	return(m_isOpen);
}
void MCard::Close()
{
}



// MCard 訊息處理常式

