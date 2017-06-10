#include "StdAfx.h"		// if remark this line, compile fatal error C1010 occurs.
#include "ADOTool.h"
typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;

using namespace AutoLib;
//本程式庫用到 ADOConnection類別, 必須做下列動作.
/*	-----------------------------
	CWinApp::InitInstance();	// 在InitInstance()之後, 執行CoInitialize(NULL);
	HRESULT hr = CoInitialize(NULL); 
	if (FAILED(hr))
	{
		AfxMessageBox(_T("COM CoInitialize Failed!");
		return FALSE;
	}	//任何一個Thread在使用COM元件時,必須執行 CoInitialize(NULL);
	-----------------------------*/
BOOL IsWow64() {
	BOOL bIsWow64 = FALSE;
	//IsWow64Process is not available on all supported versions
	//of Windows. Use GetModuleHandle to get a handle to the
	//DLL that contains the function and GetProcAddress to get
	//a pointer to the function if available.
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			//handle error
		}
	}
	return bIsWow64;
}
// 根據執行檔檔名切到對應的工作目錄,並且找出該執行檔的版次(日期),公司,...等資訊
CADOTool::CADOTool(void)
{
	m_CnnPath="NULL";
	m_TIMEFORMAT = TIMEFORMAT::yymmdd_HHMMSS;
	m_TimeFormat = cTmFormat_yymmdd_HHMMSS;
	SetPathInformation();
}

CADOTool::CADOTool(CString szCnnPath)
{
	m_CnnPath=szCnnPath;
	m_TIMEFORMAT = TIMEFORMAT::yymmdd_HHMMSS;
	m_TimeFormat = cTmFormat_yymmdd_HHMMSS;
	SetPathInformation();
}

CADOTool::~CADOTool(void)
{
}

bool CADOTool::FileExist(CString FileName)
{
	CFile cf;
	CFileStatus status;
	try	{
		if ( cf.GetStatus(FileName, status) )	{
			//檔案存在,則 GetStatus回傳true.
			return true;
		}else{
			return false;
		}
	}catch(...)	{
		return false;
	}
	//struct stat my_stat;
	//return (stat(FileName, &my_stat)==0);
}

void CADOTool::SetPathInformation(void)
{
	wchar_t namewithpath[255];
	GetModuleFileName(NULL, namewithpath, 255);
	m_AppPath = namewithpath;
	m_AppNameWithPath = namewithpath;
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];

	_wsplitpath_s(namewithpath, drive, dir, fname, ext );

	m_AppNameWithoutPath.Format(_T("%s%s"), fname, ext);

	m_AppPath.Format(_T("%s%s"), drive, dir);
	m_AppPath.MakeLower();
	m_AppPath.Replace(_T("debug\\"), _T(""));	// 刪去debug子目錄
	if (m_AppPath.Right(1)!=_T("\\"))	{
		m_AppPath+=_T("\\");
	}
	SetFileInformation(m_AppNameWithPath);
}

void CADOTool::SetFileInformation(CString szFileName)
{
	CModuleVersion oVer;
	if (oVer.GetFileVersionInfo(szFileName))	{
		m_CompanyName = oVer.GetValue(_T("CompanyName"));
		m_ProductName = oVer.GetValue(_T("ProductName"));
		m_AppFileDate = " ";
		m_AppFileDate += oVer.GetValue(_T("ProductVersion"));
		m_AppFileDate.Replace(_T("),_T("),_T("."));
		m_AppFileDate.Remove(_T(' '));
		m_Copyright = oVer.GetValue(_T("LegalCopyright"));
	}else	{
		m_CompanyName = m_ProductName = m_AppFileDate = m_Copyright = _T("Not Available");
	}
}


