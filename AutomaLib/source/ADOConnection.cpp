// ADOConnection.cpp : 實作檔
//

#include "stdafx.h"
#include "ADOConnection.h"

using namespace AutoLib;

CADOConnection::CADOConnection(CString ConnectionString):m_ConnectionString(ConnectionString)
{
	HRESULT hr=m_pConnection.CreateInstance(__uuidof(ADODB::Connection));
	if(FAILED(hr))
	{
		TRACE(_T("Unable to create connection object"));
		return;
	}
}
ADODB::_ConnectionPtr CADOConnection::operator =(const CADOConnection * pC)
{
	return(pC->m_pConnection);
}
int CADOConnection::GetState()
{
	return 0;
}
CADOConnection::~CADOConnection()
{
	try
	{
		m_pConnection.Release();
	}
	catch(_com_error e)
	{
		AfxMessageBox(_T("Connection release error."));
	}
}
int CADOConnection::GetTableIndex(CString TBName)
{
	//if(MDB) return FALSE;
	ADOX::_CatalogPtr pCatalog;
	ADOX::TablesPtr pTables;
	ADOX::_TablePtr pTableOP;
	pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
	pTables.CreateInstance(__uuidof(ADOX::Tables));
	pTableOP.CreateInstance(__uuidof(ADOX::Table));
	int index=-1;
	long lCount;
	CString strTmp;
	_variant_t vItem;
	_bstr_t aa;
	vItem.ChangeType(VT_I2,NULL);
	try
	{
		pCatalog->PutRefActiveConnection((IDispatch*)m_pConnection);
		pTables=pCatalog->GetTables();
		pTables->Refresh();
		lCount=pTables->GetCount();
		for(int i=0;i<lCount;i++)
		{
			vItem.intVal=i;
			pTableOP=pTables->GetItem(vItem);
			strTmp=(char *)(pTableOP->GetName());
			if (TBName.CompareNoCase(strTmp)==0) 
			{
				index=i;
				break;
			}
		}
		pCatalog->PutRefActiveConnection(NULL);
	}
	catch(_com_error e) 
	{
		AfxMessageBox(_T("GetTable index error"));
	}
	if (pTableOP!=NULL) pTableOP.Release();
	if (pTables!=NULL) pTables.Release();
	if (pCatalog!=NULL) pCatalog.Release();
	return(index);
}

int CADOConnection::GetFieldIndex(CString TBName,CString FDName)
{
	ADOX::_CatalogPtr pCatalog;
	ADOX::TablesPtr pTables;
	ADOX::_TablePtr pTableOP;
	ADOX::_ColumnPtr pColumnOP;
	ADOX::ColumnsPtr pColumns;

	pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
	pTables.CreateInstance(__uuidof(ADOX::Tables));
	pTableOP.CreateInstance(__uuidof(ADOX::Table));
	pColumnOP.CreateInstance(__uuidof(ADOX::_Column));
	pColumns.CreateInstance(__uuidof(ADOX::Columns));

	int index=-1,intTBIndex,i;
	CString strTmp;
	_variant_t vItem;
	vItem.ChangeType(VT_I2,NULL);
	if((intTBIndex=GetTableIndex(TBName))>=0)
	{
		long lCount;
		try
		{
			pCatalog->PutRefActiveConnection((IDispatch*)m_pConnection);
			pTables=pCatalog->GetTables();
			pTables->Refresh();
			vItem.intVal=intTBIndex;
			pTableOP=pTables->GetItem(vItem);
			pColumns=pTableOP->GetColumns();
			pColumns->Refresh();
			lCount=pColumns->GetCount();
			for(i=0;i<lCount;i++)
			{
				vItem.intVal=i;
				pColumnOP=pColumns->GetItem(vItem);
				strTmp=(char*)(pColumnOP->GetName());
				if (FDName.CompareNoCase(strTmp)==0)
				{
					index=i;
				}
			}
			pCatalog->PutRefActiveConnection(NULL);
		}
		catch(_com_error e) 
		{
			AfxMessageBox(_T("GetField index"));
		}
	}
	if (pTableOP!=NULL) pTableOP.Release();
	if (pTables!=NULL) pTables.Release();
	if (pColumnOP!=NULL) pColumnOP.Release();
	if (pColumns!=NULL) pColumns.Release();
	if (pCatalog!=NULL) pCatalog.Release();
	return(index);
}
bool CADOConnection::ExecuteSQL(CString strSQL)
{
	using namespace ADODB;
	VARIANT *vt1 = NULL;
	_variant_t vra;
	try
	{
		m_pConnection->Execute(_bstr_t(strSQL),vt1,adCmdText);
	}
	catch(_com_error e) 
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE;
}
bool CADOConnection::Open()
{
	HRESULT hr;
	try
	{
		hr=m_pConnection->put_CursorLocation(ADODB::adUseClient);
		hr=m_pConnection->Open(_bstr_t(m_ConnectionString),
			_bstr_t(""),_bstr_t(""),-1);
		return TRUE;
	}
	catch(_com_error e)
	{
		_bstr_t bstrDescription(e.Description());
		m_ConnectionString.Trim();
		AfxMessageBox(m_ConnectionString+_T(" Open failure!"));
		return FALSE;
	}
	
}
void CADOConnection::Close()
{
	try
	{
		m_pConnection->Close();
	}
	catch(_com_error e) 
	{
		AfxMessageBox(_T("Connection close error"));
		//m_pConnection=NULL;
		return;
	}
}
bool CADOConnection::BeginTrans()
{
	m_pConnection->BeginTrans();
	return(true);
}
bool CADOConnection::CommitTrans()
{
	HRESULT hr=m_pConnection->CommitTrans();
	return(SUCCEEDED(hr));

}
bool CADOConnection::RollBackTrans()
{
	HRESULT hr=m_pConnection->RollbackTrans();
	return(SUCCEEDED(hr));
}

bool CADOConnection::CreateMDB(CString PathFile)
{
	ADOX::_CatalogPtr pCatalog;
	pCatalog.CreateInstance(__uuidof(ADOX::Catalog));
	
	if (pCatalog==NULL) return FALSE;
	_bstr_t ConnectString = _bstr_t(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + PathFile);
	try
	{
		pCatalog->Create(ConnectString);
	}
	catch(_com_error e) 
	{
		AfxMessageBox(_T("CreateMDB error!"));
	}
	pCatalog.Release();
	return TRUE;
}




// CADOConnection 訊息處理常式

