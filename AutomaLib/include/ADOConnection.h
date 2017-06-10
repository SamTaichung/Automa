#pragma once
#ifdef _M_X64
#import "c:\\program files\\common files\\system\\ado\\msadox.dll"
#import "c:\\program files\\common files\\system\\ado\\msado15.dll" rename(_T("EOF"),_T("adoEOF"))
//#import "c:\\program files\\common files\\system\\ado\\msjro.dll" 
#else
#import "c:\\program files (x86)\\common files\\system\\ado\\msadox.dll"
#import "c:\\program files (x86)\\common files\\system\\ado\\msado15.dll" rename(_T("EOF"),_T("adoEOF"))
using namespace ADODB;
#import "c:\\program files (x86)\\common files\\system\\ado\\msjro.dll" 
#endif
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
// CADOConnection
namespace AutoLib{
AUTOMALIB_API class CADOConnection:public CCmdTarget
{
public:
	AUTOMALIB_API CADOConnection(CString ConnectionString=_T(""));
	AUTOMALIB_API virtual ~CADOConnection();
	AUTOMALIB_API int GetTableIndex(CString TBName);
	AUTOMALIB_API int GetFieldIndex(CString TBName,CString FDName);
	AUTOMALIB_API bool ExecuteSQL(CString strSQL);
	AUTOMALIB_API bool BeginTrans();
	AUTOMALIB_API bool CommitTrans();
	AUTOMALIB_API bool RollBackTrans();
	AUTOMALIB_API bool Open();
	AUTOMALIB_API void Close();
	AUTOMALIB_API int GetState();
	AUTOMALIB_API bool CreateMDB(CString FilePath);
	ADODB::_ConnectionPtr	m_pConnection;
	CString					m_ConnectionString;
	ADODB::_ConnectionPtr operator =(const CADOConnection *);
};
}//namespace AutoLib