// Record Log Information(szDataToWrite) in a TEXT file(szLogFileName).
bool CADOTool::RecordLog(CString szDataToWrite, CString szLogFileName, TIMEFORMAT tmFormat)
{
	switch (tmFormat)
	{
	case TIMEFORMAT::DD_HHMMSS:
		m_TimeFormat = cTmFormat_DD_HHMMSS;
		break;
	case TIMEFORMAT::dd_HHMMSS:
		m_TimeFormat = cTmFormat_dd_HHMMSS;
		break;
	case TIMEFORMAT::HHMMSS:
		m_TimeFormat = cTmFormat_HHMMSS;
		break;
	case TIMEFORMAT::mmdd_HHMMSS:
		m_TimeFormat = cTmFormat_mmdd_HHMMSS;
		break;
	case TIMEFORMAT::yyyymmdd_HHMMSS:
		m_TimeFormat = cTmFormat_yyyymmdd_HHMMSS;
		break;
	case TIMEFORMAT::yymmdd_HHMMSS:
	default:
		m_TimeFormat = cTmFormat_yymmdd_HHMMSS;
		break;
	};

	CFile cf;
	CTime t;
	CString strTm;
	t = CTime::GetCurrentTime();	
	strTm.Format(_T("%c\n%s"),wchar_t(13),t.Format( m_TimeFormat ));
	szDataToWrite =  strTm + szDataToWrite;
	if ( cf.Open(szLogFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite) )
	{
		try
		{
		cf.SeekToEnd();
		cf.Write( szDataToWrite, szDataToWrite.GetLength() );
		return true;
		}
		catch( CFileException *e )
		{
			CString strError;
			strError.Format(_T("開啟寫入Log.txt失敗. 錯誤碼為 %d"), e->m_cause );
			e->Delete();
			AfxMessageBox( strError );
			return false;
		}
	}
	return false;
}

// 刪除檔案(絕對檔名)
bool CADOTool::DeleteFile(CString szFileToDelete)
{
	CFile cf;
	CFileStatus status;
	try
	{
		if ( cf.GetStatus(szFileToDelete, status) )	//檔案存在,則 GetStatus回傳true.
		{
			cf.Remove(szFileToDelete);
			return true;
		}else{
			return false;
		}
	}
	catch (CFileException* pEx)
	{
		AfxMessageBox(_T("File ") + szFileToDelete + _T(" cannot be removed\n"));
		pEx->Delete();
		return false;
	}
}

// 匯出資料表為EXCEL的檔案(XLS格式), 匯出的檔案名為手動選擇.
bool CADOTool::ExportXLS(CADOConnection* pCnn, CString szTableName)
{
	CFileDialog fdlg(false, NULL, m_AppPath + szTableName + _T(".xls"),
			OFN_READONLY, _T("Excel Files (*.xls) |*.xls"), NULL);
	fdlg.m_pOFN->lpstrTitle =_T("請選擇(輸入)要匯出的XLS檔檔名");
	fdlg.DoModal();
	return ( ExportXLS(pCnn, szTableName, fdlg.GetFileName(), szTableName) );
}

bool CADOTool::ExportXLS(CADOConnection* pCnn, CString szTableName, CString szXlsName)
{
	return ( ExportXLS(pCnn, szTableName, szXlsName, szTableName) );
}

bool CADOTool::ExportXLS(CADOConnection* pCnn, CString szTableName, CString szXlsName, CString szSheetName)
{
	CString szXlsNameWithDir;
	if ("\\"==szXlsName.Mid(2,1))	//==成立,假定是絕對路徑.
	{
		szXlsNameWithDir = szXlsName;
	}else{
		szXlsNameWithDir = m_AppPath + szXlsName;
	};
// "SELECT Table.Fields INTO [Excel 8.0;DATABASE=路徑檔名].[工作表名] FROM [匯出資料的表名]"
	CString szSQL;
	szSQL = _T("SELECT * INTO [Excel 8.0;DATABASE=") + szXlsNameWithDir + _T("].[") +
			szSheetName +_T("] FROM [") + szTableName + _T("]");
	CADOTool::DeleteFile(szXlsNameWithDir);

	return ( DoSQL( pCnn, szSQL ) );
}

// 匯出資料表為TXT的檔案, 匯出的檔案名為手動選擇.
bool CADOTool::ExportTXT(CADOConnection* pCnn, CString szTableName)
{
	CFileDialog fdlg(false, NULL, m_AppPath+_T("ErrHistory.txt"),
			OFN_READONLY, _T("Text Files (*.txt) |*.txt"), NULL);
	fdlg.m_pOFN->lpstrTitle = _T("請選擇(輸入)要匯出的txt檔檔名");
	fdlg.DoModal();
	return ( ExportTXT(pCnn, szTableName, fdlg.GetFileName() ) );
}

bool CADOTool::ExportTXT(CADOConnection* pCnn, CString szTableName, CString szTxtName)
{
	CString szTxtFileDir, szTxtFileName;
	if (_T("\\")==szTxtName.Mid(2,1))	//==成立,假定是絕對路徑.
	{
		int nIndex = szTxtName.ReverseFind(charBackSlash);	//找出最右邊的 '\' 位置.
		szTxtFileDir = szTxtName.Mid(0, nIndex+1);	//取得路徑. 字元數=nIndex+1, (0~nIndex)
		szTxtFileName = szTxtName.Mid(nIndex+1);	//取得檔名(不含路徑)
	}else{
		szTxtFileDir = m_AppPath;
		szTxtFileName = szTxtName;
	};
// "SELECT Table.Fields INTO [Text;DATABASE=路徑].[檔名] FROM [匯出資料的表名]"
	CString szSQL;
	szSQL = _T("SELECT * INTO [Text;DATABASE=") + szTxtFileDir + _T("].[") +
			szTxtFileName +_T("] FROM [") + szTableName + _T("]");
	CADOTool::DeleteFile( szTxtFileDir + szTxtFileName );

	return ( DoSQL( pCnn, szSQL ) );
}

// 需在stdafx.h 引用Microsoft Jet And Replication Objects x.x Library. 方法如下.
// #import "c:\\program files\\common files\\system\\ado\\msjro.dll"	//宣告必須放在ado15後面
bool CADOTool::CompactDB(CString szDBFullName)
{
	CString strOld, strNew;
	CString strCD1, strCD2;
#ifndef _M_X64
	try{

		JRO::IJetEnginePtr jet(__uuidof(JRO::JetEngine));
		strOld = szDBFullName;
        strNew = m_AppPath + _T("chngMe.mdb");
		CADOTool::DeleteFile(strNew);
		strCD1 = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + strOld;
		strCD2 = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + strNew + _T(";Jet OLEDB:Engine Type=5");
		jet->CompactDatabase(strCD1.AllocSysString(),strCD2.AllocSysString());
		CADOTool::DeleteFile(strOld);
		CopyFile(strNew, strOld,false);
		return true;
	}catch(_com_error &e)
	{
		CString csError;
		csError =(LPCTSTR) e.Description();
		AfxMessageBox(csError,0);
		return false;
	}
#endif 
	return true;
}

bool CADOTool::BackupDB(CString szDBFullName)
{
	CString szTmpDBName;
	if ( (_T(".mdb")==szDBFullName.Right(4)) || (_T(".MDB")==szDBFullName.Right(4)) )
	{
		szTmpDBName=szDBFullName.Mid(0, szDBFullName.GetLength()-4);
	}else{
        return false;
	}
	CString szFile1, szOldestFile, szNameTail;
	bool isBackupNameFound=false;
	CTime tmpMinimumTime= CTime::GetCurrentTime();
	CFile cf;
	CFileStatus status;
	for (int i=0; (i<cBackupDBFileNo) && (!isBackupNameFound) ; i++)
	{
		szNameTail.Format(_T("%d%s"), i,_T(".mdb"));
		szFile1 = m_AppPath + szTmpDBName + szNameTail;
		if ( cf.GetStatus(szFile1, status) )	//檔案存在,則 GetStatus回傳true.
		{
			//檢查檔案時間是否最舊.
			if ( tmpMinimumTime > status.m_ctime )	{
				tmpMinimumTime = status.m_mtime;
				szOldestFile = szFile1;	//記住最舊日期的檔案名稱
			}
		}else{
			isBackupNameFound = true;
			szOldestFile = "";
		}
	}
	if ( ""==szOldestFile )	{
		CopyFile(szDBFullName, szFile1, false);
	}else{
		CopyFile(szDBFullName, szOldestFile, false);
	}
	return true;
}

bool CADOTool::BackupDB()
{
// CFileDialog的使用方法. 1.Create an instance,  2.Initialize m_pOFN structure,  3.Call DoModal
	CFileDialog fdlg(false, NULL, NULL,
			OFN_READONLY, _T("Database Files (*.mdb) |*.mdb"), NULL);
	fdlg.m_pOFN->lpstrTitle = _T("請選擇(輸入)要備份的mdb檔檔名");
	fdlg.DoModal();
	return ( BackupDB( fdlg.GetFileName() ) );
}

bool CADOTool::CompactDB()
{
	CFileDialog fdlg(false, NULL, NULL,
			OFN_READONLY, _T("Database Files (*.mdb) |*.mdb"), NULL);
	fdlg.m_pOFN->lpstrTitle = _T("請選擇(輸入)要壓縮,修復的mdb檔檔名");
	fdlg.DoModal();
	return ( CompactDB( fdlg.GetFileName() ) );
}

