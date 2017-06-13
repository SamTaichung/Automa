#pragma once
#include "ADOTool.h"
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
namespace AutoLib{
AUTOMALIB_API class CStringProvider
{
public:
	AUTOMALIB_API CStringProvider(CIniFile *pLangFile);
	AUTOMALIB_API CString GetString(CString strSection,CString strItem, CString strDefault = _T(""));
private:
	CIniFile *m_pLangFile;
};

}//namespace AutoLib