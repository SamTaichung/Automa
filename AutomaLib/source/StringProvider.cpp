#include "StdAfx.h"		// if remark this line, compile fatal error C1010 occurs.
#include "StringProvider.h"

using namespace AutoLib;
CStringProvider::CStringProvider(CIniFile *pLangFile)
{
	m_pLangFile = pLangFile;
}
CString CStringProvider::GetString(CString strSection, CString strItem, CString strDefault)
{
	CString strRet;
	if (!m_pLangFile->GetItemString(strSection, strItem, strRet))
	{
		strRet = strDefault;
		if (strSection.GetLength() > 0 && strItem.GetLength() > 0)
		{
			m_pLangFile->WriteItemString(strSection, strItem, strRet);
		}
	}
	return strRet;
}