bool CADOTool::RemoveDataBeforeSomeDay(CADOConnection* pCnn, CString szTableName, CString szFieldName, CString szDate)
{
	szDate.Format(_T("%s %s"), szDate.Left(8), _T("00:00:00"));	//以午夜為時間基準.
	CString szSQL;
	szSQL = _T("DELETE * FROM ") + szTableName + _T(" Where ") + szFieldName
		+ _T(" < #") + szDate + _T("#");
	return ( DoSQL(pCnn, szSQL) );
}

bool CADOTool::RemoveDataBeforeSomeDay(CADOConnection* pCnn, CString szTableName, CString szFieldName, CTime tmDate)
{
	CString szDate;
	szDate.Format(_T("%s"), tmDate.Format(_T("%Y/%m/%d 00:00:00")));	//日期格式 YYYY/mm/dd HH:MM:SS
	CString szSQL;
	szSQL = _T("DELETE * FROM ") + szTableName + _T(" Where ") + szFieldName
		+ _T(" < #") + szDate + _T("#");
	return ( DoSQL(pCnn, szSQL) );
}


// 傳入的連線物件,須保持在Open狀態,才能正常運作.
// (不知如何檢查 連線物件CADOConnection 的連線狀態)
bool CADOTool::DoSQL(CADOConnection* pCnn, CString szSQL)
{
	pCnn->BeginTrans();
	if ( pCnn->ExecuteSQL(szSQL) )	{
		pCnn->CommitTrans();
		return true;
	}else	{
		pCnn->CommitTrans();
		return false;
	}
}

//***50908 ***************與舊程式相容,故保留 無連線參數 的函數*****************50908 (Start)
bool CADOTool::RemoveDataBeforeSomeDay(CString szTableName, CString szFieldName, CString szDate)
{
	szDate.Format(_T("%s %s"), szDate.Left(8), _T("00:00:00"));	//以午夜為時間基準.
	CString strSQL;
	strSQL = _T("DELETE * FROM ") + szTableName + _T(" Where ") + szFieldName
		+ _T(" < #") + szDate + _T("#");
	return ( CADOTool::DoSQLinCnnMD(strSQL) );
}
bool CADOTool::RemoveDataBeforeSomeDay(CString szTableName, CString szFieldName, CTime tmDate)
{
	CString szDate;
	szDate.Format(_T("%s"), tmDate.Format(_T("%Y/%m/%d 00:00:00")));
	CString strSQL;
	strSQL = _T("DELETE * FROM ") + szTableName + _T(" Where ") + szFieldName
		+ _T(" < #") + szDate + _T("#");
	return ( CADOTool::DoSQLinCnnMD(strSQL) );
}
bool CADOTool::DoSQLinCnnMD(CString szSQL)
{
	CString strPath;
	CADOConnection m_cnnTmp;
	if(m_CnnPath== _T("NULL"))	{	//與舊的COG程式相容
		strPath=m_AppPath + _T("DataBase\\MachineData\\Machine.MDB");
		m_cnnTmp.m_ConnectionString= _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + strPath;
	}else	{
		m_cnnTmp.m_ConnectionString= _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + m_CnnPath;
	}
	if(!m_cnnTmp.Open())	{
		return false;
	}
	//加上BeginTrans 和 CommitTrans, 在異常記錄一覽畫面內刪資料,仍有時資料已刪但仍顯示未更新
	m_cnnTmp.BeginTrans();
	if ( m_cnnTmp.ExecuteSQL(szSQL) )	{
		m_cnnTmp.CommitTrans();
		m_cnnTmp.Close();
		return true;
	}else	{
		m_cnnTmp.CommitTrans();
		m_cnnTmp.Close();
		return false;
	};
}
bool CADOTool::SetCnnPath(CString szCnnPath)
{
	m_CnnPath=szCnnPath;
	return TRUE;
}
//***50908 ***************與舊程式相容,故保留 無連線參數 的函數*****************50908 (End)



//60109 提供外部直接修改CtrlAltDel的遮罩功能
void CADOTool::MaskCtrlAltDel(const bool bMask)
{
	try	{
		if (bMask)	{
			HANDLE	hFile;
			hFile = CreateFile(_T("c:\\disable.ini"), GENERIC_WRITE,
				FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
				NULL, CREATE_ALWAYS, NULL, NULL);
			if (hFile != INVALID_HANDLE_VALUE )	{
				CloseHandle(hFile);	//建檔即可
			}else	{
				AfxMessageBox(_T("無法建立檔案c:\\disable.ini,所以無法遮罩CtrlAltDel"));
			};
		}else	{
			::DeleteFile(_T("c:\\disable.ini"));	//恢復CtrlAltDel功能
		}
	}catch(...)	{
		AfxMessageBox(_com_error(GetLastError()).ErrorMessage(),MB_ICONSTOP);
	}
}



