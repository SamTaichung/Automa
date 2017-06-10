// Log.h: interface for the CLog class.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLog  
{
public:
	/*CLog();*/	// can be used if parameters can be got from the registry
	CLog(CString strLogFileName,DWORD dwMaxFileLength = 1000,BOOL bDisplayLogSizeWarning = TRUE);
	virtual ~CLog();

	void LogMessage(UINT uMsgID);			// log the messages using StringTable Id's
	void LogMessage(CString strMessage);	// log using Cstring objects
	BOOL ClearLogFile();					// Delete the log file	

private:

	CString m_strLogfileName;	// Log file name
	BOOL  m_bUseLog;			// Is Logging being Used
	DWORD m_dwMaxLen;			// Max Length of LogFile
	CFile *m_pLogFile;			// File Pointer to Log File
	BOOL m_bDisplayDlg;			// Has a lof size warning dialog already been displayed
	
	void Initialize();			// used by C'tors
	void DisplayWarningMessage();			// Display a warning message 
};
