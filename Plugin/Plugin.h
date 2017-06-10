
// Plugin.h : Plugin 應用程式的主標頭檔
//
#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 包含此檔案前先包含 'stdafx.h'"
#endif

#include "resource.h"       // 主要符號


// CPluginApp:
// 請參閱實作此類別的 Plugin.cpp
//

class CPluginApp : public CWinApp
{
public:
	CPluginApp();


// 覆寫
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 程式碼實作
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPluginApp theApp;