// ---class ADOTool definitions--- END  *******************

// +++class CTurnOffComputer definitions+++ BEGIN  //////////////////////////////////////////
CTurnOffComputer::CTurnOffComputer()
{
	m_hasAutoPowerOff = false;	// 預設電腦無自動Power Off功能.
}

CTurnOffComputer::CTurnOffComputer(bool bHasAutoPowerOff)
{
	m_hasAutoPowerOff = bHasAutoPowerOff;
}
CTurnOffComputer::~CTurnOffComputer()
{
}
void CTurnOffComputer::SetAutoOff(bool bAutoOff)
{
	m_hasAutoPowerOff=bAutoOff;
}

void CTurnOffComputer::Active(OFFTYPE offtype)
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 
	
	if (OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))	{
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
		tkp.PrivilegeCount = 1; 
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
		if(AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))	{
			switch (offtype)
			{
				case OFFTYPE::SHUTDOWN:
					if (m_hasAutoPowerOff)
						ExitWindowsEx(EWX_SHUTDOWN|EWX_POWEROFF|EWX_FORCE,0);
					else
						ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE,0);
					break;
				case OFFTYPE::REBOOT:
					ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
					break;
				case OFFTYPE::LOGOFF:
					ExitWindowsEx(EWX_LOGOFF|EWX_FORCE,0);
					break;
				case OFFTYPE::STANDBY:
					SetSystemPowerState( TRUE, TRUE);
					break;
				case OFFTYPE::HIBERNATE:
					SetSystemPowerState( FALSE, FALSE );
					break;
			};	// switch (offtype)
		};
	}
}
// ---class CTurnOffComputer definitions--- END  *******************

// +++class CTurnOffComputer definitions+++ BEGIN  //////////////////////////////////////////
// 實際取出VersionInfo的程式範例, 在 /*  和 */ 之間, 
/*
CSplashWnd::CSplashWnd()
{
	// get the fileversion to print over the bitmap;
	TCHAR	szProg[MAX_PATH * 2];
	::GetModuleFileName(NULL, szProg, sizeof(szProg)/sizeof(TCHAR)); 
	
	CModuleVersion oVer;
	if(oVer.GetFileVersionInfo(szProg))
	{
		// info is in ver, you can call GetValue to get variable info 
		m_sCompanyName = oVer.GetValue(_T("CompanyName"));
		m_sProductName = oVer.GetValue(_T("ProductName"));
		m_sFileVersion = "Build:";
		m_sFileVersion += oVer.GetValue(_T("ProductVersion"));
		m_sFileVersion.Replace(_T("),_T("), _T(".");
		m_sFileVersion.Remove(' ');
		m_sCopyright = oVer.GetValue(_T("LegalCopyright"));
	}
}
*/

CModuleVersion::CModuleVersion()
{
	m_pVersionInfo = NULL;				// raw version info data 
}

// Destroy: delete version info
CModuleVersion::~CModuleVersion()
{
	delete [] m_pVersionInfo;
}

// Get file version info for a given module
BOOL CModuleVersion::GetFileVersionInfo(LPCTSTR modulename)
{
	m_translation.charset = 1252;		// default = ANSI code page (1252=English)
	memset((VS_FIXEDFILEINFO*)this, 0, sizeof(VS_FIXEDFILEINFO));

	// get module handle
	TCHAR filename[_MAX_PATH];
	HMODULE hModule = ::GetModuleHandle(modulename);
	if (hModule==NULL && modulename!=NULL)
		return FALSE;

	// get module file name
	DWORD len = GetModuleFileName(hModule, filename,
		sizeof(filename)/sizeof(filename[0]));
	if (len <= 0)
		return FALSE;

	// read file version info
	DWORD dwDummyHandle; // will always be set to zero
	len = GetFileVersionInfoSize(filename, &dwDummyHandle);
	if (len <= 0)
		return FALSE;

	m_pVersionInfo = new BYTE[len]; // allocate version info
	if (!::GetFileVersionInfo(filename, 0, len, m_pVersionInfo))
		return FALSE;

	LPVOID lpvi;
	UINT iLen;
	if (!VerQueryValue(m_pVersionInfo, _T("\\"), &lpvi, &iLen))
		return FALSE;

	// copy fixed info to myself, which am derived from VS_FIXEDFILEINFO
	*(VS_FIXEDFILEINFO*)this = *(VS_FIXEDFILEINFO*)lpvi;

	// Get translation info
	if (VerQueryValue(m_pVersionInfo,
		_T("\\VarFileInfo\\Translation"), &lpvi, &iLen) && iLen >= 4)
	{
		m_translation = *(TRANSLATION*)lpvi;
		TRACE(_T("code page = %d\n"), m_translation.charset);
	}

	return dwSignature == VS_FFI_SIGNATURE;
}

