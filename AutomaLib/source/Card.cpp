// Card.cpp : ��@��
//

#include "stdafx.h"
#include "Card.h"


// MCard �����d IO�d�A�b�d��
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



// MCard �T���B�z�`��

