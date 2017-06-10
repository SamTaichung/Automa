// ADORecordset.cpp : ¹ê§@ÀÉ
//

#include "stdafx.h"
#include "ADORecordset.h"
using namespace AutoLib;

// CADORecordset

CADORecordset::CADORecordset()
{
	HRESULT hr=m_pRecordset.CreateInstance(_T("ADODB.Recordset"));
	if(FAILED(hr))
	{
		AfxMessageBox(_T("Unable to create recordset object"));
		return;
	}
}

CADORecordset::~CADORecordset()
{
	try
	{
		m_pRecordset.Release();
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}
void CADORecordset::Close()
{
	try
	{
		m_pRecordset->Close();
	}
	catch(_com_error e) 
	{
		//AfxMessageBox(e.Description());
	}	
}
bool CADORecordset::Open(CString strSQL,CADOConnection * pConnection)
{
	HRESULT hr;
	ADODB::_CommandPtr pCmd;
	if(pConnection==NULL) return FALSE;
	hr = pCmd.CreateInstance(__uuidof(ADODB::Command));
	if (SUCCEEDED(hr))
	{
		try
		{
			hr=m_pRecordset->Open(_bstr_t(strSQL),
				_variant_t((IDispatch*)(pConnection->m_pConnection)),
				ADODB::adOpenStatic,ADODB::adLockOptimistic,ADODB::adCmdText);
			return(TRUE);
		}
		catch(_com_error e) 
		{
			AfxMessageBox(e.Description());
			return FALSE;
		}	
	}
	return TRUE;
}
bool CADORecordset::MoveLast()
{
	return(SUCCEEDED(m_pRecordset->MoveLast()));
}
bool CADORecordset::MoveFirst()
{
	return(SUCCEEDED(m_pRecordset->MoveFirst()));
}
bool CADORecordset::MovePrevious()
{
	return(SUCCEEDED(m_pRecordset->MovePrevious()));
}
bool CADORecordset::MoveNext()
{
	return(SUCCEEDED(m_pRecordset->MoveNext()));
}
bool CADORecordset::isEOF()
{
	VARIANT_BOOL eof;
	m_pRecordset->get_adoEOF(&eof);
	return(eof==VARIANT_TRUE);
}
bool CADORecordset::isBOF()
{
	VARIANT_BOOL bof;
	m_pRecordset->get_BOF(&bof);
	return(bof==VARIANT_TRUE);
}
bool CADORecordset::Update()
{
	return(SUCCEEDED(m_pRecordset->Update()));
}
bool CADORecordset::AddNew()
{
	return(SUCCEEDED(m_pRecordset->AddNew()));
}
void CADORecordset::SetValue(CString FDName,const int V)
{
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->put_Value(_variant_t(long(V)));
	ADODB::_RecordsetPtr pR;
}
void CADORecordset::SetValue(CString FDName,const long V)
{
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->put_Value(_variant_t(V));
}
void CADORecordset::SetValue(CString FDName,const double V)
{
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->put_Value(_variant_t(V));
}
void CADORecordset::SetValue(CString FDName,const CString V)
{
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->put_Value(_variant_t(V));
}
void CADORecordset::SetValue(CString FDName,const CTime V)
{
	COleDateTime cdtV;
	cdtV.SetDateTime(V.GetYear(),V.GetMonth(),V.GetDay(),V.GetHour(),V.GetMinute(),V.GetSecond());
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->put_Value(_variant_t(cdtV));
}
void CADORecordset::SetValue(CString FDName,const COleDateTime V)
{
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->put_Value(_variant_t(V));
}
void CADORecordset::SetValue(CString FDName,const bool V)
{
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->put_Value(_variant_t(V));
}
void CADORecordset::SetValue(CString FDName,const unsigned short V)
{
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->put_Value(_variant_t(long(V)));
}
void CADORecordset::GetValue(CString FDName,CString &V)
{
	_variant_t t;
	t.bstrVal=NULL;
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->get_Value(&t);
	if (t.vt==VT_NULL)
	{
		V=_T("");
	}else{
		V=CString(t.bstrVal);
	}
}
void CADORecordset::GetValue(CString FDName,CTime &V)
{
	_variant_t t;
	t.bstrVal=NULL;
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->get_Value(&t);
	if (t.vt==VT_NULL)
	{
		V=CTime();
	}else{
		V=CTime(t.date);
	}
}
void CADORecordset::GetValue(CString FDName,COleDateTime &V)
{
	_variant_t t;
	t.bstrVal=NULL;
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->get_Value(&t);
	if (t.vt==VT_NULL)
	{
		V=COleDateTime();
	}else{
		V=COleDateTime(t.date);
	}
}
void CADORecordset::GetValue(CString FDName,int &V)
{
	_variant_t t;
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->get_Value(&t);
	V=t.intVal;
}
void CADORecordset::GetValue(CString FDName,long &V)
{
	_variant_t t;
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->get_Value(&t);
	V=t.lVal;
}
void CADORecordset::GetValue(CString FDName,double &V)
{
	_variant_t t;
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->get_Value(&t);
	V=t.dblVal;
}
void CADORecordset::GetValue(CString FDName,bool &V)
{
	_variant_t t;
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->get_Value(&t);
	V=t.boolVal;
}
void CADORecordset::GetValue(CString FDName,unsigned short &V)
{
	_variant_t t;
	m_pRecordset->GetFields()->GetItem(_bstr_t(FDName))->get_Value(&t);
	V=t.intVal;
}