// Get string file info.   Key name is something like "CompanyName". "ProductVersion"
CString CModuleVersion::GetValue(LPCTSTR lpKeyName)
{
	CString sVal;
	if (m_pVersionInfo) {
		// To get a string value must pass query in the form
		//
		//    "\StringFileInfo\<langID><codepage>\keyname"
		//
		// where <lang-codepage> is the languageID concatenated with the
		// code page, in hex. Wow.
		//
		CString query;
		query.Format(_T("\\StringFileInfo\\%04x%04x\\%s"),
			m_translation.langID,
			m_translation.charset,
			lpKeyName);

		LPCTSTR pVal;
		UINT iLenVal;
		if (VerQueryValue(m_pVersionInfo, (LPTSTR)(LPCTSTR)query,
				(LPVOID*)&pVal, &iLenVal))
		{
			sVal = pVal;
		}
	}
	return sVal;
}
// ---class CModuleVersion definitions--- END  *******************

// +++class CInputBox definitions+++ BEGIN  //////////////////////////////////////////

HFONT CInputBox::m_hFont = NULL;
HWND  CInputBox::m_hWndInputBox = NULL;
HWND  CInputBox::m_hWndParent = NULL;
HWND  CInputBox::m_hWndEdit = NULL;
HWND  CInputBox::m_hWndOK = NULL;
HWND  CInputBox::m_hWndCancel = NULL;
HWND  CInputBox::m_hWndPrompt = NULL;

HINSTANCE CInputBox::m_hInst = NULL;
/*
Author      : mah
Date        : 13.06.2002
Description : 
    Constructs window class InputBox
*/
CInputBox::CInputBox(HWND hWndParent)
{
	HINSTANCE hInst = GetModuleHandle(NULL);

	WNDCLASSEX wcex;

	if (!GetClassInfoEx(hInst, _T("InputBox"), &wcex))	{
		wcex.cbSize = sizeof(WNDCLASSEX); 

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInst;
		wcex.hIcon			= NULL;	//LoadIcon(hInst, (LPCTSTR)IDI_MYINPUTBOX);
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= _T("InputBox");
		wcex.hIconSm		= NULL;

		if (RegisterClassEx(&wcex) == 0)
			MessageBox(NULL, _T("Can't create CInputBox!"), _T("Error"), MB_OK);
	}
    m_hWndParent = hWndParent;
    Text = NULL;
}

CInputBox::~CInputBox()
{
    if (Text) delete[] Text;
}

