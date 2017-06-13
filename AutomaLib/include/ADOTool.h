#pragma once
#pragma warning( disable :4091)
#ifdef _M_X64
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif
#ifdef AUTOMALIB_EXPORTS
#define AUTOMALIB_API __declspec(dllexport)
#else
#define AUTOMALIB_API __declspec(dllimport)
#endif
#define cTmFormat_yymmdd_HHMMSS _T("%y%m%d-%H:%M:%S")
#define cTmFormat_yyyymmdd_HHMMSS _T("%Y%m%d-%H:%M:%S")
#define cTmFormat_mmdd_HHMMSS _T("%m%d-%H:%M:%S")
#define cTmFormat_dd_HHMMSS _T("%d-%H:%M:%S")
#define cTmFormat_DD_HHMMSS _T("%D-%H:%M:%S")
#define cTmFormat_HHMMSS _T("%H:%M:%S")
//�ɶ����榡,y�Q�i�줣�t�@�����~��,Y�Q�i��t�@�����~��,
//m�Q�i����(01-12),d�Q�i���(01-31),D�Q�i���(001-366),
//H�p��(00-23),M��(00-59),S��(00-59)
#define cDefaultLogFileName _T("Log.txt")	//�w�]��Log�ɦW.
#define cDefaultTimeFormat TIMEFORMAT::yymmdd_HHMMSS
#define charBackSlash char(92)
#define cBackupDBFileNo 6

#include "ADOConnection.h"
#include "ADORecordset.h"
#include <afxtempl.h>		// 50912
#include <iostream>
#include "fstream"
#include <sys/stat.h>	// to use FileExist

namespace AutoLib{

AUTOMALIB_API BOOL IsWow64();
class MBase;
AUTOMALIB_API class CADOTool
{
public:
	CADOTool(void);
	CADOTool(CString szCnnPath);	//50908
	~CADOTool(void);
	enum TIMEFORMAT {
			yymmdd_HHMMSS,
			yyyymmdd_HHMMSS,
			mmdd_HHMMSS,
			dd_HHMMSS,
			DD_HHMMSS,
			HHMMSS,
	}m_TIMEFORMAT;

	bool DeleteFile(CString szFileToDelete);
	// �Ҽ{�@�Ω�,�n�ާ@����Ʈw(mdb)�����ɦW�q�`�O���ۦP��,�ҥH�ǤJ��Ʈw���s�u
	bool ExportXLS(CADOConnection* pCnn, CString szTableName, CString szXlsName, CString szSheetName);
	bool ExportXLS(CADOConnection* pCnn, CString szTableName, CString szXlsName);	//�w�]��SheetName��TableName
	bool ExportXLS(CADOConnection* pCnn, CString szTableName);	//�ץX��XLS�ɦW�Ѿާ@�̦ۦ��w. �w�]��SheetName��TableName
	bool ExportTXT(CADOConnection* pCnn, CString szTableName, CString szTxtName);
	bool ExportTXT(CADOConnection* pCnn, CString szTableName);	//TXT�ɦW�Ѿާ@�̦ۦ��w.
	bool RecordLog(CString szDataToWrite, CString szLogFileName=cDefaultLogFileName, TIMEFORMAT tmFormat=cDefaultTimeFormat);
	bool CompactDB(CString szDBFullName);
	bool BackupDB(CString szDBFullName);
	bool CompactDB();	// �u�X��ܮ�,���ϥΪ̦ۤv��w�n���Y�״_����Ʈw�ɦW
	bool BackupDB();	// �u�X��ܮ�,���ϥΪ̦ۤv��w�n�ƥ�����Ʈw�ɦW
	bool RemoveDataBeforeSomeDay(CString szTableName, CString szFieldName, CString szDate);	//50908
	bool RemoveDataBeforeSomeDay(CString szTableName, CString szFieldName, CTime tmDate);	//�O�d����,�P�µ{���ۮe
	bool RemoveDataBeforeSomeDay(CADOConnection* pCnn, CString szTableName, CString szFieldName, CString szDate);
	bool RemoveDataBeforeSomeDay(CADOConnection* pCnn, CString szTableName, CString szFieldName, CTime tmDate);
	CString GetAppPath() { return(m_AppPath); };
	CString GetAppNameWithPath() { return(m_AppNameWithPath); };
	CString GetAppNameWithoutPath() { return(m_AppNameWithoutPath); };
	CString GetAppFileDate() { return(m_AppFileDate); };
	CString GetProductName() { return(m_ProductName); };
	CString GetCompanyName() { return(m_CompanyName); };
	CString GetCopyright() { return(m_Copyright); };
	bool SetCnnPath(CString szCnnPath);	
	AUTOMALIB_API bool FileExist(CString FileName);	
	AUTOMALIB_API bool EnableCtrlAltDel(bool bEnable);
	AUTOMALIB_API void MaskCtrlAltDel(const bool bMask);

private:
	CString m_TimeFormat;
	CString m_AppNameWithPath;			// ���ε{���������ɦW.(���R��Debug)
	CString m_AppPath;					// ���ε{����������|(���t���ε{���ɦW),���tDebug
	CString m_AppNameWithoutPath;		// ���ε{�����ɦW
	CString m_AppFileDate;
	CString m_ProductName;
	CString m_CompanyName;
	CString m_Copyright;
	CString m_CnnPath;					//50908
	bool DoSQLinCnnMD(CString szSQL);	//50908
	void SetPathInformation();			//50908
	void SetFileInformation(CString szFileName);	//50908
	bool DoSQL(CADOConnection* pCnn, CString szSQL);
};/* class ADOTool */

class CTurnOffComputer :public CObject
{
public :
CTurnOffComputer();
CTurnOffComputer(bool bHasAutoPowerOff);
enum OFFTYPE{
	SHUTDOWN,
	REBOOT,
	LOGOFF,
	STANDBY,
	HIBERNATE,
};
void SetAutoOff(bool bAutoOff);
void Active(OFFTYPE offtype);

~CTurnOffComputer();
private:
	//�q�����tAuto-Power-Off�\��,�h���i�ϥ�EWX_POWEROFF,�H�K�۰ʭ��}��
	bool m_hasAutoPowerOff;
};/* class CTurnOffComputer  */

// tell linker to link with version.lib for VerQueryValue, etc.
#pragma comment(linker,"/defaultlib:version.lib")

//////////////////
// CModuleVersion version info about a module.
// To use:
//
// CModuleVersion ver
// if (ver.GetFileVersionInfo("_T("mymodule))) {
//		// info is in ver, you can call GetValue to get variable info like
//		CString s = ver.GetValue(_T("CompanyName"));
// }
//
// You can also call the static fn DllGetVersion to get DLLVERSIONINFO.
//
class CModuleVersion : public VS_FIXEDFILEINFO 
{
protected:
	BYTE* m_pVersionInfo;	// all version info

