#pragma once
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif

#include "adoconnection.h"

// CADORecordset
namespace AutoLib{
AUTOMALIB_API class CADORecordset : public CCmdTarget
{

public:
	AUTOMALIB_API CADORecordset();
	AUTOMALIB_API virtual ~CADORecordset();
	ADODB::_RecordsetPtr m_pRecordset;
	AUTOMALIB_API bool Open(CString strSQL,CADOConnection * pConnection);
	AUTOMALIB_API void Close();
	AUTOMALIB_API bool MoveLast();
	AUTOMALIB_API bool MoveFirst();
	AUTOMALIB_API bool MovePrevious();
	AUTOMALIB_API bool MoveNext();
	AUTOMALIB_API bool AddNew();
	AUTOMALIB_API bool Update();
	AUTOMALIB_API bool isEOF();
	AUTOMALIB_API bool isBOF();
	AUTOMALIB_API void SetValue(CString FDName,const CString V);
	AUTOMALIB_API void SetValue(CString FDName,const CTime V);
	AUTOMALIB_API void SetValue(CString FDName,const COleDateTime V);
	AUTOMALIB_API void SetValue(CString FDName,const int V);
	AUTOMALIB_API void SetValue(CString FDName,const long V);
	AUTOMALIB_API void SetValue(CString FDName,const double V);
	AUTOMALIB_API void SetValue(CString FDName,const bool V);
	AUTOMALIB_API void SetValue(CString FDName,const unsigned short V);
	AUTOMALIB_API void GetValue(CString FDName,CString &V);
	AUTOMALIB_API void GetValue(CString FDName,CTime &V);
	AUTOMALIB_API void GetValue(CString FDName,COleDateTime &V);
	AUTOMALIB_API void GetValue(CString FDName,int &V);
	AUTOMALIB_API void GetValue(CString FDName,long &V);
	AUTOMALIB_API void GetValue(CString FDName,double &V);
	AUTOMALIB_API void GetValue(CString FDName,bool &V);
	AUTOMALIB_API void GetValue(CString FDName,unsigned short &V);
	AUTOMALIB_API ADODB::EditModeEnum GetEditMode()
		{return(m_pRecordset->GetEditMode());};
	AUTOMALIB_API long GetState()
		{return(m_pRecordset->GetState());};
	AUTOMALIB_API bool CancelUpdate()
		{return(SUCCEEDED(m_pRecordset->CancelUpdate()));};
	
};

}//namespace AutoLib