/*
Author      : mah
Date        : 13.06.2002
Description : Window procedure
*/
LRESULT CALLBACK CInputBox::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LOGFONT lfont;

	switch (message) 
	{
		case WM_CREATE:
            // font
            memset(&lfont, 0, sizeof(lfont));
            lstrcpy(lfont.lfFaceName, _T("Arial"));
            lfont.lfHeight = 16;
            lfont.lfWeight = FW_NORMAL;//FW_BOLD;
            lfont.lfItalic = FALSE;
            lfont.lfCharSet = DEFAULT_CHARSET;
            lfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
            lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
            lfont.lfQuality = DEFAULT_QUALITY;
            lfont.lfPitchAndFamily = DEFAULT_PITCH;
	        m_hFont = CreateFontIndirect(&lfont);
	        m_hInst = GetModuleHandle(NULL);

			// creating Edit
			m_hWndEdit = CreateWindowEx(WS_EX_STATICEDGE,
				_T("edit"),_T(""),
				WS_VISIBLE | WS_CHILD  | WS_TABSTOP | ES_AUTOHSCROLL, 
				5, INPUTBOX_HEIGHT - 50, INPUTBOX_WIDTH - 16, 20, 
				hWnd, 
				NULL, 
				m_hInst, 
				NULL); 
            // setting font
			SendMessage(m_hWndEdit, WM_SETFONT, (WPARAM)m_hFont, 0);
            // button OK
			m_hWndOK = CreateWindowEx(WS_EX_STATICEDGE,
				_T("button"),_T("OK"),
				WS_VISIBLE | WS_CHILD | WS_TABSTOP, 
				INPUTBOX_WIDTH - 100, 10, 90, 25, 
				hWnd, 
				NULL, 
				m_hInst, 
				NULL); 
            // setting font
            SendMessage(m_hWndOK, WM_SETFONT, (WPARAM)m_hFont, 0);
            // button Cancel
			m_hWndCancel = CreateWindowEx(WS_EX_STATICEDGE,
				_T("button"),_T("Cancel"),
				WS_VISIBLE | WS_CHILD | WS_TABSTOP, 
				INPUTBOX_WIDTH - 100, 40, 90, 25, 
				hWnd, 
				NULL, 
				m_hInst, 
				NULL); 
            // setting font
            SendMessage(m_hWndCancel, WM_SETFONT, (WPARAM)m_hFont, 0);
            // static Propmpt
			m_hWndPrompt = CreateWindowEx(WS_EX_STATICEDGE,
				_T("static"),_T(""),
				WS_VISIBLE | WS_CHILD, 
				5, 10, INPUTBOX_WIDTH - 110, INPUTBOX_HEIGHT - 70, 
				hWnd, 
				NULL, 
				m_hInst, 
				NULL);
            // setting font
            SendMessage(m_hWndPrompt, WM_SETFONT, (WPARAM)m_hFont, 0);
            SetFocus(m_hWndEdit);
			break;
		case WM_DESTROY:
			DeleteObject(m_hFont);
			EnableWindow(m_hWndParent, TRUE);
			SetForegroundWindow(m_hWndParent);
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
        case WM_COMMAND:
            switch (HIWORD(wParam))
            {
                case BN_CLICKED:
                    if ((HWND)lParam == m_hWndOK)
                        PostMessage(m_hWndInputBox, WM_KEYDOWN, VK_RETURN, 0);
                    if ((HWND)lParam == m_hWndCancel)
                        PostMessage(m_hWndInputBox, WM_KEYDOWN, VK_ESCAPE, 0);
                    break;
            }
            break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

/*
Author      : mah
Date        : 13.06.2002
Description : 
        Constructs InputBox window
*/
BOOL CInputBox::DoModal(LPCTSTR szCaption, LPCTSTR szPrompt)
{
	RECT r;
	GetWindowRect(GetDesktopWindow(), &r);

	m_hWndInputBox = CreateWindowEx(WS_EX_TOOLWINDOW, 
                _T("InputBox"),
                szCaption,
                WS_POPUPWINDOW | WS_CAPTION | WS_TABSTOP, 
                (r.right - INPUTBOX_WIDTH) / 2, (r.bottom - INPUTBOX_HEIGHT) / 2,
                INPUTBOX_WIDTH, INPUTBOX_HEIGHT,
                m_hWndParent,
                NULL,
                m_hInst,
                NULL);
    if(m_hWndInputBox == NULL)
        return FALSE;

    SetWindowText(m_hWndPrompt, szPrompt);
    SetForegroundWindow(m_hWndInputBox);

	EnableWindow(m_hWndParent, FALSE);

    ShowWindow(m_hWndInputBox, SW_SHOW); 
    UpdateWindow(m_hWndInputBox);

    BOOL ret = 0;
	MSG msg;
    HWND hWndFocused;

    while (GetMessage(&msg, NULL, 0, 0))	{       
		if (msg.message == WM_KEYDOWN)	{
			if (msg.wParam == VK_ESCAPE)	{
				SendMessage(m_hWndInputBox, WM_DESTROY, 0, 0);
                ret = 0;
            }
			if (msg.wParam == VK_RETURN)	{
                int nCount = GetWindowTextLength(m_hWndEdit);
                nCount++;
                if (Text)	{
                    delete[] Text;
                    Text = NULL;
                }
                Text = new TCHAR[nCount];
                GetWindowText(m_hWndEdit, Text, nCount);
			    SendMessage(m_hWndInputBox, WM_DESTROY, 0, 0);
                ret = 1;
            }
			if (msg.wParam == VK_TAB)	{
                hWndFocused = GetFocus();
                if (hWndFocused == m_hWndEdit) SetFocus(m_hWndOK);
                if (hWndFocused == m_hWndOK) SetFocus(m_hWndCancel);
                if (hWndFocused == m_hWndCancel) SetFocus(m_hWndEdit);
            }
		}	// ---if (msg.message == WM_KEYDOWN)
        TranslateMessage(&msg);
		DispatchMessage(&msg);      
    }	// ---while (GetMessage(&msg, NULL, 0, 0))
	return ret;
}
// ---class CInputBox definitions--- END  *******************




// +++class CIniFile definitions+++ BEGIN  //////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
//default constructor
CIniFile::CIniFile()
{
	m_isIniFileOpen = false;
}
//default destructor
CIniFile::~CIniFile()
{
}
BOOL CIniFile::WriteItemString(CString Section, CString Key, CString Data)
{
	wchar_t pszData[MAX_PATH];
	CString strFile;
	strFile.Format(_T("%s%s"), FilePath, FileName);
	if (!::PathFileExists(strFile))
	{

		// UTF16-LE BOM(FFFE)
		WORD wBOM = 0xFEFF;
		DWORD NumberOfBytesWritten;

		HANDLE hFile = ::CreateFile(strFile, GENERIC_WRITE, 0,
			NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		::WriteFile(hFile, &wBOM, sizeof(WORD), &NumberOfBytesWritten, NULL);
		::CloseHandle(hFile);
	}
	return WritePrivateProfileString(Section, Key, Data, strFile);
}
BOOL CIniFile::GetItemString(CString Section, CString Key, CString &Data, CString strDefault)
{
	wchar_t pszData[MAX_PATH];
	CString strFile;
	strFile.Format(_T("%s%s"), FilePath, FileName);
	if (GetPrivateProfileString(Section, Key, strDefault, pszData, MAX_PATH, strFile))
	{
		Data = pszData;
		return TRUE;
	}
	else {
		Data = strDefault;
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////
// Public Functions
/////////////////////////////////////////////////////////////////////

//sets path of ini file to read and write from
void CIniFile::SetPath(CString Path)
{
	FilePath = Path;
}
//sets name of ini file to read and write from
void CIniFile::SetName(CString Name)
{
	FileName=Name+_T(".ini");
	FullFileName = FilePath+FileName;
}
// ---class CIniFile definitions--- END  *******************
//51207 改寫機碼(MyGina.dll必須先複製到system32目錄下),
bool CADOTool::EnableCtrlAltDel(bool bEnable)
{
	wchar_t szSpace[255];
	for (int i = 0; i<255; ++i) {
		szSpace[i] = 0;
	}
	::GetWindowsDirectory(szSpace, 255);
	CString szTmp, szMsg;
	szTmp.Format(_T("%s%s"), szSpace, _T("\\system32\\MyGina.dll"));
	if (!FileExist(szTmp))
		return false;	//若檔案MyGina.dll不存在,則無法Disable CtrlAltDel
	HKEY hKey;
	LONG lRet;
	const int BUFSIZE = 255;
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon"),
		0, KEY_ALL_ACCESS, &hKey);	// 注意: 若用 KEY_QUERY_VALUE 開啟,則無法寫入.
	if (ERROR_SUCCESS == lRet) {

		MaskCtrlAltDel(!bEnable);	//建立或刪除"c:\disable.ini"檔案
									//60107 一律使用MyGina.dll, 若"c:\disable.ini"存在則擋掉CtrlAltDel.
		szTmp.Format(_T("%s"), _T("MyGina.dll"));
		BYTE szGinaDLLName[BUFSIZE];
		DWORD dwBufLen = BUFSIZE;
		int i;
		for (i = 0; i < szTmp.GetLength(); ++i) {
			szGinaDLLName[i] = szTmp.GetAt(i);
		}
		szGinaDLLName[i] = _T('\0');

		RegSetValueEx(hKey, _T("GinaDLL"), NULL, REG_SZ,
			szGinaDLLName, DWORD(i));
		AfxMessageBox(szTmp);

		lRet = RegQueryValueEx(hKey, _T("GinaDLL"), NULL, NULL,
			(LPBYTE)szGinaDLLName, &dwBufLen);
		if ((lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE))
			return false;
		szMsg.Format(_T("%s"), szGinaDLLName);
	}
	else {
		szMsg.Format(_T("%s"), _T("RegOpenKeyEx 失敗!"));
		return false;
	}
	AfxMessageBox(szMsg);
	RegCloseKey(hKey);
	return true;
}