	struct TRANSLATION 
		{
			WORD langID;			// language ID
			WORD charset;			// character set (code page)
		} m_translation;

public:
	CModuleVersion();
	virtual ~CModuleVersion();

	BOOL	GetFileVersionInfo(LPCTSTR modulename);
	CString	GetValue(LPCTSTR lpKeyName);
};/* class CModuleVersion */


/*
Author      : mah
Date        : 13.06.2002
Description : 
    similar to Visual Basic InputBox
*/
const int INPUTBOX_WIDTH = 400;
const int INPUTBOX_HEIGHT = 320;	//����250,�����11��. 320��13��

class CInputBox  
{
    static HFONT m_hFont;
    static HWND  m_hWndInputBox;
    static HWND  m_hWndParent;
    static HWND  m_hWndEdit;
    static HWND  m_hWndOK;
    static HWND  m_hWndCancel;
    static HWND  m_hWndPrompt;

    static HINSTANCE m_hInst;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
    // text from InputBox
	LPTSTR Text;
    BOOL DoModal(LPCTSTR szCaption, LPCTSTR szPrompt);

	CInputBox(HWND hWndParent);
	virtual ~CInputBox();
};	/* CInputBox */


/* CIniFile */
AUTOMALIB_API class CIniFile  
{
private:
	CString m_LastError;
	bool	m_isIniFileOpen;
private:
	//stores path of ini file to read/write
	CString FilePath;
	//stores name of ini file to read/write
	CString FileName;
	CString FullFileName;

public:

	AUTOMALIB_API void SetName(CString Name);
	AUTOMALIB_API void SetPath(CString Path);
	AUTOMALIB_API CIniFile();
	AUTOMALIB_API virtual ~CIniFile();
	AUTOMALIB_API BOOL GetItemString(CString Section,CString Key,CString& strData,CString strDefault=_T(""));
	AUTOMALIB_API BOOL WriteItemString(CString Section, CString Key, CString Data);
}; /* CIniFile */

}//namespace AutoLib