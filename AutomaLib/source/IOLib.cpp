#include "stdafx.h"
#include "IOLib.h"
#include "IO.h"

using namespace AutoLib;

MIOLib::MIOLib(MBase *pB,CString strID,CString strName):
	MBase(pB,strID,strName)
{
}
MIOLib::~MIOLib()
{
}
bool MIOLib::GetValue(MIO* pIO)
{
	return pIO->m_Value;
}
void MIOLib::SetValue(MIO* pIO, bool bValue)
{
	pIO->m_Value = bValue;
}

